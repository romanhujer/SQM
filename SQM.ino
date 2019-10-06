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
#define Version "1.0.1"

#include "Config.h"



#include <SPI.h>
#include <Wire.h>
// #include <avr/wdt.h>

#include "Setup.h"


// setup for SQM TSL2591
#include "SQM_TSL2591.h"

// Temperatue sensor
#include <BME280I2C.h>
BME280I2C bme;

boolean USBmodeON = false;
boolean InitError = false;
boolean SerialOK  = false;
boolean Blik      = false;

String SensorID;
String BME_MSG;
String TSL_MSG;


SQM_TSL2591 sqm = SQM_TSL2591(2591);
void readSQM(void);

void setup() {

  pinMode(ModePin, INPUT_PULLUP);

#ifdef  STATUS_LED_ON
  pinMode(LedPin, OUTPUT);
#endif

#ifdef BUZZER_ON
  pinMode(BuzzerPin, OUTPUT);
#endif

#ifdef DEBUG_ON
  Serial.begin(9600);
  Serial.println("Start");
#else
  Serial.begin(115200);
  //  Serial.setTimeout(1000);
#endif


  if ( bme.begin()) {
    switch (bme.chipModel())
    {
      case BME280::ChipModel_BME280:
        SensorID = "BME280";
        BME_MSG  = "BME280   Success";
        break;
      case BME280::ChipModel_BMP280:
        SensorID = "BMP280";
        BME_MSG = "BMP280 No Humidity";
        break;
      default:
        SensorID = "UNKNOWN";
        BME_MSG = "UNKNOWN sensor!";
        InitError = true;
    }
  }


  if (sqm.begin()) {
    TSL_MSG = "TSL2591  Success";
    //    TSL_MSG = "SQM(TSL) Success";
#ifdef DEBUG_ON
    sqm.verbose = true;
#else
    sqm.verbose = false;
#endif
    sqm.config.gain = TSL2591_GAIN_LOW;
    sqm.config.time = TSL2591_INTEGRATIONTIME_200MS;
    sqm.configSensor();
#ifdef DEBUG_ON
    sqm.showConfig();
#endif
    sqm.setCalibrationOffset(CALIBRATION_OFFSET);
  }
  else {
    TSL_MSG = "SQM not found";
    InitError = true;
  }
  // OledDisp.setRot180();
  OledDisp.firstPage();
  do {
    OledDisp.setContrast(0);
    OledDisp.setFont(u8g_font_unifont);
    OledDisp.setPrintPos(1, 15);
    OledDisp.print("SQM Ready V");
    OledDisp.print(Version);
    OledDisp.setPrintPos(1, 37);
    OledDisp.print(TSL_MSG);
    OledDisp.setPrintPos(1, 60);
    OledDisp.print(BME_MSG);
  } while ( OledDisp.nextPage() );

  if (!InitError) {

#ifdef BUZZER_ON
    buzzer(500);
#endif
    delay(1000); // Pause for 1 seconds

  } else {
    for (int _i = 0; _i < 10; _i++) {
      buzzer(50);
      delay(50) ;
    }
    while (true);
  }


} // end of Setup

