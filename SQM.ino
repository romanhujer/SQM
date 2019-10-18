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
#define Version "1.0.6"
#define SERIAL_NUMBER "20191018"
#include "Config.h"
#include "Setup.h"
#include "Validate.h"

#include <Wire.h>
#include <EEPROM.h>
#include <BMx280I2C.h>
#include <U8x8lib.h>
#include "SQM_TSL2591.h"

// Setup  OLed Display

#ifdef SH1106_ON
      U8X8_SH1106_128X64_NONAME_HW_I2C OledDisp(/* reset=*/ U8X8_PIN_NONE);
#endif  
#ifdef SSD1306_ON
       U8X8_SSD1306_128X64_NONAME_HW_I2C OledDisp(/* reset=*/ U8X8_PIN_NONE);
#endif 

// Setup temperatue sensor
BMx280I2C bme(BME_I2C_ADDRESS);

// Setup for SQM TSL2591
SQM_TSL2591 sqm = SQM_TSL2591(2591);

float SqmCalOffset =  SQM_CAL_OFFSET ;   // SQM Calibration offset from EEPROM
float TempCalOffset = TEMP_CAL_OFFSET;   // Temperature Calibration offset from EEPROM

boolean InitError = false;
boolean USBmodeON = false;
boolean SerialOK  = false;
boolean Blik      = false;
boolean Humidity  = true;

float temp = 0;
float hum = 0;
float pres = 0;
uint16_t counter = 0;
byte page = 0;
String BME_Msg;
String TSL_Msg;
char oled[6] = "A5,11";

void setup() {

  Wire.begin(); 
  pinMode(ModePin, INPUT_PULLUP);
  pinMode(BuzzerPin, OUTPUT);
  Serial.begin(SERIAL_BAUD);
  Serial.setTimeout(1000);
  if ( bme.begin()){ 
     if (bme.isBME280()){
        Humidity = true;
#ifdef SH1106_ON                
        BME_Msg  = "BME280  OK";
#endif  
#ifdef SSD1306_ON
        BME_Msg  = "BME"; 
#endif                     
        bme.resetToDefaults();        
        bme.writeOversamplingPressure(BMx280MI::OSRS_P_x16);
        bme.writeOversamplingTemperature(BMx280MI::OSRS_T_x16);
        bme.writeOversamplingHumidity(BMx280MI::OSRS_H_x16);
     } else {
        Humidity = false;
#ifdef SH1106_ON        
        BME_Msg =  "no Humidity";
#endif
#ifdef SSD1306_ON        
        BME_Msg =  "noH";
#endif        
         
     }
  } else {
#ifdef SH1106_ON            
       BME_Msg =  "BME Err";
#endif
#ifdef SSD1306_ON        
       BME_Msg =  "Berr";
#endif       
       InitError = true;
  }


void readSQM(void);
  if (sqm.begin()) {
#ifdef SH1106_ON            
    TSL_Msg = "TSL2591 OK";
#endif
#ifdef SSD1306_ON        
        TSL_Msg = "TSL";
#endif        
    sqm.verbose = false;
    sqm.config.gain = TSL2591_GAIN_LOW;
    sqm.config.time = TSL2591_INTEGRATIONTIME_200MS;
    sqm.configSensor();
//    sqm.showConfig();
  } 
  else {
#ifdef SH1106_ON                
    TSL_Msg = "TSL2591 Err";
#endif
#ifdef SSD1306_ON        
    TSL_Msg = "Terr";
#endif            
    InitError = true;
  } // end of if (sqm.begin())
    
 
// Init Oled Dislay
if ( OledDisp.begin()) {
    OledDisp.setFont(OLED_FONT);
    OledDisp.setPowerSave(false);
    oled[3]='1';
    oled[4]= (ReadEEAutoContras())? '1' : '0';
    DisplFirstPage();
  }
  else {
    InitError = true;
    Serial.println("OLED Err");
  } // end of if (sqm.begin())

  if (InitError) {
     for (byte _i = 0; _i < 20; _i++) {
        buzzer(50);
        delay(50);
     }
     while (true);
   } // end of if (InitError) 
   
   delay(1000); // Pause for 1 seconds

   SqmCalOffset = ReadEESqmCalOffset();     // SQM Calibration offset from EEPROM
   TempCalOffset = ReadEETempCalOffset();   // Temperature Calibration offset from EEPROM
   
   sqm.setCalibrationOffset(SqmCalOffset);  // call offset
   
   DisplCalData();      
   delay(1000); // Pause for 1 seconds

} // end of Setup

//=======================================================================================

