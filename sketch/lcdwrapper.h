#ifndef LCDWRAPPER_H
#define LCDWRAPPER_H

#include "Arduino.h"
#include "config.h"

class cBaseLCDScreen: public Print{
public:
  virtual void init() = 0;
  virtual void startDraw(int linesCount, int scroll) = 0;
  virtual void finishDraw() = 0;
};

#if (LcdScreen==LCD_OFF)
class cLCDScreenDisabled: public cBaseLCDScreen{
public:
  virtual void init() override{}
  virtual size_t write(uint8_t str) override{}
  virtual void startDraw(int linesCount, int scroll) override{}
  virtual void finishDraw() override{}
};
#endif

#if (LcdScreen==LCD_I2C_SSD1306ASCII)
class cLCDScreenSSD1306ASCII: public cBaseLCDScreen{
protected:
  int                    m_AvailableLines;  
  int                    m_CurrentLine;
public:
  virtual void init() override;
  virtual size_t write(uint8_t str) override;
  virtual void startDraw(int linesCount, int scroll) override;
  virtual void finishDraw() override;
};
#endif

#if (LcdScreen==LCD_I2C_OLEDI2C)

const int MAX_STRING_LENGTH = 60;

class cLCDScreenOLEDI2C: public cBaseLCDScreen{
protected:
  int                    m_AvailableLines;  
  int                    m_CurrentLine;
  char                   m_Buffer[MAX_STRING_LENGTH+1];
  int                    m_BufferLength;
public:
  virtual void init() override;
  virtual size_t write(uint8_t str) override;
  virtual void startDraw(int linesCount, int scroll) override;
  virtual void finishDraw() override;
};
#endif

#endif
