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
