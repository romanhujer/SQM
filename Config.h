// Config.h 
// User specific configuration for SQM
// 
// Copyright (c) 2019 Roman Hujer   http://hujer.net
//

#define DEBUG_OFF   // Debug

// Select only one your display
#define SSD1306_OFF // SSD1306 0.96" 128x64 OLED display 
#define SH1106_ON   // SH1106  1.3"  128x64 OLED display

#define OLED_AUTO_CONTRAS_ON  // OLed display auto kontrast

#define SQM_W_OFF   //SQM_W protocol 
#define SQM_LU_ON   //SQM_LU (SQM-LU-DL) protocol 

//
#define CALIBRATION_ON         // Calibration on 

#ifdef CALIBRATION_ON 
  #define SQM_CAL_OFFSET  0.00  // SQM Calibration offset
  #define TEMP_CAL_OFFSET 0.0  // Temperature Calibration offset
#endif

#define BUZZER_ON  // Buzzer


#define EXTENDET_PROTOCOL_ON  // My Extender protocol for weather info 

#define STATUS_LED_OFF         // Hardware Version 1.x no status LED
