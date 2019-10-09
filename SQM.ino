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
#define Version "1.0.3"
#define Serial_No "20191010"

#include "Config.h"

#include <EEPROM.h>
//#include <SPI.h>
//#include <Wire.h>

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

String BME_Msg;
String TSL_Msg;
float temp = 0;
float hum = 0;
float pres = 0;

uint16_t counter   = 0;

#ifdef CALIBRATION_ON 
 float SqmCalOffset ;  // SQM Calibration offset from EEPROM
 float TempCalOffset;   // Temperature Calibration offset from EEPROM
#endif



SQM_TSL2591 sqm = SQM_TSL2591(2591);
void readSQM(void);

void setup() {
 pinMode(ModePin, INPUT_PULLUP);

#ifdef BUZZER_ON
  pinMode(BuzzerPin, OUTPUT);
#endif

  Serial.begin(115200);
//  Serial.setTimeout(1000);

  if ( bme.begin()) {
    switch (bme.chipModel())
    {
      case BME280::ChipModel_BME280:
        BME_Msg  = "BME280 OK";
        break;
      default:
        BME_Msg =  "BME Err";
        InitError = true;
    } 
  } else {
        InitError = true;
  }
  if (sqm.begin()) {
    TSL_Msg = "TSL2591 OK";
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
  }
  else {
    TSL_Msg = "SQM Err";
    InitError = true;
  }
  DisplFirstPage( TSL_Msg, BME_Msg);
  if (!InitError) {
#ifdef BUZZER_ON
    buzzer(500);
#endif
   delay(1000); // Pause for 1 seconds
  } 

  else {

//    for (byte _i = 0; _i < 10; _i++) {
#ifdef BUZZER_ON    
      buzzer(5000);
#endif
      delay(50);
//     }

    while (true);
   
 }


} // end of Setup

void loop() {

String response;

#ifdef CALIBRATION_ON 
   SqmCalOffset = ReadEESqmCalOffset();    // SQM Calibration offset from EEPROM
   TempCalOffset = ReadEETempCalOffset();   // Temperature Calibration offset from EEPROM
   sqm.setCalibrationOffset(SqmCalOffset);
#endif    
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
    sqm.takeReading();  
    DisplSqm( sqm.mpsas, sqm.dmpsas, int(temp+0.5), int(hum), int(pres / 100), '#');
    delay(2000);

  } else {

    if (!SerialOK) {
      DisplWaitUSB('@');
    }
    while (Serial.available() > 0) {
      SerialOK  = true;
      if (!digitalRead(ModePin))  break ;  // check end USB mode
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
        response = "i,00000002,00000003,00000001"
                    + SERIAL_NUMBER;
        Serial.println(response);

      } else if ( command.equals("r")) { // Reading request
  
        response = "r," + sqm_string +
                        + "m,0000005915Hz," 
                        + counter_string 
                        + "c,000000.200s," 
                        + temp_string +"C";
        Serial.println(response);
/*
      } else if (command.equals("u")) { // Unaveraged readin request
        response = "u," + sqm_string + "m,"
                        + "0000005915Hz," 
                        + counter_string + "c,"
                        + "000000.200s," 
                        + temp_string +"C";
        Serial.println(response);
        
*/        
#ifdef EXTENDET_PROTOCOL_ON
      } else if (command.equals("w")) { // My eXtension request for weather information

        String ir_string = String(sqm.ir);
//        while ( ir_string.length() < 5) { 
//        ir_string = String("0" + ir_string );
//        }
        String vis_string = String(sqm.vis);
//        while ( vis_string.length() < 5) { 
//          vis_string = String("0" + vis_string );
//        }
        
        String hum_string = String(int(hum));
        while ( hum_string.length() < 3) { 
          hum_string = String("0" + hum_string );
        }
        String pres_string = String(int(pres / 100));
 //       while ( pres_string.length() < 4) { 
 //         pres_string = String("0" + pres_string );
 //       }
        response = "w," + sqm_string + "m,"
                          + String(sqm.dmpsas, 2) + "#,"
                          + ir_string + "i,"
                          + vis_string + "v,"                          
                          + temp_string + "C,"
                          + hum_string + "h,"
                          + pres_string + "p";
        Serial.println(response);
#endif
#ifdef CALIBRATION_ON 
          } else if (command.equals("c")) { // read Calibration information request

          response= "c," + String(SqmCalOffset,2)  + "d," 
                         + String(TempCalOffset,1) + "t";  
            
         Serial.println(response);
        } else if ( command.equals("zd") ) {
          response = Serial.readStringUntil(',');
          SqmCalOffset=response.toFloat(); 
          WriteEESqmCalOffset(SqmCalOffset);
          Serial.println("zd,"+String(SqmCalOffset,2));
        } else if ( command.equals("zt") ) {
          response = Serial.readStringUntil(',');
          TempCalOffset=response.toFloat(); 
          WriteEETempCalOffset(TempCalOffset);
          Serial.println("zt,"+String(TempCalOffset,1));         
 #endif            
      }
      DisplSqm( sqm.mpsas, sqm.dmpsas, int(temp+0.5), int(hum), int(pres / 100), '@');
    }
  }
}
