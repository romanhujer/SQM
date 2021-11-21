/*

 * Copyright (c) 2020 Roman Hujer 

*/

const char* host       = "my.cloud.iot";
String app             = "/app.bin";  //URI for HTTP GET  app
#define HTTP_PORT  80

const char* ssid       = "my_ssid";
const char* password   = "my_password";

const char* SensorID = "SQM-ESP01";
const char* sensor_key = "xxxxxxxxxxxxxxxxxx";  // For auth on web server

//select chip
#define ESP01_ON  // For ESP-01 is _ON for full ESP8266 is _OFF 
#define DEEP_SLEEP_OFF  //only ESP8266 for battery run  

//#define SERIAL_BAUD 74880  // 74880 default  pro ESP chip boot  (For some Linux need use 115200)   
#define SERIAL_BAUD 115200


#define EEPROM_OFF         // USE eeprom for store callibration values 
#define SQM_CAL_OFFSET  -2.50  // default SQM Calibration offset but priority is in EEPROM stored value
#define TEMP_CAL_OFFSET 0.0    // default Temperature Calibration offset but priority is in EEPROM stored value

#define SLEEP_SEC 300   // Interval
