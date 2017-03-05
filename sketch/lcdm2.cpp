#include "config.h"
#if Device==DEVICE_LCDM2_DISPENSER

#include "Arduino.h"
#include "lcdm2.h"

const byte SOH = 0x01;
const byte STX = 0x02;
const byte ETX = 0x03;
const byte EOT = 0x04;
const byte ACK = 0x06;
const byte NCK = 0x15;
const byte ID  = 0x50;

const byte CMD_PURGE = 0x44;
const byte CMD_UPPER_DISPENSE = 0x45;
const byte CMD_STATUS = 0x46;
const byte CMD_ROM_VERSION = 0x47;
const byte CMD_LOWER_DISPENSE = 0x55;
const byte CMD_UPPER_AND_LOWER_DISPENSE = 0x56;
const byte CMD_UPPER_TEST_DISPENSE = 0x76;
const byte CMD_LOWER_TEST_DISPENSE = 0x77;

const byte STATES_COUNT = 27;
const char state_string_0[] PROGMEM = "Good"; 
const char state_string_1[] PROGMEM = "Normal stop"; 
const char state_string_2[] PROGMEM = "Pickup error"; 
const char state_string_3[] PROGMEM = "JAM at CHK1,2 Sensor"; 
const char state_string_4[] PROGMEM = "Overflow bill "; 
const char state_string_5[] PROGMEM = "JAM at EXIT Sensor or EJT Sensor"; 
const char state_string_6[] PROGMEM = "JAM at DIV Sensor"; 
const char state_string_7[] PROGMEM = "Undefined command"; 
const char state_string_8[] PROGMEM = "Upper Bill- End"; 
const char state_string_9[] PROGMEM = "Counting Error(between CHK3,4 Sensor and DIV Sensor)"; 
const char state_string_10[] PROGMEM = "Note request error"; 
const char state_string_11[] PROGMEM = "Counting Error(between DIV Sensor and EJT Sensor)"; 
const char state_string_12[] PROGMEM = "Counting Error(between EJT Sensor and EXIT Sensor)"; 
const char state_string_13[] PROGMEM = "Reject Tray is not recognized"; 
const char state_string_14[] PROGMEM = "Lower Bill-End"; 
const char state_string_15[] PROGMEM = "Motor Stop"; 
const char state_string_16[] PROGMEM = "JAM at Div Sensor"; 
const char state_string_17[] PROGMEM = "Timeout (From DIV Sensor to EJT Sensor)"; 
const char state_string_18[] PROGMEM = "Over Reject"; 
const char state_string_19[] PROGMEM = "Upper Cassette is not recognized"; 
const char state_string_20[] PROGMEM = "Lower Cassette is not recognized"; 
const char state_string_21[] PROGMEM = "Dispensing timeout"; 
const char state_string_22[] PROGMEM = "JAM at EJT Sensor";
const char state_string_23[] PROGMEM = "Diverter solenoid or SOL Sensor error";
const char state_string_24[] PROGMEM = "SOL Sensor error";
const char state_string_25[] PROGMEM = "JAM at CHK3,4 Sensor";
const char state_string_26[] PROGMEM = "Purge error(Jam at Div Sensor)";
char state_string_buffer[40];

const char* const state_strings[STATES_COUNT] PROGMEM = {state_string_0, state_string_1, state_string_2, state_string_3, state_string_4, 
                                                        state_string_5, state_string_6, state_string_7, state_string_8, state_string_9,
                                                        state_string_10,state_string_11,state_string_12,state_string_13,state_string_14,
                                                        state_string_15,state_string_16,state_string_17,state_string_18,state_string_19,
                                                        state_string_20,state_string_21,state_string_22,state_string_23,state_string_24,
                                                        state_string_25,state_string_26};
byte states[STATES_COUNT] = {0x30,
                             0x31,
                             0x32,
                             0x33,
                             0x34,
                             0x35,
                             0x36,
                             0x37,
                             0x38,
                             0x3A,
                             0x3B,
                             0x3C,
                             0x3D,
                             0x3F,
                             0x40,
                             0x41,
                             0x42,
                             0x43,
                             0x44,
                             0x45,
                             0x46,
                             0x47,
                             0x48,
                             0x49,
                             0x4A,
                             0x4C,
                             0x4E};

cDevice_LCDM2::cDevice_LCDM2(cBaseLCDScreen* lcdscreen):
                                                      cBaseDevice(lcdscreen){
  waitState = WAIT_EOT;
  command = 0;
  dataLength = 0;
  currentLength = 0;
  dispense_part = DISPENSE_ALL;
  state_index = 0;
  needRefresh = false;
  scroll= 0;
  lastCMDSize = 0;
  resetStatus();
}

void cDevice_LCDM2::resetStatus(){
  statusString0[0] = '\0';
  statusString1[0] = '\0';
  statusString2[0] = '\0';
}

