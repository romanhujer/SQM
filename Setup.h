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

String SERIAL_NUMBER = Serial_No; 

#ifdef SSD1306_ON
#define OLED_ON
#include <U8glib.h>
#define OLED_ON
U8GLIB_SSD1306_128X64 OledDisp(U8G_I2C_OPT_NONE);
#endif

#ifdef SH1106_ON
#ifdef OLED_ON
#error "Select only one display!!!"
#endif
#define OLED_ON
#include <U8glib.h>
U8GLIB_SH1106_128X64 OledDisp(U8G_I2C_OPT_NONE);
#endif
#ifndef OLED_ON
 #error "Please select one Oled Display."
#endif
