/*
 *
   SQM.ino

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
   
  SQM using TSL2591 sensor as a Sky Quality Meter
  and 128x64 OLED as display

  Requires Arduino MEGA or MEGA 2560 because of size of code 
  plus buffer for display
  

  This code is in the public domain and is based on 
  Gabe Shaughnessy's example from Oct 21 2017

  RD Beck
  Jan 2019
 */

// setup for OLED1
#include <SPI.h>
#include <Wire.h>

#include <U8glib.h>
//U8GLIB_SSD1306_128X64 OledDisp(U8G_I2C_OPT_NONE);
U8GLIB_SH1106_128X64 OledDisp(U8G_I2C_OPT_NONE);


// setup for TSL2591
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "SQM_TSL2591.h"

// Temperatue sensosr
#include <BME280I2C.h>
BME280I2C bme;


SQM_TSL2591 sqm = SQM_TSL2591(2591);
void readSQM(void);

void setup() {
  Serial.begin(9600);
  Serial.println("Start");
  
  // pinMode(13, OUTPUT);
  if (sqm.begin()) {

    Serial.println("Found SQM (TSL) sensor");
    sqm.config.gain = TSL2591_GAIN_LOW;
    sqm.config.time = TSL2591_INTEGRATIONTIME_200MS;
    sqm.configSensor();
    sqm.showConfig();
    sqm.setCalibrationOffset(0.0);
  } else{
    Serial.println("SQM sensor not found");
 }


// add a logo or other text for initialization

  // OledDisp.setRot180();
   OledDisp.firstPage();
   do {
      OledDisp.setFont(u8g_font_unifont);
      OledDisp.setPrintPos(10, 30);
      OledDisp.print("  SQM Ready");
  } while ( OledDisp.nextPage() );
  delay(1000); // Pause for 2 seconds

}

void loop() {
  float temp = 0;
  float hum = 0;
  float pres = 0;
  String SensorID;

  sqm.takeReading();

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
/*
// write to OLED
 */
    OledDisp.firstPage();
    do {
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
}
