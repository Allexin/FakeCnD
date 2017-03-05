#ifndef CCNET_BV_H
#define CCNET_BV_H

#include "config.h"
#include "base_device.h"
#include "lcdwrapper.h"

#if Device==DEVICE_CCNET_BILL_VALIDATOR

struct sState
{
     byte mainCode;
     byte subCode;
};

enum eMenu{
  COUNTRY = 0,
  BILL,
  STATE,
  PROCESS_BILL,
  COUNT
};

enum eDeviceState{
  DS_POWER_UP = 0x10,
  DS_INITIALIZE = 0x13,
  DS_IDLING = 0x14,
  DS_ACCEPTING =0x15,
  DS_STACKING =0x17,
  DS_RETURNING =0x18,
  DS_UNIT_DISABLED =0x19,
  DS_HOLDING =0x1A,
  DS_REJECTING =0x1C,
  DS_FAILURE =0x47,
  DS_ESCROW_POSITION =0x80,
  DS_BILL_STACKED =0x81,
  DS_BILL_RETURNED =0x82
};

const int DS_INITIALIZE_CHANGE_TIME = 2000;
const int DS_ACCEPTING_CHANGE_TIME = 800;
const int DS_REJECTING_CHANGE_TIME = 1500;
const int DS_STACKING_CHANGE_TIME = 2500;
const int DS_RETURNING_CHANGE_TIME = 1500;
const int DS_HOLDING_CHANGE_TIME = 10000;
const int DS_ESCROW_POSITION_CHANGE_TIME = 10000;

enum eCommand{
  C_RESET = 0x30,
  C_GET_STATUS = 0x31,
  C_SET_SECURITY = 0x32,
  C_POLL = 0x33,
  C_ENABLE_BILL_TYPES = 0x34,
  C_STACK = 0x35,
  C_RETURN = 0x36,
  C_IDENTIFICATION = 0x37,
  C_HOLD = 0x38,
  C_GET_BILL_TABLE = 0x41,
  C_GET_CRC32_OF_THE_CODE = 0x51
};

enum eReadState{
  WAIT_SYNC,
  WAIT_ADDR,
  WAIT_LNG,
  WAIT_END
};

class cDevice_CCNET_BV: public cBaseDevice{
protected:
  byte country_index;
  byte bill_index;
  byte state_index;       
  eMenu menu_index;
  bool needRefresh;  

  bool needPollToChangeState;
  eDeviceState deviceState;
  int changeStateTime; 
  byte billEnables[3];
  byte billSecurity[3];
  byte billEscrow[3];

  char waitChar;

  eReadState bufferState;
  byte receivedData[256];
  byte currentLength;
  byte definedLength;

  void addByteToBuffer(byte dataByte);
  void processReceivedByte(byte dataByte);
  void update_state(int dt);  
  void update_info();
  void onCommandReceived(eCommand currentCommand, const byte* data, int dataLength);
public:  
  cDevice_CCNET_BV(cBaseLCDScreen* lcdscreen);

  virtual void onPrev() override;
  virtual void onNext() override;
  virtual void onOk() override;
  virtual void init() override;
  virtual void update(int dt) override;
};

#endif

#endif //CCNET_BV_H
