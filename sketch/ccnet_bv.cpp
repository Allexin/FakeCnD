#include "config.h"
#if Device==DEVICE_CCNET_BILL_VALIDATOR
#include "ccnet_bv.h"

#include <avr/pgmspace.h>

const byte SYNC = 0x02;
const byte ADDR = 0x03; //BILL VALIDATOR

const byte MAX_BILLS_COUNT = 24;
struct sCountry
{
     char name[4];
     long bills[MAX_BILLS_COUNT];
     byte bills_count;
};

const byte COUNTRIES_COUNT = 3;
//    XXX    1    2    3    4     5    6    7    8    9    10   11   12   13   14   15   16  17   18   19   20   21   22   23   24
const sCountry COUNTRIES[COUNTRIES_COUNT] = {  
    {"USD", {1   ,2   ,5   ,10   ,20  ,50  ,100 ,0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   }, 7},
    {"EUR", {5   ,10  ,20  ,50   ,100 ,200 ,500 ,0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   }, 7},
    {"RUR", {10  ,50  ,100 ,200  ,500 ,1000,2000,5000,0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   }, 8}
  };

const byte STATES_COUNT = 22;
const char state_string_0[] PROGMEM = "Accepting"; 
const char state_string_1[] PROGMEM = "Rej due Insertion"; 
const char state_string_2[] PROGMEM = "Rej due Magnetic"; 
const char state_string_3[] PROGMEM = "Rej due Remained bill in head"; 
const char state_string_4[] PROGMEM = "Rej due Multiplying "; 
const char state_string_5[] PROGMEM = "Rej due Conveying"; 
const char state_string_6[] PROGMEM = "Rej due Identification"; 
const char state_string_7[] PROGMEM = "Rej due Verification "; 
const char state_string_8[] PROGMEM = "Rej due Optic "; 
const char state_string_9[] PROGMEM = "Rej due Inhibit "; 
const char state_string_10[] PROGMEM = "Rej due Capacity "; 
const char state_string_11[] PROGMEM = "Rej due Operation "; 
const char state_string_12[] PROGMEM = "Rej due Length "; 
const char state_string_13[] PROGMEM = "Rej due UV "; 
const char state_string_14[] PROGMEM = "Stack Motor Failure"; 
const char state_string_15[] PROGMEM = "Transport Motor Speed Failure "; 
const char state_string_16[] PROGMEM = "Transport Motor Failure "; 
const char state_string_17[] PROGMEM = "Aligning Motor Failure"; 
const char state_string_18[] PROGMEM = "Initial Cassette Status Failure"; 
const char state_string_19[] PROGMEM = "Optic Canal Failure "; 
const char state_string_20[] PROGMEM = "Magnetic Canal Failure "; 
const char state_string_21[] PROGMEM = "Capacitance Canal Failure ";
char state_string_buffer[40];

const char* const state_strings[STATES_COUNT] PROGMEM = {state_string_0, state_string_1, state_string_2, state_string_3, state_string_4, 
                                                        state_string_5, state_string_6, state_string_7, state_string_8, state_string_9,
                                                        state_string_10,state_string_11,state_string_12,state_string_13,state_string_14,
                                                        state_string_15,state_string_16,state_string_17,state_string_18,state_string_19,
                                                        state_string_20,state_string_21};

const sState STATES[STATES_COUNT] = {
            {0x15, 0x00},

            {0x1c, 0x60},
            {0x1c, 0x61},
            {0x1c, 0x62},
            {0x1c, 0x63},
            {0x1c, 0x64},
            {0x1c, 0x65},
            {0x1c, 0x66},
            {0x1c, 0x67},
            {0x1c, 0x68},
            {0x1c, 0x69},
            {0x1c, 0x6a},
            {0x1c, 0x6c},
            {0x1c, 0x6d},

            {0x47, 0x50},
            {0x47, 0x51},
            {0x47, 0x52},
            {0x47, 0x53},
            {0x47, 0x54},
            {0x47, 0x55},
            {0x47, 0x56},
            {0x47, 0x5f}
            };


