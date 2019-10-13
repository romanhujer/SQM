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
#define Version "1.0.4"
String SERIAL_NUMBER = "20191012";
#include "Config.h"
#include "Setup.h"
#include "Validate.h"
#include <Wire.h>

#ifdef USE_EEPROM_ON
 #include <EEPROM.h>
#endif 

// Setup  OLed Display
#ifdef USE_OLED_ON
  #ifdef USE_U8x8_ON     
    #include <U8x8lib.h>
    #ifdef SH1106_ON
      U8X8_SH1106_128X64_NONAME_HW_I2C OledDisp(/* reset=*/ U8X8_PIN_NONE);
    #endif  
    #ifdef SSD1306_ON
      U8X8_SSD1306_128X64_NONAME_HW_I2C OledDisp(/* reset=*/ U8X8_PIN_NONE);
    #endif 
  #endif    
  #ifdef USE_U8G2_ON
    #include <U8g2lib.h>
    #ifdef SH1106_ON
      U8G2_SH1106_128X64_NONAME_F_HW_I2C  OledDisp(U8G2_R0, /* reset=*/ U8X8_PIN_NONE); 
    #endif
    #ifdef SSD1306_ON
     U8G2_SSD1306_128X64_NONAME_F_HW_I2C OledDisp(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
    #endif 
  #endif
  #ifdef USE_U8GLIB_ON
    #include <U8glib.h>
    #ifdef SH1106_ON
    U8GLIB_SH1106_128X64 OledDisp(U8G_I2C_OPT_NONE);
    #endif
    #ifdef  SSD1306_ON
    U8GLIB_SSD1306_128X64 OledDisp(U8G_I2C_OPT_NONE);
    #endif
  #endif
#endif

#ifdef USE_WEATHER_SENSOR_ON
// Setup temperatue sensor
  #include <BMx280I2C.h>
  BMx280I2C bme(BME_I2C_ADDRESS);
#endif

#ifdef USE_SQM_SENSOR_ON
// Setup for SQM TSL2591
  #include "SQM_TSL2591.h"
  SQM_TSL2591 sqm = SQM_TSL2591(2591);
#endif

#ifdef CALIBRATION_ON 
  float SqmCalOffset =  SQM_CAL_OFFSET ;   // SQM Calibration offset from EEPROM
  float TempCalOffset = TEMP_CAL_OFFSET;   // Temperature Calibration offset from EEPROM
#endif

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

void setup() {

Wire.begin(); 

pinMode(ModePin, INPUT_PULLUP);

#ifdef BUZZER_ON
  pinMode(BuzzerPin, OUTPUT);
#endif

Serial.begin(SERIAL_BAUD);
//  Serial.setTimeout(1000);

#ifdef USE_WEATHER_SENSOR_ON
  
  if ( bme.begin()){ 
     if (bme.isBME280()){
        Humidity = true;
        BME_Msg  = "BME280  OK";
        bme.resetToDefaults();        
        bme.writeOversamplingPressure(BMx280MI::OSRS_P_x16);
        bme.writeOversamplingTemperature(BMx280MI::OSRS_T_x16);
        bme.writeOversamplingHumidity(BMx280MI::OSRS_H_x16);
     } else {
        BME_Msg =  "no Humidity";
        Humidity = false ;
     }
  } else {
       BME_Msg =  "BME Err";
       InitError = true;
  }
#else   
  BME_Msg =  "no Weather";
#endif 
#ifdef DEBUG_ON
  Serial.println(BME_Msg);
#endif

#ifdef USE_SQM_SENSOR_ON
  void readSQM(void);
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
#else 
    TSL_Msg = "No SQM";
#endif 
    
#ifdef DEBUG_ON
  Serial.println(SQM_Msg);
#endif

  
#ifdef USE_OLED_ON
  if ( OledDisp.begin()) {
    OledDisp.setFont(OLED_FONT);
 #ifndef USE_U8GLIB_ON 
    OledDisp.setPowerSave(0);
 #endif    
    DisplFirstPage( TSL_Msg, BME_Msg);
  }
  else {
    InitError = true;
    Serial.println("OLED Err");
  }
#else
   Serial.println("SQM no OLED V" + Version);
   Serial.println(SQM_Msg);
   Serial.println(BME_Msg); 
#endif 
    
  if (!InitError) {
#ifdef BUZZER_ON
    buzzer(500);
#endif
   delay(1000); // Pause for 1 seconds
  } 
  else {
    for (byte _i = 0; _i < 10; _i++) {
#ifdef BUZZER_ON    
      buzzer(50);
#endif
      delay(50);
     }
#ifdef DEBUG_ON
  Serial.println("System Error!");
#endif
     while (true);
   }

} // end of Setup

void loop() {

#ifdef CALIBRATION_ON 
  #ifdef USE_EEPROM_ON
    SqmCalOffset = ReadEESqmCalOffset();    // SQM Calibration offset from EEPROM
    TempCalOffset = ReadEETempCalOffset();   // Temperature Calibration offset from EEPROM
  #endif
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
#ifdef USE_WEATHER_SENSOR_ON    
    ReadWeather() ;
#endif

#ifdef USE_SQM_SENSOR_ON
  #ifdef TEMPER_CALIB_ON
    #ifdef USE_WEATHER_SENSOR_ON    
      sqm.setTemperature( temp );
    #endif
  #endif
    sqm.takeReading();  
    DisplSqm( sqm.mpsas, sqm.dmpsas, int(temp+0.5), int(hum), int(pres / 100), '#'); 
#else 
    DisplSqm( 0., 0., int(temp+0.5), int(hum), int(pres / 100), '#');
#endif     
    delay(2000);
  } else {
//
// USB mode 
// ======================================================
    if (!SerialOK) {
#ifdef USE_OLED_ON      
      DisplWaitUSB('@');
#else       
    Serial.println("Wait Serial data");      
#endif      
    }
      while (Serial.available() > 0) {
       String response;  
       SerialOK  = true;
      if (!digitalRead(ModePin))  break ;  // check end USB mode
#ifdef USE_WEATHER_SENSOR_ON    
      ReadWeather();
#endif
#ifdef TEMPER_CALIB_ON
  #ifdef USE_WEATHER_SENSOR_ON    
      sqm.setTemperature( temp );
  #endif      
#endif
      String counter_string = String(counter++);
      while ( counter_string.length() < 10) {
        counter_string = "0" + counter_string;
      }
      char _sign, _tsig;
#ifdef USE_SQM_SENSOR_ON
      sqm.takeReading();
      String sqm_string = String(abs(sqm.mpsas), 2);
      while ( sqm_string.length() < 5) {
        sqm_string = "0" + sqm_string;
      }
      _sign = ( sqm.mpsas < 0. ) ? '-' : ' ';
      sqm_string = _sign + sqm_string;
#else 
      String sqm_string = " 00.00";
#endif
#ifdef USE_WEATHER_SENSOR_ON
      String temp_string = String(abs(temp), 1);
      while ( temp_string.length() < 5) {
        temp_string = "0" + temp_string;
      }
      _tsig = ( temp < 0. ) ? '-' : ' ';
      temp_string = _tsig + temp_string;
#else
      String temp_string = " 000.0";
#endif      
      String command = Serial.readStringUntil('x');

      if ( command.equals("i")) {  // Unit information request (note lower case "i")
        response = "i,00000002,00000003,00000001,"
                    + SERIAL_NUMBER;
        Serial.println(response);

      } else if ( command.equals("r")) { // Reading request
  
        response = "r," + sqm_string 
                        + "m,0000005915Hz," 
                        + counter_string 
                        + "c,000000.200s," 
                        + temp_string +"C";
        Serial.println(response);

      } else if (command.equals("u")) { // Unaveraged reading request
        response = "u," + sqm_string 
                        + "m,0000005915Hz," 
                        + counter_string 
                        + "c,000000.200s," 
                        + temp_string +"C";
        Serial.println(response);     
     
#ifdef EXTENDET_PROTOCOL_ON
      } else if (command.equals("w")) { // My extension request for weather information
  #ifdef USE_SQM_SENSOR_ON
        String ir_string = String(sqm.ir);
        while ( ir_string.length() < 5) { 
        ir_string = "0" + ir_string;
        }
        String vis_string = String(sqm.vis);
        while ( vis_string.length() < 5) { 
          vis_string = "0" + vis_string;
        }
   #else
         String ir_string =  "00000";
         String vis_string = "00000";
   #endif
   #ifdef USE_WEATHER_SENSOR_ON        
        String hum_string = String(int(hum));
        while ( hum_string.length() < 3) { 
          hum_string = "0" + hum_string;
        }
        String pres_string = String(int(pres / 100));
        while ( pres_string.length() < 4) { 
         pres_string = "0" + pres_string;
       }
   #else
        String hum_string = "000";
        String hum_string = "0000";
   #endif
        response = "w," + sqm_string + "m,"
   #ifdef USE_SQM_SENSOR_ON        
                          + String(sqm.dmpsas, 2) + "e,"
   #else     
                          + "0.00e,"
   #endif                          
                          + ir_string + "i,"
                          + vis_string + "v,"                          
                          + temp_string + "C,"
                          + hum_string + "h,"
                          + pres_string + "p";
        Serial.println(response);
#endif
#ifdef CALIBRATION_ON 
          } else if (command.equals("c")) { // read Calibration information request

          _sign = ( SqmCalOffset  < 0. ) ? '-' : ' ';
          _tsig = ( TempCalOffset < 0. ) ? '-' : ' ';

          response= "c," + String(_sign) + String(abs(SqmCalOffset),2)  + "d," 
                         + String(_tsig) + String(abs(TempCalOffset),1) + "t";
                                       
         Serial.println(response);
  #ifdef  USE_EEPROM_ON       
        } else if ( command.equals("zd") ) {
          response = Serial.readStringUntil('e');
          SqmCalOffset=response.toFloat();        
          WriteEESqmCalOffset(SqmCalOffset);          
          Serial.println("zd,"+String(SqmCalOffset,2));
        } else if ( command.equals("zt") ) {
          response = Serial.readStringUntil('e');
          TempCalOffset=response.toFloat();          
          WriteEETempCalOffset(TempCalOffset);          
          Serial.println("zt,"+String(TempCalOffset,1));
  #endif                   
#endif            
        }
#ifdef USE_OLED_ON        
        DisplSqm( sqm.mpsas, sqm.dmpsas, int(temp+0.5), int(hum), int(pres / 100), '@');
#endif        
    }
  }
}
