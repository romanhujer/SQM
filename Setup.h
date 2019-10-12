// Setup.h
//
// Copyright (c) 2019 Roman Hujer   http://hujer.net
//
//
// HW specific config

#define ModePin 5         // Mode Pin

#ifdef BUZZER_ON
  #define BuzzerPin  4      // Buzzer Pin
#endif

#ifdef USE_WEATHER_SENSOR_ON
// I2C mode for BME280 sensor
// 5V ------ CSB (enables the I2C interface)
// GND ----- SDO (I2C Address 0x76)
// 5V ------ SDO (I2C Address 0x77)
  #define BME_I2C_ADDRESS 0x76  //Default I2C for BME280 sensor 
#endif 


#ifdef USE_U8x8_ON
  #define OLED_FONT  u8x8_font_7x14B_1x2_f
#endif
#ifdef USE_U8G2_ON
  #define OLED_FONT  u8g2_font_ncenB08_tr
#endif
#ifdef USE_U8GLIB_ON 
  #define OLED_FONT  u8g_font_unifont
#endif
