 #ifndef CONFIG_H
 #define CONFIG_H

 //This config must be edited by user before upload sketch.


//SECTION INFO - do not edit this section. This is only list of all available devices and screens
//DEVICES
#define DEVICE_OFF                    0
#define DEVICE_CCNET_BILL_VALIDATOR   1
#define DEVICE_LCDM2_DISPENSER        2


//SCREENS
#define LCD_OFF               0
#define LCD_I2C_SSD1306ASCII  1
#define LCD_I2C_OLEDI2C       2



//SECTION DEVICES - uncomment only one string in each block

//UNCOMMENT ONE DEVICE EMULATOR
//#define Device DEVICE_CCNET_BILL_VALIDATOR //Bill validator based on CCNET V2.4 protocol
#define Device DEVICE_LCDM2_DISPENSER //Puloon LCDM 2000 dispenser

//UNCOMMENT ONE LCD SCREEN. Stay all commented if you do not want use screen
#define LcdScreen LCD_I2C_SSD1306ASCII //please install library SSD1306Ascii. Most lightweight variant! But with flickering :(
//#define LcdScreen LCD_I2C_OLEDI2C //please install libary OLED_I2C. Cool, but not lightweight. DO NOT USE ON ARDUINO NANO or other devices with 2k RAM


//SECTION BUTTONS - set correct pins for buttons
//PINS. set to -1 if you do not want use button
const int BUTTON_PREV = 15;
const int BUTTON_NEXT = 16;
const int BUTTON_OK   = 17;
//SETTINGS
const int  DEBOUNCE_TIME = 200; //millis



  #ifndef LcdScreen
    #define LcdScreen LCD_OFF
  #endif
  #ifndef Device
    #define Device DEVICE_OFF
  #endif
#endif

