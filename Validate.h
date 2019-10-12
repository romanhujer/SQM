// Validate.h
//
// Copyright (c) 2019 Roman Hujer   http://hujer.net
//
//  Validate configuration

//#pragma once

#ifdef USE_OLED_ON
  #ifdef USE_U8x8_ON
    #define OLED_LIB_ON
  #endif
  #ifdef USE_U8G2_ON
    #ifdef OLED_LIB_ON
      #error "Select only one display lib!"
    #endif
    #define OLED_LIB_ON
  #endif  
  #ifdef USE_U8GLIB_ON
    #ifdef OLED_LIB_ON
      #error "Select only one displaylib!"
    #endif
    #define OLED_LIB_ON
  #endif
  #ifndef OLED_LIB_ON
    #error "Please select one display lib!"
  #endif
  #ifdef SH1106_ON  
    #define OLED_DISP_ON
  #endif 
  #ifdef SSD1306_ON
    #ifdef OLED_DISP_ON
      #error "Select only one display type"
    #endif
  #endif
  #ifndef OLED_DISP_ON
    #error "Please select one display type"
  #endif 
#endif
