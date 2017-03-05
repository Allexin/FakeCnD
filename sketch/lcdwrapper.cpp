#include "lcdwrapper.h"
#include "config.h"


#if (LcdScreen==LCD_I2C_SSD1306ASCII)
#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C
SSD1306AsciiWire oled;


void cLCDScreenSSD1306ASCII::init(){
  m_AvailableLines = 8;

  Wire.begin();         
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.set400kHz();  
  oled.setFont(Adafruit5x7);  
  oled.setScroll(false);
}

size_t cLCDScreenSSD1306ASCII::write(uint8_t str){
  if (m_CurrentLine>=0 && m_CurrentLine<m_AvailableLines)
    oled.write(str);
  if (str=='\n')
    m_CurrentLine++;
  return 1;  
}

void cLCDScreenSSD1306ASCII::startDraw(int linesCount, int scroll){
  if (linesCount>m_AvailableLines){
    if (scroll+m_AvailableLines<=linesCount)
      m_CurrentLine = -scroll;
    else
      m_CurrentLine = -(linesCount-m_AvailableLines);  
  }
  else  
    m_CurrentLine = 0;
  oled.clear();
}
void cLCDScreenSSD1306ASCII::finishDraw(){
  
}

#endif //SSD1306ASCII

#if (LcdScreen==LCD_I2C_OLEDI2C)
#include <OLED_I2C.h>

OLED  oled(SDA, SCL, 8);


extern uint8_t SmallFont[];
const int FONT_HEIGHT = 8;


void cLCDScreenOLEDI2C::init(){
  m_AvailableLines = 64 / FONT_HEIGHT;  

  oled.begin();
  oled.setFont(SmallFont);
}

size_t cLCDScreenOLEDI2C::write(uint8_t str){
  if (m_CurrentLine>=0 && m_CurrentLine<m_AvailableLines){
    if (m_BufferLength<MAX_STRING_LENGTH && str>=0x20){
      m_Buffer[m_BufferLength] = str;
      m_BufferLength++;
    }
  }
  if (str=='\n'){
    if (m_CurrentLine>=0 && m_BufferLength>0){
      m_Buffer[m_BufferLength] = '\0';
      oled.print(m_Buffer, LEFT, m_CurrentLine*FONT_HEIGHT);
      m_BufferLength = 0;      
    }
    m_CurrentLine++;
  }
  return 1;
}

void cLCDScreenOLEDI2C::startDraw(int linesCount, int scroll){
  if (linesCount>m_AvailableLines){
    if (scroll+m_AvailableLines<=linesCount)
      m_CurrentLine = -scroll;
    else
      m_CurrentLine = -(linesCount-m_AvailableLines);  
  }
  else  
    m_CurrentLine = 0;
  m_BufferLength = 0;  
  oled.clrScr();
  
}
void cLCDScreenOLEDI2C::finishDraw(){
  if (m_BufferLength>0){
    oled.print(m_Buffer, LEFT, m_CurrentLine*FONT_HEIGHT);
    m_BufferLength = 0;
  }
  oled.update();
}

#endif //OLED_I2C
