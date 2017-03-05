//READ ME
//DO NOT FORGET TO EDIT config.h
//At least - uncomment device which you want to use!

#include "config.h"
#include "lcdwrapper.h"
#if LcdScreen==LCD_I2C_SSD1306ASCII
  cLCDScreenSSD1306ASCII screen;
#elif LcdScreen==LCD_I2C_OLEDI2C
  cLCDScreenOLEDI2C screen;
#elif LcdScreen==LCD_OFF
  cLCDScreenDisabled screen;
#endif

#if Device==DEVICE_CCNET_BILL_VALIDATOR
  #include "ccnet_bv.h"
  cDevice_CCNET_BV device(&screen);
#elif Device==DEVICE_LCDM2_DISPENSER
  #include "lcdm2.h"
  cDevice_LCDM2 device(&screen);  
#elif Device==DEVICE_OFF
  #include "base_device.h"
  cDeviceDisabled device(&screen);
#endif


void setup() {
  if (BUTTON_PREV>=0)
    pinMode(BUTTON_PREV, INPUT);
  if (BUTTON_NEXT>=0)  
    pinMode(BUTTON_NEXT, INPUT);
  if (BUTTON_OK>=0)  
    pinMode(BUTTON_OK,   INPUT);
  
  
  screen.init();
  device.init();
}

bool prevState = false;
bool nextState = false;
bool okState = false;
long lastButtonUpdate = 0;
long lastUpdate = 0;

void loop() {  
  int dt = millis() - lastUpdate;
  lastUpdate += dt;
  long ctime = millis();
  bool currentPrevState = (BUTTON_PREV>=0) && (digitalRead(BUTTON_PREV)==HIGH);
  bool currentNextState = (BUTTON_NEXT>=0) && (digitalRead(BUTTON_NEXT)==HIGH);
  bool currentOkState = (BUTTON_OK>=0) && (digitalRead(BUTTON_OK)==HIGH);
  if (ctime-lastButtonUpdate>DEBOUNCE_TIME){
    if (currentPrevState && !prevState){
      lastButtonUpdate = ctime;
      device.onPrev();    
    }
    if (currentNextState && !nextState){
      lastButtonUpdate = ctime;
      device.onNext();    
    }
    if (currentOkState && !okState){
      lastButtonUpdate = ctime;
      device.onOk();    
    }
  };
  prevState = currentPrevState;
  nextState = currentNextState;
  okState = currentOkState;

  device.update(dt);  
}

