// Config.h 
// User specific configuration for SQM
// 
// Copyright (c) 2019 Roman Hujer   http://hujer.net
//

#define DEBUG_OFF   // Debug
#define SERIAL_BAUD 115200    //Serial port baud Default is 115200

#define USE_WEATHER_SENSOR_ON
#define USE_SQM_SENSOR_ON
#define USE_OLED_ON

#ifdef USE_OLED_ON
// Select only one your display
  #define SH1106_ON   // SH1106  1.3"  128x64 OLED display is default
  #define SSD1306_OFF // SSD1306 0.96" 128x64 OLED display 
  
// Select onlyone display lib
  #define USE_U8x8_ON    // Use new u8x8lib is default for Arduino Nano
  #define USE_U8G2_OFF   // Use new u8g2lib not working on Arduino Nano project is big
  #define USE_U8GLIB_OFF // Use old u8glib
// Oled display auto kontrast deault ON 
  #define OLED_AUTO_CONTRAS_ON  
  #define DEFALUT_CONTRAS 0
#endif

#define USE_EEPROM_ON  // Use EEPROM for store Calibration vaule default ON

#define CALIBRATION_ON          // Calibration default ON 
#ifdef CALIBRATION_ON 
  #define SQM_CAL_OFFSET  0.00  // default SQM Calibration offset but priority is in EEPROM stored value
  #define TEMP_CAL_OFFSET 0.0   // default Temperature Calibration offset but priority is in EEPROM stored value
#endif

#define BUZZER_ON  // Buzzer

#define EXTENDET_PROTOCOL_ON  // My Extender protocol for weather info 