void cDevice_LCDM2::sendNCK(){
  waitState = WAIT_EOT;
  Serial.write(NCK);
  needRefresh = true;
}
void cDevice_LCDM2::sendACK(){
  Serial.write(ACK);
}
void cDevice_LCDM2::sendCommand(const byte* cmdSeq, int length){
  waitState=WAIT_ACK;
  byte xorV = 0;
  for (int i = 0; i<length; i++){
    Serial.write(cmdSeq[i]);
    xorV = xorV ^ cmdSeq[i];
  }
  Serial.write(xorV);
  lastCMDSize = length;
  if (lastCMDSize>COMMAND_MAX_SIZE)
    lastCMDSize = COMMAND_MAX_SIZE;
  memmove(lastCMD, cmdSeq, lastCMDSize);
}

void cDevice_LCDM2::addByteToBuffer(byte dataByte){
  if (currentLength<256){
    receivedData[currentLength] = dataByte;
    currentLength++;
  }
}

void byteToStr(byte* buf2, byte value){
  if (value>99){
    buf2[0] = '9';
    buf2[1] = '9';
    return;
  }

  if (value<10){
    buf2[0] = '0';
    buf2[1] = value + 48;
    return;
  }

  buf2[0] = (value / 10) + 48;
  buf2[1] = (value % 10) + 48;
}

void cDevice_LCDM2::processCommand(){
  byte state_code = states[state_index];
  needRefresh = true;

  switch(command){
    case CMD_PURGE:{
      static const byte data[6] = {SOH, ID, STX, 0x44, state_code ,ETX};
      sendCommand(data,6);

      resetStatus();
      snprintf(statusString0,STATUS_STRING_MAX_SIZE, "0x44 purge");
      snprintf(statusString1,STATUS_STRING_MAX_SIZE, "state: 0x%x", state_code);
    };break;
    case CMD_UPPER_DISPENSE:{
      int req_value = (receivedData[0]-48)*10 + (receivedData[1]-48);      
      int resp_value = req_value;
      switch(dispense_part){
        case DISPENSE_ALL: ;break;
        case DISPENSE_HALF: resp_value = resp_value / 2;break;
        case DISPENSE_NONE: resp_value = 0;break;
      }
      byte svalue[2];
      byteToStr(svalue, resp_value);      

      byte data[13] = {SOH, ID, STX, 0x45, svalue[0], svalue[1],svalue[0],svalue[1],  state_code,0x30, '0', '0', ETX};      
      sendCommand(data,13);

      resetStatus();
      snprintf(statusString0,STATUS_STRING_MAX_SIZE, "0x45 disp up");
      snprintf(statusString1,STATUS_STRING_MAX_SIZE, "state: 0x%x", state_code);
      snprintf(statusString1,STATUS_STRING_MAX_SIZE, "up: %u/%u", resp_value, req_value);
    };break;
    case CMD_STATUS:{
      static const byte data[9] = {SOH, ID, STX, 0x46, 0x30, state_code,0xff,0xff, ETX};
      sendCommand(data,9);

      resetStatus();
      snprintf(statusString0,STATUS_STRING_MAX_SIZE, "0x46 status");
      snprintf(statusString1,STATUS_STRING_MAX_SIZE, "state: 0x%x", state_code);
    };break;
    case CMD_ROM_VERSION:{
      static const byte data[13] = {SOH, ID, STX, 0x47, 'T', '0','1', 'T', '0','0','0','0', ETX};
      sendCommand(data,13);

      resetStatus();
      snprintf(statusString0,STATUS_STRING_MAX_SIZE, "0x47 rom version");
    };break;
    case CMD_LOWER_DISPENSE:{
      int req_value = (receivedData[0]-48)*10 + (receivedData[1]-48);      
      int resp_value = req_value;
      switch(dispense_part){
        case DISPENSE_ALL: ;break;
        case DISPENSE_HALF: resp_value = resp_value / 2;break;
        case DISPENSE_NONE: resp_value = 0;break;
      }
      byte svalue[2];
      byteToStr(svalue, resp_value);      

      byte data[13] = {SOH, ID, STX, 0x55, svalue[0], svalue[1],svalue[0],svalue[1],  state_code,0x30, '0', '0', ETX};      
      sendCommand(data,13);

      resetStatus();
      snprintf(statusString0,STATUS_STRING_MAX_SIZE, "0x55 disp lw");
      snprintf(statusString1,STATUS_STRING_MAX_SIZE, "state: 0x%x", state_code);
      snprintf(statusString1,STATUS_STRING_MAX_SIZE, "lw: %u/%u", resp_value, req_value);
    };break;
    case CMD_UPPER_AND_LOWER_DISPENSE:{
      int req_value1 = (receivedData[0]-48)*10 + (receivedData[1]-48);      
      int resp_value1 = req_value1;
      int req_value2 = (receivedData[2]-48)*10 + (receivedData[3]-48);      
      int resp_value2 = req_value2;

      switch(dispense_part){
        case DISPENSE_ALL: ;break;
        case DISPENSE_HALF: resp_value1 = resp_value1 / 2;
                            resp_value2 = resp_value2 / 2;
                            break;
        case DISPENSE_NONE: resp_value1 = 0;
                            resp_value2 = 0;
                            break;
      }
      byte svalue1[2];
      byteToStr(svalue1, resp_value1);      
      byte svalue2[2];
      byteToStr(svalue2, resp_value2);      

      byte data[20] = {SOH, ID, STX, 0x56, svalue1[0], svalue1[1],svalue1[0],svalue1[1], svalue2[0], svalue2[1],svalue2[0],svalue2[1], state_code,0x30,0x30, '0', '0','0', '0', ETX};      
      sendCommand(data,20);

      resetStatus();
      snprintf(statusString0,STATUS_STRING_MAX_SIZE, "0x56 disp up&lw");
      snprintf(statusString1,STATUS_STRING_MAX_SIZE, "state: 0x%x", state_code);
      snprintf(statusString1,STATUS_STRING_MAX_SIZE, "up: %u/%u, lw:%u/%u", resp_value1, req_value1, resp_value2, req_value2);
    };break;
    case CMD_UPPER_TEST_DISPENSE:{
      static const byte data[13] = {SOH, ID, STX, 0x76, '0', '1',0x30,0x30,  state_code,0x30, '0', '0', ETX};
      sendCommand(data,13);
      
      resetStatus();
      snprintf(statusString0,STATUS_STRING_MAX_SIZE, "0x76 up test");
      snprintf(statusString1,STATUS_STRING_MAX_SIZE, "state: 0x%x", state_code);
    };break;
    case CMD_LOWER_TEST_DISPENSE:{
      static const byte data[13] = {SOH, ID, STX, 0x77, '0', '1',0x30,0x30,  state_code,0x30, '0', '0', ETX};
      sendCommand(data,13);
      
      resetStatus();
      snprintf(statusString0,STATUS_STRING_MAX_SIZE, "0x77 up test");
      snprintf(statusString1,STATUS_STRING_MAX_SIZE, "state: 0x%x", state_code);
    };break;
  }
}