#define POLYNOMIAL 0x08408
unsigned int GetCRC16(unsigned char* bufData, unsigned int sizeData)
{
 unsigned int CRC, i;
 unsigned char j;
 CRC = 0;
 for(i=0; i < sizeData; i++)
 {
 CRC ^= bufData[i];
 for(j=0; j < 8; j++)
 {
 if(CRC & 0x0001) {CRC >>= 1; CRC ^= POLYNOMIAL;}
 else CRC >>= 1;
 }
 }
 return CRC;
};

bool getBillState(const byte* billState, int bill){
  byte block;
  if (bill<8){
    block = 2;
  }
  else
  if (bill<16){
    block = 1;
    bill -= 8;
  }
  else{
    block = 0;
    bill -= 16;
  };
  return (billState[block] >> bill) & 1;
}

void sendResponse(const byte* data, byte lng){
  byte buf[255];
  byte fullLength = lng + 5;
  buf[0] = SYNC;
  buf[1] = ADDR;
  buf[2] = fullLength;
  if (lng>0)
    memcpy(&buf[3], data, lng);

  short int* crc16ptr = (short int*)&buf[3+lng];  
  *crc16ptr = (short int)GetCRC16(buf,fullLength-2);
  if (Serial){
    Serial.write(buf,fullLength);
  }
};

void sendACK(){
  static const byte data[] = {0x00};
  sendResponse(data, 1);   
};

void sendNAK(){
  static const byte data[] = {0xff};
  sendResponse(data, 1);  
};

void sendIllegalMessage(){
  static const byte data[] = {0x30};
  sendResponse(data, 1);  
};
            

cDevice_CCNET_BV::cDevice_CCNET_BV(cBaseLCDScreen* lcdscreen):
                                            cBaseDevice(lcdscreen){
  country_index = 0;
  bill_index = 0;
  state_index = 0;       
  menu_index = COUNTRY;
  needRefresh = false;  
  needPollToChangeState = true;
  deviceState = DS_POWER_UP;
  changeStateTime = 0; 
  waitChar = '|';
  bufferState = WAIT_SYNC;
  currentLength = 0;
  definedLength = 0;
}

void cDevice_CCNET_BV::update_info()
{
  m_Screen->startDraw(8,(int)menu_index);
  
  m_Screen->println("CCNET BILL VALIDATOR");
  if (menu_index == COUNTRY)
    m_Screen->print("> ");
  else
    m_Screen->print("  ");
  m_Screen->print("country:");  
  m_Screen->println(COUNTRIES[country_index].name);
  if (menu_index == BILL)
    m_Screen->print("> ");
  else
    m_Screen->print("  ");
  m_Screen->print("bill:");
  m_Screen->println(COUNTRIES[country_index].bills[bill_index]);
  if (menu_index == STATE)
    m_Screen->print("> ");
  else
    m_Screen->print("  ");
  m_Screen->print("state:0x");
  m_Screen->print(STATES[state_index].mainCode,HEX);
  if (STATES[state_index].subCode!=0){
    m_Screen->print(",0x");
    m_Screen->print(STATES[state_index].subCode,HEX);
  }
  strcpy_P(state_string_buffer, (char*)pgm_read_word(&(state_strings[state_index])));    
  m_Screen->println("");
  m_Screen->println(state_string_buffer);

  if (menu_index == PROCESS_BILL)
    m_Screen->print("> ");
  else
    m_Screen->print("  ");
  m_Screen->println("-[ INSERT BILL ]-");
  m_Screen->println("");

  if (waitChar!=0)
    m_Screen->print(waitChar);
  switch (deviceState){
    case DS_POWER_UP:{
      m_Screen->println(F("POWER UP"));
    };
    break;
    case DS_INITIALIZE:{
      m_Screen->println(F("INITIALIZE"));
    };
    break;
    case DS_IDLING:{
      m_Screen->println(F("IDLING"));
    };
    break;
    case DS_ACCEPTING:{
      m_Screen->println(F("ACCEPTING"));
    };
    break;
    case DS_STACKING:{
      m_Screen->println(F("STACKING"));
    };
    break;
    case DS_RETURNING:{
      m_Screen->println(F("RETURNING"));
    };
    break;
    case DS_UNIT_DISABLED:{
      m_Screen->println(F("DISABLED"));
    };
    break;
    case DS_HOLDING:{
      m_Screen->println(F("HOLDING"));
    };
    break;
    case DS_REJECTING:{
      m_Screen->println(F("REJECTING"));
    };
    break;
    case DS_FAILURE:{
      m_Screen->println(F("FAILURE"));
    };
    break;
    case DS_ESCROW_POSITION:{
      m_Screen->println(F("ESCROW POSITION"));
    };
    break;
    case DS_BILL_STACKED:{
      m_Screen->println(F("BILL STACKED"));
    };
    break;
    case DS_BILL_RETURNED:{
      m_Screen->println(F("BILL RETURNED"));
    };
    break;
  }
  m_Screen->finishDraw();
}

