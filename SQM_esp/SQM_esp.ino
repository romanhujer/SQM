/*

   Copyright (c) 2020 Roman Hujer

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#define Product "SQM ESP"
#define Version "1.0"

#include <Wire.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>


//
#include "Config.h"
// My testing config
//#include "myConfig.h"


#include "Setup.h"

#include <BMx280I2C.h>
BMx280I2C bme(BME_I2C_ADDRESS);

#include "SQM_TSL2591.h"


// Setup for SQM TSL2591
SQM_TSL2591 sqm = SQM_TSL2591(2591);

float SqmCalOffset =  SQM_CAL_OFFSET ;   // SQM Calibration offset from EEPROM
float TempCalOffset = TEMP_CAL_OFFSET;   // Temperature Calibration offset from EEPROM

boolean InitError = false;
boolean Humidity  = true;

float temp = 0;
float hum =  0;
float pres = 0;
float mas =  0;
float dmas = 0;
 
uint16_t counter = 0;
String BME_Msg;
String TSL_Msg;
String WiFi_Msg;

void setup()
{
#ifdef ESP01_ON
  Wire.begin(0, 2); // I2C pin ESP01
#else
  Wire.begin();   // I2C pin ESP8266 
#endif
  Serial.begin(SERIAL_BAUD);
  Serial.setTimeout(1000);
  Serial.println();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
retry:
  Serial.printf("Wait for WiFi.");
  for (uint8_t t = 30; t > 0; t--) {
    if ( WiFi.status() == WL_CONNECTED ) break;
    Serial.print(".");
    delay(500);
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial.printf("\nWIFI not connect. Now sleep retry next 60sec...");
#ifdef DEEP_SLEEP_ON
    ESP.deepSleep(60e6);
#else
    delay (60000);
    goto retry;
#endif
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if ( bme.begin()) {
    if (bme.isBME280()) {
      Humidity = true;
      BME_Msg  = "BME280  OK";
      bme.resetToDefaults();
      bme.writeOversamplingPressure(BMx280MI::OSRS_P_x16);
      bme.writeOversamplingTemperature(BMx280MI::OSRS_T_x16);
      bme.writeOversamplingHumidity(BMx280MI::OSRS_H_x16);
    } else {
      Humidity = false;
      BME_Msg =  "no Humidity";
    }
  } else {
    BME_Msg =  "BME Err";
    InitError = true;
  }
  Serial.println(BME_Msg);
  
// init SQM sensor
  void readSQM(void);
  if (sqm.begin()) {
    sqm.verbose = false;
    sqm.config.gain = TSL2591_GAIN_LOW;
    sqm.config.time = TSL2591_INTEGRATIONTIME_200MS;
    sqm.configSensor();
    sqm.showConfig();
    TSL_Msg = "TSL2591 OK";
  }
  else {
    TSL_Msg = "TSL2591 Err";
    InitError = true;
  } // end of if (sqm.begin())

  Serial.println(TSL_Msg);

#ifdef  EEPROM_ON
  SqmCalOffset = ReadEESqmCalOffset();     // SQM Calibration offset from EEPROM
  TempCalOffset = ReadEETempCalOffset();   // Temperature Calibration offset from EEPROM
#else 
  SqmCalOffset =  SQM_CAL_OFFSET;  
  TempCalOffset = TEMP_CAL_OFFSET; 
#endif

  sqm.setCalibrationOffset(SqmCalOffset);  // call offset
}

void loop()
{
  String response;
  String url;
  float battery = 0;

  ReadWeather();
  
  if (ReadEEAutoTempCal()) sqm.setTemperature( temp );  //temp call
  
  sqm.takeReading();  
  
  mas = sqm.mpsas;
  dmas = sqm.dmpsas;
  battery = int(analogRead(A0) / 1023.0 * 11 * 100 + 0.5) / 100.;
  Serial.println("");
  Serial.print("SQM: ");
  Serial.print(mas);
  Serial.print("+-");
  Serial.print(dmas);
  Serial.println("mas^2");
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.println("°C");
  Serial.print("Humidity: ");
  Serial.print(hum );
  Serial.println("%");
  Serial.print("Pressure: ");
  Serial.print(pres/100);
  Serial.println(" hPa");
  Serial.print("Battery Voltage: ");
  Serial.println(battery);
    url = "?ID=";
    url += SensorID;
    url += "&KEY=";
    url += sensor_key;
    url += "&T=";
    url +=  temp;
    url += "&H=";
    url += hum;
    url += "&P=";
    url += pres/100;
    url += "&S=";
    url += mas;
    url += "&D=";
    url += dmas;
    url += "&V=";
    url += battery;
    send_cloud ( url  );

#ifdef DEEP_SLEEP_ON
/// WAKE_RF_DEFAULT, WAKE_RFCAL, WAKE_NO_RFCAL, WAKE_RF_DISABLED.
  Serial.println("WiFi disconnet");
  WiFi.disconnect();
  delay(100);
  Serial.println("Deep sleep.");
  ESP.deepSleep(SLEEP_SEC * 1000000);
#else
  delay( SLEEP_SEC * 1000); // cekej
#endif
}

void send_cloud ( String url)
{
  

  WiFiClient wifi_client;
  const int httpPort = HTTP_PORT;


  if (!wifi_client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // This will send the request to the server
  wifi_client.print(String("GET ") + app + url + " HTTP/1.1\r\n" +
                    "Host: " + host + "\r\n" +
                    "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (wifi_client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println("HTTP Timeout !");
      wifi_client.stop();
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while (wifi_client.available()) {
    String line = wifi_client.readStringUntil('\r');
   Serial.print(line);
  }
  Serial.println();
  Serial.println("closing connection");

}


void ReadWeather() {
  if ( bme.measure() ) {

    do {
      delay(100);
    } while (!bme.hasValue());
    pres = bme.getPressure();
    temp = bme.getTemperature();
    if ( Humidity) hum =  bme.getHumidity();
    else hum = 0;
    temp = temp + TempCalOffset;  //calll

  }
}