void loop() {
  String response;
  
  if (digitalRead(ModePin))   {
    SerialOK  = false;
    if (USBmodeON) {
      buzzer(200);
    }
    USBmodeON = false;
  }
  else {

    if (!USBmodeON) {
      buzzer(200);
    }
    USBmodeON = true;
  }
  if ( !USBmodeON ) {
    OledDisp.setPowerSave(false);
    oled[3]='1';
    ReadWeather() ;

    if (ReadEEAutoTempCal()) sqm.setTemperature( temp );  //temp call
    
    sqm.takeReading();  
    
    DisplSqm( sqm.mpsas, sqm.dmpsas, int(temp+0.5), int(hum), int(pres / 100), '#'); 

    delay(3000);
    
  }
    else {
//
// USB mode 
// ======================================================
    if (!SerialOK) {
      DisplWaitUSB('@');
      delay(50);
    }
     while ( Serial.available()) {       
      char  _sign;
//      float _f;   
      SerialOK  = true;
      
      
      if (digitalRead(ModePin))  break ;  // check end USB mode

      ReadWeather();
      if (ReadEEAutoTempCal()) sqm.setTemperature( temp );   //calib

      String counter_string = String(counter++);
      while ( counter_string.length() < 10) {
        counter_string = '0' + counter_string;
      }
   
      sqm.takeReading();
      String sqm_string = String( ( sqm.mpsas <0) ? -sqm.mpsas : sqm.mpsas, 2);
      while ( sqm_string.length() < 5) {
        sqm_string = '0' + sqm_string;
      }
      _sign = ( sqm.mpsas < 0 ) ? '-' : ' ';
      sqm_string = _sign + sqm_string;


      String temp_string = String( (temp < 0 ) ? - temp : temp, 1);
      while ( temp_string.length() < 5) {
        temp_string = '0' + temp_string;
      }
      _sign = ( temp < 0 ) ? '-' : ' ';
      temp_string = _sign + temp_string;
   
      String command = Serial.readStringUntil('x');

// Unit information request (note lower case "i")
      if ( command.equals("i")) {  
        Serial.print("i,00000002,00000003,00000001,");
        Serial.println(SERIAL_NUMBER);
        
// Reading request
      } else if ( command.equals("r")) { 

         response = "r," + sqm_string 
                        + "m,0000002591Hz," 
                        + counter_string 
                        + "c,0000000.200s," 
                        + temp_string +"C";
         Serial.println(response);

// Unaveraged reading request
      } else if (command.equals("u")) { 
        response = "u," + sqm_string 
                        + "m,0000002591Hz," 
                        + counter_string 
                        + "c,0000000.200s," 
                        + temp_string +"C";
        Serial.println(response);     

#ifdef EXTENDET_PROTOCOL_ON

// My extension request for weather information
      } else if (command.equals("w")) { 
        String ir_string = String(sqm.ir);
        while ( ir_string.length() < 5) { 
        ir_string = '0' + ir_string;
        }
        String vis_string = String(sqm.vis);
        while ( vis_string.length() < 5) { 
          vis_string = '0' + vis_string;
        }
        String f_string = String(sqm.full);
        while ( f_string.length() < 5) { 
          f_string = '0' + f_string;
        }
        String hum_string = String(int(hum));
        while ( hum_string.length() < 3) { 
          hum_string = '0' + hum_string;
        }
        String pres_string = String(int(pres / 100));
        while ( pres_string.length() < 4) { 
         pres_string = '0' + pres_string;
       }
       response = "w," + sqm_string + "m,"
                          + String(sqm.dmpsas, 2) + "e,"
                          + f_string + "f,"
                          + ir_string + "i,"
                          + vis_string + "v,"                          
                          + hum_string + "h,"
                          + pres_string + "p,"
                          + temp_string + "C";
        Serial.println(response);
        
#endif // end of  EXTENDET_PROTOCOL_ON

// read Calibration information request
       } else if (command.equals("c")) { 
              sqm_string = String( (SqmCalOffset < 0 ) ? - SqmCalOffset : SqmCalOffset, 2);            
              while ( sqm_string.length() < 9) {
                 sqm_string = '0' + sqm_string;
              }
              _sign = ( SqmCalOffset < 0 ) ? '-' : '0';
              sqm_string = _sign + sqm_string;
//              _f= ( TempCalOffset < 0 )? -TempCalOffset : TempCalOffset;  
//              temp_string = String( _f, 1);
              temp_string = String( ( TempCalOffset < 0 )? -TempCalOffset : TempCalOffset, 1);  
              while ( temp_string.length() < 5) {
                temp_string = '0' + temp_string;
              }
              _sign = ( TempCalOffset < 0 ) ? '-' : ' ';
              temp_string = _sign + temp_string;
              response= "c," + sqm_string  + "m," 
                             + "0000000.000s,"
                             + temp_string + "C,"
                             + sqm_string  + "m,"
                             + temp_string + "C"; 
             Serial.println(response);
          }   else if ( command[0] == 'z' ) {
              response = command.substring(1,4);

// Calibartion commad           
           if ( response.equals("cal")) { 
              char _x = command[4];                       
              if (  _x == '5') {    // Calibration Light offest 
                response = command.substring(5); 
                SqmCalOffset=response.toFloat();        
                WriteEESqmCalOffset(SqmCalOffset);          
                sqm_string = String( (SqmCalOffset <0 )? -SqmCalOffset : SqmCalOffset , 2);
                while ( sqm_string.length() < 9) {
                   sqm_string = '0' + sqm_string;
                }
                _sign = ( SqmCalOffset < 0 ) ? '-' : '0';
                sqm_string = _sign + sqm_string;
                Serial.println("z,5,"+ sqm_string + 'm');
              } 
              
// Calibration Temperature
              else if ( (_x == '6') || (_x == '8') ) {  
                
                response = command.substring(5); 
                TempCalOffset=response.toFloat();          
                WriteEETempCalOffset(TempCalOffset); 
//                _f= ( TempCalOffset < 0 )? -TempCalOffset : TempCalOffset;  
//                temp_string = String( _f, 1);
                temp_string = String( ( TempCalOffset < 0 )? -TempCalOffset : TempCalOffset, 1);
                while ( temp_string.length() < 5) {
                  temp_string = '0' + temp_string;
                }
                _sign = ( TempCalOffset < 0 ) ? '-' : ' ';
                temp_string = _sign + temp_string;
                Serial.println("z," + String(_x )+ ',' + temp_string + 'C');
               } 

// Calibration Light offest                               
               else if ( _x == '7') {                      
                   Serial.println("z," + String(_x) + ',' + command.substring(5) + 's');                
                   }
// Set display contras                                
               else if ( _x == '9') {           
                   response = command.substring(5);
                   WriteEEScontras( response.toInt());         
                   Serial.println("z," + String(_x) + ',' + command.substring(5) + 'x');                
               }
               
// Enable temperature callibration               
               else if ( _x == 'A') { 
                    WriteEEAutoTempCal(true);
                    Serial.println("zAaL");                        
               }

// Disable temperature callibration               
               else if ( _x == 'B') { 
                    WriteEEAutoTempCal(true);
                    Serial.println("zBaL");                        
               }

// Delete calibration - set default factory value see Setup.h              
               else if ( _x == 'D') { 
                  SqmCalOffset =  SQM_CAL_OFFSET ;   // set to default 
                  TempCalOffset = TEMP_CAL_OFFSET;   // set to default 
                  WriteEETempCalOffset(TempCalOffset); 
                  WriteEESqmCalOffset(SqmCalOffset); 
                  WriteEEScontras(DEFALUT_CONTRAS); 
                  Serial.println("zxdL");                        
               }
        
          }           
// Disable Oled Display          
        } else if (command.equals("A50")) {
          oled[3] = '0';
          OledDisp.setPowerSave(true);        
          Serial.println(oled);
// Enable Oled Display                    
        } else if (command.equals("A51")) {
          oled[3] = '1';
          OledDisp.setPowerSave(false); 
          Serial.println(oled);       
// Disable Autocontras - Dimmer                    
        } else if (command.equals("A5d")) {
          oled[4] = '0';
          WriteEEAutoContras(false);
          Serial.println(oled);
// Enable Autocontras - Dimmer                             
        } else if (command.equals("A5e")) {
          oled[4] = '1';
          WriteEEAutoContras(true);
          Serial.println(oled);
// Display status          
        } else if (command.equals("A5")) {
//         oled[4]= (ReadEEAutoContras())? '1' : '0';
          Serial.println(oled);
        }

        DisplSqm( sqm.mpsas, sqm.dmpsas, int(temp+0.5), int(hum), int(pres / 100), '@');
        SqmCalOffset  = ReadEESqmCalOffset();    // SQM Calibration offset from EEPROM
        TempCalOffset = ReadEETempCalOffset();   // Temperature Calibration offset from EEPROM
        sqm.setCalibrationOffset(SqmCalOffset);
       
    } // end of while ( Serial.available() )
  }  // end of if (digitalRead(ModePin)) 
} // end of loop()