void cDevice_CCNET_BV::onCommandReceived(eCommand currentCommand, const byte* data, int dataLength){
  switch (currentCommand){
    case C_RESET:{ //ALL
      deviceState = DS_INITIALIZE;
      needPollToChangeState = false;
      changeStateTime = DS_INITIALIZE_CHANGE_TIME;
      //disable all bills when reset
      billEnables[0] = 0x00;
      billEnables[1] = 0x00;
      billEnables[2] = 0x00;

      //set security to high on all bills when reset
      billSecurity[0] = 0xff;
      billSecurity[1] = 0xff;
      billSecurity[2] = 0xff;

      //disable escrow on all bills when reset
      billEscrow[0] = 0x00;
      billEscrow[1] = 0x00;
      billEscrow[2] = 0x00;
    };
    break;
    case C_GET_STATUS:{ //ALL
      static const byte data[] = {billEnables[0],billEnables[1],billEnables[2],billSecurity[0],billSecurity[1],billSecurity[2]};
      sendResponse(data, 6);
    };
    break;
    C_SET_SECURITY:{ //ALL
      if (dataLength!=3){
        sendNAK();
      }
      else{
        billSecurity[0] = data[0];
        billSecurity[1] = data[1];
        billSecurity[2] = data[2];
        sendACK();
      }
    };
    break;
    case C_POLL:{ //ALL
      if (deviceState==DS_REJECTING || deviceState==DS_FAILURE){
        byte data[] = {STATES[state_index].mainCode,STATES[state_index].subCode};
        sendResponse(data,2);
      }
      else
      if (deviceState==DS_ESCROW_POSITION || deviceState==DS_BILL_STACKED || deviceState==DS_BILL_RETURNED){
        byte data[] = {deviceState, bill_index};
        sendResponse(data,2);
      }
      else{
        byte data[] = {deviceState};
        sendResponse(data,1);  
      }
      needPollToChangeState = false;
    };
    break;
    case C_ENABLE_BILL_TYPES:{ //ALL
      if (dataLength!=6){
        sendNAK();
      }
      else{
        billEnables[0] = data[0];
        billEnables[1] = data[1];
        billEnables[2] = data[2];

        billEscrow[0] = data[3];
        billEscrow[1] = data[4];
        billEscrow[2] = data[5];

        if (billEnables[0]==0x00 && billEnables[1]==0x00 && billEnables[2]==0x00){
          deviceState = DS_UNIT_DISABLED;
          needPollToChangeState = false;
          changeStateTime = 0;
        }
        else{
          if (deviceState == DS_UNIT_DISABLED){
            deviceState = DS_IDLING;
            needPollToChangeState = false;
            changeStateTime = 0;
          }
        }
        
        sendACK();
      }
    };
    break;
    case C_STACK:{ //ESCROW, HOLDING
      if (deviceState==DS_ESCROW_POSITION || deviceState==DS_HOLDING){
        deviceState = DS_STACKING;
        needPollToChangeState = false;
        changeStateTime = DS_STACKING_CHANGE_TIME;
        sendACK();
      }
      else{
        sendIllegalMessage();
      }
    };
    break;
    case C_RETURN:{ //ESCROW, HOLDING
      if (deviceState==DS_ESCROW_POSITION || deviceState==DS_HOLDING){
        deviceState = DS_RETURNING;
        needPollToChangeState = false;
        changeStateTime = DS_RETURNING_CHANGE_TIME;
        sendACK();
      }
      else{
        sendIllegalMessage();
      }
    };
    break;
    case C_IDENTIFICATION:{ //POWERUP, INITIALIZE, DISABLED, FAILURE
      if (deviceState==DS_POWER_UP || deviceState==DS_INITIALIZE || deviceState==DS_UNIT_DISABLED || deviceState==DS_FAILURE){
        static const byte data[] = {'A','R','D','U','I','N','O',' ','C','C','N','E','T','B','V',
                                    'F','A','K','E','-','0','0','0','0','0','0','1',
                                    1,0,0,0,0,0,1};
        sendResponse(data,34);
      }
      else{
        sendIllegalMessage();
      }
    };
    break;
    case C_HOLD:{ //ESCROW, HOLDING
      if (deviceState==DS_ESCROW_POSITION || deviceState==DS_HOLDING){
        deviceState = DS_HOLDING;
        needPollToChangeState = false;
        changeStateTime = DS_HOLDING_CHANGE_TIME;
        sendACK();
      }
      else{
        sendIllegalMessage();
      }
    };
    break;
    case C_GET_BILL_TABLE:{ //POWERUP, INITIALIZE, DISABLED, FAILURE
      if (deviceState==DS_POWER_UP || deviceState==DS_INITIALIZE || deviceState==DS_UNIT_DISABLED || deviceState==DS_FAILURE){
        byte data[120];
        for (int i = 0; i<MAX_BILLS_COUNT; i++){
          if (COUNTRIES[country_index].bills[i]==0){
            data[i*5+0] = 0;
            data[i*5+1] = 0;
            data[i*5+2] = 0;
            data[i*5+3] = 0;
            data[i*5+4] = 0;
          }
          else{
            int value = COUNTRIES[country_index].bills[i];
            byte zeros = 0;
            while (value % 10 == 0){
              zeros ++;
              value %=10;
            };

            data[i*5+0] = (byte)value;
            data[i*5+1] = COUNTRIES[country_index].name[0];
            data[i*5+2] = COUNTRIES[country_index].name[1];
            data[i*5+3] = COUNTRIES[country_index].name[2];
            data[i*5+4] = zeros;
          }
        }
        sendResponse(data,120);
      }
      else{
        sendIllegalMessage();
      }
    };
    break;
    case C_GET_CRC32_OF_THE_CODE:{ //POWERUP, INITIALIZE, DISABLED, FAILURE
      if (deviceState==DS_POWER_UP || deviceState==DS_INITIALIZE || deviceState==DS_UNIT_DISABLED || deviceState==DS_FAILURE){
        static const byte data[] = {1,0,0,1};
        sendResponse(data,4);
      }
      else{
        sendIllegalMessage();
      }
    };
    break;   
    default:{
      //unknown command
      sendIllegalMessage();
    }
  }
  needRefresh = true;
}