void cDevice_LCDM2::processReceivedByte(byte value){
  if (waitState==WAIT_ACK){
        if (value!=ACK && value!=NCK){
            snprintf(statusString0,STATUS_STRING_MAX_SIZE, "send nck resp");
            snprintf(statusString1,STATUS_STRING_MAX_SIZE, "reason");
            snprintf(statusString2,STATUS_STRING_MAX_SIZE, "ACK(0x06),NCK(0x15) but 0x%x", value);
            sendNCK();
            if (value==EOT)
                waitState=WAIT_ID;
            return;
        }
        else{
            if (value==ACK)
                waitState=WAIT_EOT;
            if (value==NCK)
                if (lastCMDSize>0)
                    sendCommand(lastCMD,lastCMDSize);
                else
                    waitState=WAIT_EOT;
            return;
        }
  }

  if (waitState==WAIT_EOT){
      if (value!=EOT){
          snprintf(statusString0,STATUS_STRING_MAX_SIZE, "send nck resp");
          snprintf(statusString1,STATUS_STRING_MAX_SIZE, "reason");
          snprintf(statusString2,STATUS_STRING_MAX_SIZE, "EOT(0x04) but 0x%x", value);
          sendNCK();
          return;
      }
      else{
          waitState=WAIT_ID;
          return;
      }
  }

  if (waitState==WAIT_ID){
      if (value!=ID){
          snprintf(statusString0,STATUS_STRING_MAX_SIZE, "send nck resp");
          snprintf(statusString1,STATUS_STRING_MAX_SIZE, "reason");
          snprintf(statusString2,STATUS_STRING_MAX_SIZE, "ID(0x50) but 0x%x", value);
          sendNCK();
          waitState=WAIT_EOT;
          return;
      }
      else{
          waitState=WAIT_STX;
          return;
      }
  }

  if (waitState==WAIT_STX){
      if (value!=STX){  
          snprintf(statusString0,STATUS_STRING_MAX_SIZE, "send nck resp");
          snprintf(statusString1,STATUS_STRING_MAX_SIZE, "reason");
          snprintf(statusString2,STATUS_STRING_MAX_SIZE, "STX(0x02) but 0x%x", value);
          sendNCK();        
          waitState=WAIT_EOT;
          return;
      }
      else{
          waitState=WAIT_CMD;
          return;
      }
  }

  if (waitState==WAIT_ETX){
      if (value!=ETX){
          snprintf(statusString0,STATUS_STRING_MAX_SIZE, "send nck resp");
          snprintf(statusString1,STATUS_STRING_MAX_SIZE, "reason");
          snprintf(statusString2,STATUS_STRING_MAX_SIZE, "ETX(0x03) but 0x%x", value);
          sendNCK();                  
          waitState=WAIT_EOT;
          return;
      }
      else{
          waitState=WAIT_BCC;
          return;
      }
  }

  if (waitState==WAIT_DATA){
      addByteToBuffer(value);
      if (currentLength>dataLength){
          snprintf(statusString0,STATUS_STRING_MAX_SIZE, "send nck resp");
          snprintf(statusString1,STATUS_STRING_MAX_SIZE, "reason");
          snprintf(statusString2,STATUS_STRING_MAX_SIZE, "incorrect data length");
          sendNCK();
          return;
      }
      if (currentLength==dataLength)
          waitState = WAIT_ETX;
      return;
  }

  if (waitState==WAIT_BCC){
      byte vxor = EOT ^ ID ^ STX ^ command ^ ETX;
      for (int i = 0; i<currentLength; i++)
        vxor = vxor ^ receivedData[i];
      if (vxor!=value){
          snprintf(statusString0,STATUS_STRING_MAX_SIZE, "send nck resp");
          snprintf(statusString1,STATUS_STRING_MAX_SIZE, "reason");
          snprintf(statusString2,STATUS_STRING_MAX_SIZE, "incorrect bcc");
          sendNCK();
          waitState=WAIT_EOT;
      }
      else{
          sendACK();
          processCommand();
          waitState=WAIT_EOT;
      };
  }

  if (waitState==WAIT_CMD){
      command = value;
      currentLength = 0;
      dataLength = 0;
      if (value==CMD_PURGE || value==CMD_UPPER_TEST_DISPENSE || value==CMD_LOWER_TEST_DISPENSE || value==CMD_STATUS || value==CMD_ROM_VERSION)
          waitState=WAIT_ETX;
      else if (value==CMD_UPPER_DISPENSE || value==CMD_LOWER_DISPENSE){
          waitState=WAIT_DATA;
          dataLength = 2;
      }
      else if (value==CMD_UPPER_AND_LOWER_DISPENSE){
          waitState=WAIT_DATA;
          dataLength = 4;
      }
      else{
          snprintf(statusString0,STATUS_STRING_MAX_SIZE, "send nck resp");
          snprintf(statusString1,STATUS_STRING_MAX_SIZE, "reason");
          snprintf(statusString2,STATUS_STRING_MAX_SIZE, "unknown cmd 0x%x",value);
          sendNCK();
          waitState=WAIT_EOT;
      };
  }  
}

