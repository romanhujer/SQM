/*
   SQM.ino   Sky Quality Meter

   Copyright (c) 2018 Roman Hujer   http://hujer.net

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,ss
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

  Description:
  Sky Quality Meter using the TSL2591
   
  base on https://github.com/gshau/SQM_TSL2591/
 
  and BME280 weather sensor 
  
  and 128x64 OLED I2C display 0.96" (SSD1306) or 1.3" (SH1106)

  Wiring diagram a PCB  on   https://easyeda.com/hujer.roman/sqm-hr

*/
#define Version "SQM_1.0"

#include "Config.h"

#include "Setup.h"

#include <SPI.h>
#include <Wire.h>

#ifdef SSD1306_ON
  #define OLED_ON
  #include <U8glib.h>
  #define OLED_ON
  U8GLIB_SSD1306_128X64 OledDisp(U8G_I2C_OPT_NONE);
#endif

#ifdef SH1106_ON 
  #ifdef OLED_ON
     #error "Select olny one display!!!"
  #endif
  #define OLED_ON
  #include <U8glib.h>
  U8GLIB_SH1106_128X64 OledDisp(U8G_I2C_OPT_NONE);
#endif


// setup for TSL2591
#include <Wire.h>
// #include <Adafruit_Sensor.h>
#include "SQM_TSL2591.h"

// Temperatue sensor
#include <BME280I2C.h>
BME280I2C bme;

boolean DisplayON = true;

SQM_TSL2591 sqm = SQM_TSL2591(2591);
void readSQM(void);

void setup() {

pinMode(DisplayOnPin, INPUT_PULLUP);
pinMode(LedPin, OUTPUT);


#ifdef BUZZER_ON
  pinMode(BuzzerPin, OUTPUT);
#endif
  
  Serial.begin(9600);
  Serial.println("Start");
  

  if (sqm.begin()) {

    Serial.println("Found SQM (TSL) sensor");
    sqm.config.gain = TSL2591_GAIN_LOW;
    sqm.config.time = TSL2591_INTEGRATIONTIME_200MS;
    sqm.configSensor();
 //   sqm.showConfig();
    sqm.setCalibrationOffset(CALIBRATION_OFFSET);
 
  } else{
    Serial.println("SQM sensor not found");
 }

  // OledDisp.setRot180();
  
   OledDisp.firstPage();
   do {
    
         OledDisp.setContrast(0);   
         OledDisp.setFont(u8g_font_unifont);
         OledDisp.setPrintPos(10, 30);
         OledDisp.print("  SQM Ready");
//         if (digitalRead(DisplayOnPin) == 0) {
//           OledDisp.setPrintPos(10, 45);
//           OledDisp.print(" Display OFF");
//        } 
      
    
  } while ( OledDisp.nextPage() );
#ifdef BUZZER_ON  
  buzzer(500);
  delay(500); // Pause for 2 seconds
#else 
  delay(1000); // Pause for 2 seconds
#endif
digitalWrite(LedPin, LED_OFF);

} // end of Setup

void loop() {
  float temp = 0;
  float hum = 0;
  float pres = 0;
  String SensorID;

  

       if (digitalRead(DisplayOnPin))   {
        if (DisplayON){ 
            DisplayON = false; 
          } else { 
            DisplayON = true;
          }
#ifdef BUZZER_ON  
            buzzer(200);
#endif                         

       }  
 

  if ( bme.begin()) {
    switch (bme.chipModel())
    {
      case BME280::ChipModel_BME280:
        SensorID = "BME280";
//        Serial.println("Found BME280 sensor! Success.");
        break;
      case BME280::ChipModel_BMP280:
        SensorID = "BMP280";
//        Serial.println("Found BMP280 sensor! No Humidity available.");
        break;
      default:
        Serial.println("Found UNKNOWN sensor! Error!");
    }
    BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
    BME280::PresUnit presUnit(BME280::PresUnit_Pa);
    Serial.print("Get sensord data.");
  
    bme.read(pres, temp, hum, tempUnit, presUnit);
    
  } 
   digitalWrite(LedPin, LED_OFF);

#ifdef TEMPER_CALIB_ON
   sqm.setTemperature( temp );
#endif   
   sqm.takeReading();


// write to serial monitor for debugging purposes

//  Serial.print("full:   "); Serial.println(sqm.full);
//  Serial.print("ir:     "); Serial.println(sqm.ir);
//  Serial.print("vis:    "); Serial.println(sqm.vis);
  Serial.print("Mag/Arc-Sec :  "); Serial.print(sqm.mpsas);
  Serial.print(" ± "); Serial.println(sqm.dmpsas);
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.println("°C");
  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.println("%");
  Serial.print("Pressure: ");
  Serial.print(pres/100);
  Serial.println(" hPa");
  Serial.println("======================================");




    OledDisp.firstPage();
    do {
        if (! DisplayON ) {
           OledDisp.sleepOn();
           
        } else   {
           OledDisp.sleepOff();
         
        }  
     OledDisp.setContrast(0);   
     OledDisp.setFont(u8g_font_unifont);
     OledDisp.setPrintPos(1, 10);
     OledDisp.print("Mag/Arc-Sec T");   
     OledDisp.setPrintPos(1, 30);
     OledDisp.print(sqm.mpsas);
     OledDisp.print(" ");
     OledDisp.print(char(0xb1));
     OledDisp.print(sqm.dmpsas);
     OledDisp.print(" ");
     OledDisp.print(int(temp));
     OledDisp.print(char(0xb0));
     OledDisp.print("C");
     OledDisp.setPrintPos(1, 50);
     OledDisp.print("H:");
     OledDisp.print(int(hum));
     OledDisp.print("% P:");
     OledDisp.print(int(pres/100));
     OledDisp.print("hPa");
    } while( OledDisp.nextPage()); 

    
 delay(2000);
 // digitalWrite(LedPin, LED_ON);
 }