void cDevice_CCNET_BV::addByteToBuffer(byte dataByte){
  if (currentLength<256){
    receivedData[currentLength] = dataByte;
    currentLength++;
  }
}

void cDevice_CCNET_BV::processReceivedByte(byte dataByte){
  switch(bufferState){
    case WAIT_SYNC:{      
      if (dataByte==SYNC){ 
        currentLength = 0;
        addByteToBuffer(dataByte);
        bufferState = WAIT_ADDR;
      }
      else{
        //DO NOTHING - still wait sync
      }
    };break;
    case WAIT_ADDR:{      
      if (dataByte==ADDR){ 
        addByteToBuffer(dataByte);
        bufferState = WAIT_LNG;
      }
      else{
        bufferState = WAIT_SYNC;
      }
    }; break;
    case WAIT_LNG:{   
      definedLength = dataByte;   
      addByteToBuffer(dataByte);
      bufferState = WAIT_END;
    }; break;
    case WAIT_END:{      
      addByteToBuffer(dataByte);
      if (currentLength>=definedLength){
        bufferState = WAIT_SYNC;
        
        short int receivedCRC;
        memcpy(&receivedCRC, &receivedData[definedLength-2], 2);
        short int bufferCRC = GetCRC16(receivedData, definedLength-2);

        if (bufferCRC==receivedCRC){
          onCommandReceived((eCommand)receivedData[3],&receivedData[4],definedLength-6);
        }
        else{
          sendNAK();        
        }
      }
    }; break;
  }
}

