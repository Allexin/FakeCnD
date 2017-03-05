#ifndef BASE_DEVICE_H
#define BASE_DEVICE_H

#include "lcdwrapper.h"

class cBaseDevice{
protected:
  cBaseLCDScreen*   m_Screen;
public:
  cBaseDevice(cBaseLCDScreen* lcdscreen):
    m_Screen(lcdscreen){
  }
  
  virtual void onPrev() {}
  virtual void onNext() {}
  virtual void onOk() {}
  virtual void init() {}
  virtual void update(int dt) =0;
};

class cDeviceDisabled{
protected:
  cBaseLCDScreen*   m_Screen;
  int               m_MessageTime;
public:
  cDeviceDisabled(cBaseLCDScreen* lcdscreen):
    m_Screen(lcdscreen){
      m_MessageTime = 0;
  }
  
  virtual void onPrev() {}
  virtual void onNext() {}
  virtual void onOk() {}
  virtual void init() {
      m_Screen->startDraw(2,0);
        m_Screen->println("DEVICE NOT SELECTED");
        m_Screen->println("pls, edit config.h");
      m_Screen->finishDraw();
      Serial.begin(9600);
    }
  virtual void update(int dt){
    m_MessageTime += dt;
    if (m_MessageTime>=1000){
      Serial.write("DEVICE NOT SELECTED. pls, edit config.h\n");
      m_MessageTime=0;
    }
    }
};

#endif
