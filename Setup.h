// Setup.h
//
// Copyright (c) 2019 Roman Hujer   http://hujer.net
//
//
// HW specific config

bool LED_ON = HIGH;
bool LED_OFF = LOW;

#define DisplayOnPin 5    // Display On Pin
#define LedPin 13         // Default LedPin on Adruino Nano

#ifdef BUZZER_ON
  #define BuzzerPin  4      // Buzzer Pin
  #define BuzzerTone 443    // Buzzer tone in Hz 
#endif

#ifdef BUZZER_ON    
  byte b_d_on = 0; 
  byte b_d_off = 0;
#endif