void cDevice_CCNET_BV::update_state(int dt){
  if (Serial){
    while (Serial.available() >0){
      int dataByte = Serial.read();
      if (dataByte!=-1)
        processReceivedByte((byte)dataByte);
    }
    
  }
  else{
    deviceState = DS_POWER_UP;
    needPollToChangeState = true;
  }
    

  changeStateTime -= dt;
  if (needPollToChangeState)
    return;
  bool needChangeState = changeStateTime<=0;
  if (needChangeState)
    changeStateTime = 0;

  switch (deviceState) {
    case DS_POWER_UP:{
      if (needChangeState){
        deviceState = DS_INITIALIZE;
        needPollToChangeState = false;
        changeStateTime = DS_INITIALIZE_CHANGE_TIME;

        //disable all bills when reset
        billEnables[0] = 0x00;
        billEnables[1] = 0x00;
        billEnables[2] = 0x00;
  
        //set security to high on all bills when reset
        billSecurity[0] = 0xff;
        billSecurity[1] = 0xff;
        billSecurity[2] = 0xff;
  
        //disable escrow on all bills when reset
        billEscrow[0] = 0x00;
        billEscrow[1] = 0x00;
        billEscrow[2] = 0x00;
        
        needRefresh = true;
      }
    };
    break;
    case DS_INITIALIZE:{
      if (needChangeState){
        if (billEnables[0]==0x00 && billEnables[1]==0x00 && billEnables[2]==0x00){
          deviceState = DS_UNIT_DISABLED;
          needPollToChangeState = false;
          changeStateTime = 0;
        }
        else{
          deviceState = DS_IDLING;
          needPollToChangeState = false;
          changeStateTime = 0;
        }

        needRefresh = true;
      }
    };
    break;
    case DS_IDLING:{
      //DO NOTTHING
    };
    break;
    case DS_ACCEPTING:{
      if (needChangeState){
        if (getBillState(billEnables,bill_index)){//bill - enabled
          if (getBillState(billEscrow,bill_index)){
            deviceState = DS_ESCROW_POSITION;
            needPollToChangeState = false;
            changeStateTime = DS_ESCROW_POSITION_CHANGE_TIME;
          }
          else{
            deviceState = DS_STACKING;
            needPollToChangeState = false;
            changeStateTime = DS_STACKING_CHANGE_TIME;
          }
        }
        else{//bill disabled - returning
          deviceState = DS_RETURNING;
          needPollToChangeState = false;
          changeStateTime = DS_RETURNING_CHANGE_TIME;
        }

        needRefresh = true;
      }
    };
    break;
    case DS_STACKING:{
      if (needChangeState){
        deviceState = DS_BILL_STACKED;
        needPollToChangeState = true;
        changeStateTime = 0;
        needRefresh = true;
      }
    };
    break;
    case DS_RETURNING:{
      if (needChangeState){
        deviceState = DS_BILL_RETURNED;
        needPollToChangeState = true;
        changeStateTime = 0;
        needRefresh = true;
      }
    };
    break;
    case DS_UNIT_DISABLED:{
      //DO NOTTHING
    };
    break;
    case DS_HOLDING:{
      if (needChangeState){
        deviceState = DS_RETURNING;
        needPollToChangeState = false;
        changeStateTime = DS_RETURNING_CHANGE_TIME;
        needRefresh = true;
      }
    };
    break;
    case DS_REJECTING:{
      if (needChangeState){
        deviceState = DS_IDLING;
        needPollToChangeState = false;
        changeStateTime = 0;
        needRefresh = true;
      }
    };
    break;
    case DS_FAILURE:{
      //DO NOTHING
    };
    break;
    case DS_ESCROW_POSITION:{
      if (needChangeState){
        deviceState = DS_RETURNING;
        needPollToChangeState = false;
        changeStateTime = DS_RETURNING_CHANGE_TIME;
        needRefresh = true;
      }
    };
    break;
    case DS_BILL_STACKED:{
      if (needChangeState){
        deviceState = DS_IDLING;
        needPollToChangeState = false;
        changeStateTime = 0;
        needRefresh = true;
      }
    };
    break;
    case DS_BILL_RETURNED:{
      if (needChangeState){
        deviceState = DS_IDLING;
        needPollToChangeState = false;
        changeStateTime = 0;
        needRefresh = true;
      }
    };
    break;
  }
};

