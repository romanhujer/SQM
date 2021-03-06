// Setup.h
//
// Copyright (c) 2019 Roman Hujer   http://hujer.net
//
//
// HW specific config

bool LED_ON = HIGH;
bool LED_OFF = LOW;

#define ModePin 5         // Mode Pin

#ifdef STATUS_LED_ON
   #define LedPin 13        // Status Led Pin
#endif

#ifdef BUZZER_ON
  #define BuzzerPin  4      // Buzzer Pin
  #define BuzzerTone 443    // Buzzer tone in Hz 
#endif


// Set Unihedron protokol

//SQM-W

#ifdef SQM_W_ON
  String PROTOCOL_NUMBER = "00000004";
  String MODEL_NUMBER    = "00000006";
  String FEATURE_NUMBER  = "00000023";
  #define SQM_ON
#endif

// SQM-LU-V SQM-LU-DL-V
#ifdef SQM_LU_ON
  #ifdef SQM_ON
    #error "Select only one SQM protokol!!!"
  #endif
  String PROTOCOL_NUMBER = "00000002";
  String MODEL_NUMBER    = "00000003";
  String FEATURE_NUMBER  = "00000001";
  #define SQM_ON
#endif
#ifndef SQM_ON
  #error "Please select one SQM protokol."
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