void cDevice_LCDM2::update_state(int dt){
  if (Serial){
    while (Serial.available() >0){
      int dataByte = Serial.read();
      if (dataByte!=-1)
        processReceivedByte((byte)dataByte);
    }    
  }
}

void cDevice_LCDM2::update_info(){
  m_Screen->startDraw(9,scroll);
  m_Screen->println("LCDM2000 DISPENSER");

  m_Screen->print("dispense: ");
  switch(dispense_part){
    case DISPENSE_ALL:m_Screen->println("ALL");break;
    case DISPENSE_HALF:m_Screen->println("HALF");break;
    case DISPENSE_NONE:m_Screen->println("NONE");break;    
  }
  
  m_Screen->println("state:");
  
  m_Screen->print("0x");
  m_Screen->print(states[state_index],HEX);
  m_Screen->print(":");
  strcpy_P(state_string_buffer, (char*)pgm_read_word(&(state_strings[state_index])));    
  m_Screen->println(state_string_buffer);

  m_Screen->println(statusString0);
  
  m_Screen->println(statusString1);
  
  m_Screen->println(statusString2);
}
  
void cDevice_LCDM2::onPrev(){
  switch (dispense_part){
    case DISPENSE_ALL: dispense_part = DISPENSE_HALF; break;
    case DISPENSE_HALF: dispense_part = DISPENSE_NONE; break;
    case DISPENSE_NONE: dispense_part = DISPENSE_ALL; break;
  };
  needRefresh = true;
}

void cDevice_LCDM2::onNext(){
  state_index ++;
  if (state_index>=STATES_COUNT)
    state_index= 0;
  needRefresh = true;  
}
void cDevice_LCDM2::onOk(){
  scroll++;
  if (scroll>8)
    scroll = 0;
  needRefresh = true;
}
void cDevice_LCDM2::init(){
  waitState = WAIT_EOT;

  Serial.begin(9600);
  Serial.write("started");

  needRefresh = true;
}

void cDevice_LCDM2::update(int dt){
  update_state(dt);
  if (needRefresh)
    update_info();
  needRefresh = false;
}

#endif