void loop() {
  float temp = 0;
  float hum = 0;
  float pres = 0;

  String temp_string;
  String response = "";
  bool new_data = false;

  /// wdt_reset();

  if (digitalRead(ModePin) == 0)   {
    SerialOK  = false;
#ifdef BUZZER_ON
    if (USBmodeON) {
      buzzer(200);
    }
#endif
    USBmodeON = false;

  }
  else {
    
      #ifdef BUZZER_ON
        if (!USBmodeON) {
          buzzer(200);
        }
      #endif
    
    USBmodeON = true;
    //        OledDisp.sleepOn();
  }

  if ( !USBmodeON ) {
    BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
    BME280::PresUnit presUnit(BME280::PresUnit_Pa);

    bme.read(pres, temp, hum, tempUnit, presUnit);
#ifdef TEMPER_CALIB_ON
    sqm.setTemperature( temp );
#endif
    sqm.takeReading();

    OledDisp.firstPage();
    do {

      OledDisp.setContrast(0);
      OledDisp.setFont(u8g_font_unifont);
      OledDisp.setPrintPos(1, 15);
      OledDisp.print("Mag/Arc-Sec T");
      if (Blik) {
        OledDisp.print(" #");
      }
      OledDisp.setPrintPos(1, 37);
      OledDisp.print(sqm.mpsas);
      OledDisp.print(" ");
      OledDisp.print(char(0xb1));
      OledDisp.print(sqm.dmpsas);
      OledDisp.print(" ");
      OledDisp.print(int(temp));
      OledDisp.print(char(0xb0));
      OledDisp.print("C");
      OledDisp.setPrintPos(1, 60);
      OledDisp.print("H:");
      OledDisp.print(int(hum));
      OledDisp.print("% P:");
      OledDisp.print(int(pres / 100));
      OledDisp.print("hPa");
    } while ( OledDisp.nextPage());
    if (Blik) {
      Blik = false;
    } else  {
      Blik = true;
    }

    delay(2000);

  } else {

    if (!SerialOK) {
      OledDisp.firstPage();
      do {

        OledDisp.setContrast(0);
        OledDisp.setFont(u8g_font_unifont);
        OledDisp.setPrintPos(1, 37);
        OledDisp.print("Wait USB data");
      } while ( OledDisp.nextPage());
    }
    while (Serial.available() > 0) {
      SerialOK  = true;
      if (digitalRead(ModePin) == 0)  break ;  // check end USB mode

      BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
      BME280::PresUnit presUnit(BME280::PresUnit_Pa);
      bme.read(pres, temp, hum, tempUnit, presUnit);

#ifdef TEMPER_CALIB_ON
      sqm.setTemperature( temp );
#endif
      sqm.takeReading();
      String command = Serial.readStringUntil('x');

      if ( command.equals("i")) {  // Unit information request (note lower case "i")

      response = String(response + "i," + PROTOCOL_NUMBER + "," + MODEL_NUMBER + "," + FEATURE_NUMBER + "," + SERIAL_NUMBER);
        Serial.println(response);

      } else if ( command.equals("r")) { // Reading request

      if (sqm.mpsas < 10.) {
          response = "r,  ";
        } else if (sqm.mpsas < 0) {
          response = "r,";
        } else {
          response = "r, ";
        }
        if (temp < 0.) {
          temp_string = ",  ";
        } else if (temp < 10.) {
          temp_string = ",   ";
        } else {
          temp_string = ",  ";
        }
        temp_string = String(temp_string + String(temp, 1) + "C");
        response = String(response + String(sqm.mpsas, 2) + "m,0000005915Hz,0000000000c,0000000.000s" + temp_string);
        Serial.println(response);

      } else if (command.equals("u")) { // Unaveraged readin request
      if (sqm.mpsas < 10.) {
          response = "u,  ";
        } else if (sqm.mpsas < 0) {
          response = "u,";
        } else {
          response = "u, ";
        }
        if (temp < 0.) {
          temp_string = ",  ";
        } else if (temp < 10.) {
          temp_string = ",   ";
        } else {
          temp_string = ",  ";
        }
        temp_string = String(temp_string + String(temp, 1) + "C");
        response = String(response + String(sqm.mpsas, 2) + "m,0000005915Hz,0000000000c,0000000.000s" + temp_string);
        Serial.println(response);
/* Not now  redy to use 
      } else if (command.equals("c")) { // Calibration information request
        response = "c,00000017.60m,0000000.000s, 039.4C,00000008.71m, 039.4C";
        Serial.println(response);
*/
#ifdef EXTENDET_PROTOCOL_ON
      } else if (command.equals("X") { // My eXtension request for weather information
        
        response = "X,";
        response = String(response) + String(sqm.mpsas, 2) + "m," + String(int(temp)) + "C," + String(int(hum)) + "%," + String(int(pres / 100)) + "hPa";
        Serial.println(response);
#endif
      } else if (command.equals("L5")) {
        Serial.println("L5,370");
      }
      OledDisp.firstPage();
      do {

        OledDisp.setContrast(0);
        OledDisp.setFont(u8g_font_unifont);
        OledDisp.setPrintPos(1, 15);
        //     OledDisp.print("SQM in USB mode");
        OledDisp.print("Mag/Arc-Sec T");
        if (Blik) {

          OledDisp.print(" @");
        }
        OledDisp.setPrintPos(1, 37);
        OledDisp.print(sqm.mpsas);
        OledDisp.print(char(0xb1));
        OledDisp.print(sqm.dmpsas);
        OledDisp.print(" ");
        OledDisp.print(int(temp));
        OledDisp.print(char(0xb0));
        OledDisp.print("C");
        OledDisp.setPrintPos(1, 60);
        OledDisp.print("H:");
        OledDisp.print(int(hum));
        OledDisp.print("% P:");
        OledDisp.print(int(pres / 100));
      } while ( OledDisp.nextPage());
      if (Blik) {
        Blik = false;
      } else  {
        Blik = true;
      }
//    delay(100);
    }
  }
}
