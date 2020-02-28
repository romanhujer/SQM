/*

 * Copyright (c) 2020 Roman Hujer 

*/

const char* sensor_key = "xxxxxxxxxxxxxxxxxx";  // For auth on web server
const char* host       = "my.cloud.iot";
const char* ssid       = "my_ssid";
const char* password   = "my_password";

//select chip
#define ESP01_ON  // For ESP-01 is _ON for full ESP8266 is _OFF 
#define DEEP_SLEEP_ON  //only ESP8266 for battery run  

#define SERIAL_BAUD 74880  // 74880 default  pro ESP chip boot  (For some Linux need use 115200)   

#define EEPROM_OFF         // USE eeprom for store callibration values 
#define SQM_CAL_OFFSET  -2.00  // default SQM Calibration offset but priority is in EEPROM stored value
#define TEMP_CAL_OFFSET 0.0    // default Temperature Calibration offset but priority is in EEPROM stored value

#define SLEEP_SEC 300   // Interval
