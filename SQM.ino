/*
   SQM.ino   Sky Quality Meter

   Copyright (c) 2019 Roman Hujer   http://hujer.net

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
#define Version "1.0.2"
#define Serial_No "20191008"

#include "Config.h"

#include <SPI.h>
#include <Wire.h>


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
float temp = 0;
float hum = 0;
float pres = 0;

int counter   = 0;

#ifdef CALIBRATION_ON 
 float SqmCalOffset = SQM_CAL_OFFSET;    // SQM Calibration offset
 float TempCalOffset = TEMP_CAL_OFFSET;   // Temperature Calibration offset
#endif



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
        BME_MSG =  "BMP280 no Humid.";
        break;
      default:
        SensorID = "N/A";
        BME_MSG =  "UNKNOWN sensor!";
        InitError = true;
    }
  }
  if (sqm.begin()) {
    TSL_MSG = "TSL2591  Success";
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
#ifdef CALIBRATION_ON 
    sqm.setCalibrationOffset(SqmCalOffset);
#endif    
  }
  else {
    TSL_MSG = "SQM notf ound";
    InitError = true;
  }
  // OledDisp.setRot180();

  DisplFirstPage( TSL_MSG, BME_MSG);

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

 String response;

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
 
  }

  if ( !USBmodeON ) {
    ReadWeather() ;
#ifdef TEMPER_CALIB_ON
    sqm.setTemperature( temp );
#endif
    //        OledDisp.sleepOn();
    
     sqm.takeReading();
    
    //        OledDisp.sleepOff();
    DisplSqm( sqm.mpsas, sqm.dmpsas, int(temp+0.5), int(hum), int(pres / 100), '#');
    delay(2000);

  } else {

    if (!SerialOK) {
      DisplWaitUSB('@');
    }
    while (Serial.available() > 0) {
      SerialOK  = true;
      if (digitalRead(ModePin) == 0)  break ;  // check end USB mode
      ReadWeather();
#ifdef TEMPER_CALIB_ON
      sqm.setTemperature( temp );
#endif
      sqm.takeReading();
      String counter_string = String(counter++);
      while ( counter_string.length() < 10) {
        counter_string = String("0" + counter_string);
      }
      String sqm_string = String(abs(sqm.mpsas), 2);
      while ( sqm_string.length() < 5) {
        sqm_string = String("0" + sqm_string);
      }
      if (sqm.mpsas < 0.) {
        sqm_string = String("-" + sqm_string);
      } else {
        sqm_string = String(" " + sqm_string);
      }
      String temp_string = String(abs(temp), 1);
      while ( temp_string.length() < 5) {
        temp_string = String("0" + temp_string);
      }
      if (temp < 0.) {
        temp_string = String("-" + temp_string);
      } else {
        temp_string = String(" " + temp_string);
      }

      String command = Serial.readStringUntil('x');

      if ( command.equals("i")) {  // Unit information request (note lower case "i")

        response = "i," + String(PROTOCOL_NUMBER) + "," 
                        + String(MODEL_NUMBER) + ","
                        + String(FEATURE_NUMBER) + "," 
                        + SERIAL_NUMBER;
        Serial.println(response);

      } else if ( command.equals("r")) { // Reading request
   
        response = "r," + sqm_string + "m,"
                        + "0000005915Hz," 
                        + counter_string + "c,"
                        + "000000.200s," 
                        + temp_string +"C";
        Serial.println(response);

      } else if (command.equals("u")) { // Unaveraged readin request
        response = "u," + sqm_string + "m,"
                        + "0000005915Hz," 
                        + counter_string + "c,"
                        + "000000.200s," 
                        + temp_string +"C";
        Serial.println(response);
        
        /* Not now  redy to use
              } else if (command.equals("c")) { // Calibration information request
                response = "c,00000017.60m,0000000.000s, 039.4C,00000008.71m, 039.4C";
                Serial.println(response);
        */
        
#ifdef EXTENDET_PROTOCOL_ON
      } else if (command.equals("w")) { // My eXtension request for weather information
        String full_string = String(sqm.full);     
        while ( full_string.length() < 5) { 
          full_string = String("0" + full_string );
        }
        String ir_string = String(sqm.ir);
        while ( ir_string.length() < 5) { 
          ir_string = String("0" + ir_string );
        }
        String vis_string = String(sqm.vis);
        while ( vis_string.length() < 5) { 
          vis_string = String("0" + vis_string );
        }
        String hum_string = String(int(hum));
        while ( hum_string.length() < 3) { 
          hum_string = String("0" + hum_string );
        }
        String pres_string = String(int(pres / 100));
        while ( pres_string.length() < 4) { 
          pres_string = String("0" + pres_string );
        }
        response = "w," + sqm_string + "m,"
                          + String(sqm.dmpsas, 2) + "#,"
                          + full_string + "f,"
                          + ir_string + "i,"
                          + vis_string + "v,"
                          + temp_string + "C,"
                          + hum_string + "h,"
                          + pres_string + "p";
         Serial.println(response);
#endif
      }
      DisplSqm( sqm.mpsas, sqm.dmpsas, int(temp+0.5), int(hum), int(pres / 100), '@');
    }
  }
}
