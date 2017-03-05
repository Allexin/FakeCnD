#ifndef LCDM2_H
#define LCDM2_H

#include "config.h"
#include "base_device.h"
#include "lcdwrapper.h"

#if Device==DEVICE_LCDM2_DISPENSER

enum eWaitState{
  WAIT_EOT  = 1,
  WAIT_ID,
  WAIT_STX,
  WAIT_CMD,
  WAIT_DATA,
  WAIT_ETX,
  WAIT_BCC,
  WAIT_ACK
};

enum eDispenseSettings{
  DISPENSE_ALL,
  DISPENSE_HALF,
  DISPENSE_NONE
};

const int STATUS_STRING_MAX_SIZE =60;
const int COMMAND_MAX_SIZE =20;

class cDevice_LCDM2: public cBaseDevice{
protected:
  eWaitState waitState;
  byte command;
  byte dataLength;
  byte currentLength;
  char receivedData[256];
  eDispenseSettings dispense_part;
  int state_index;
  bool needRefresh;
  int scroll;
  byte lastCMD[COMMAND_MAX_SIZE];
  byte lastCMDSize;

  char statusString0[STATUS_STRING_MAX_SIZE];
  char statusString1[STATUS_STRING_MAX_SIZE];
  char statusString2[STATUS_STRING_MAX_SIZE];
  void resetStatus();
  void update_state(int dt);  
  void update_info();

  void processCommand();
  void processReceivedByte(byte value);

  void addByteToBuffer(byte dataByte);

  void sendNCK();
  void sendACK();
  void sendCommand(const byte* cmdSeq, int length);
public:
  cDevice_LCDM2(cBaseLCDScreen* lcdscreen);
  
  virtual void onPrev() override;
  virtual void onNext() override;
  virtual void onOk() override;
  virtual void init() override;
  virtual void update(int dt) override;
};

#endif

#endif //LCDM2_H