void cDevice_CCNET_BV::onPrev(){
  if (menu_index==0)
    menu_index = eMenu(COUNT-1);
  else
    menu_index = eMenu(menu_index-1);  
  needRefresh = true;
}
void cDevice_CCNET_BV::onNext(){
  menu_index = eMenu(menu_index+1);
  if (menu_index==COUNT)
    menu_index = eMenu(0);
  needRefresh = true;
}
void cDevice_CCNET_BV::onOk(){
  switch (menu_index){
    case COUNTRY:
      country_index++;
      if (country_index==COUNTRIES_COUNT)
        country_index = 0;
      bill_index = 0;
    break;
    case BILL:
      bill_index ++;
      if (bill_index==COUNTRIES[country_index].bills_count)
        bill_index = 0;
    break;
    case STATE:
      state_index ++;
      if (state_index==STATES_COUNT)
        state_index = 0;
    break;
    case PROCESS_BILL:
      if (STATES[state_index].mainCode==DS_ACCEPTING){
        deviceState = DS_ACCEPTING;
        needPollToChangeState = false;
        changeStateTime = DS_ACCEPTING_CHANGE_TIME;
      }
      else
      if (STATES[state_index].mainCode==DS_REJECTING){
        deviceState = DS_REJECTING;
        needPollToChangeState = false;
        changeStateTime = DS_REJECTING_CHANGE_TIME;
      }
      else
      if (STATES[state_index].mainCode==DS_FAILURE){
        deviceState = DS_FAILURE;
        needPollToChangeState = false;
        changeStateTime = 0;
      }
      else{
        //WTF??
      }
    break;
  }
  needRefresh = true;
}

void cDevice_CCNET_BV::init(){
  Serial.begin(9600);

  needRefresh = true;
}
void cDevice_CCNET_BV::update(int dt){
  update_state(dt);
  if (needRefresh)
    update_info();
  needRefresh = false;

  if (needPollToChangeState){
    if (millis() / 1000 != (millis() - dt) / 1000){
      needRefresh = true;
      switch (waitChar){
        case 0:
        case '|':waitChar = '/';break;
        case '/':waitChar = '-';break;
        case '-':waitChar = '\\';break;
        case '\\':waitChar = '|';break;
      }
    }
  }
  else{
    waitChar=0;
  }
}

#endif
