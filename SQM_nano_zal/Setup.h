// Setup.h
//
// Copyright (c) 2019 Roman Hujer   http://hujer.net
//
//
// HW specific config
#ifndef SETUP_H
#define SETUP_H

#define ModePin 5         // Mode Pin  
#define BuzzerPin  4      // Buzzer Pin

// I2C mode for BME280 sensor
// 5V ------ CSB (enables the I2C interface)
// GND ----- SDO (I2C Address 0x77)
// 5V ------ SDO (I2C Address 0x76)
#define BME_I2C_ADDRESS 0x76  //Default I2C for BME280 sensor 

#define OLED_FONT  u8x8_font_7x14B_1x2_f

#endif
