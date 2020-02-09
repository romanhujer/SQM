// Validate.h
//
// Copyright (c) 2019 Roman Hujer   http://hujer.net
//
//  Validate configuration

#pragma once

#ifdef SH1106_ON  
    #define OLED_DISP_ON
#endif 
#ifdef SSD1306_ON
    #ifdef OLED_DISP_ON
      #error "Select only one display type"
    #endif
    #define OLED_DISP_ON
#endif
#ifndef OLED_DISP_ON
    #error "Please select one display type"
#endif 
