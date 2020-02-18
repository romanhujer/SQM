/*

# Copyright (c) 2018 Roman Hujer 

*/


const char* sensor_key = "xxxxxxxxxxxxxxxxxx";  // sensor01


#define SQM_CAL_OFFSET  -1.20  // default SQM Calibration offset but priority is in EEPROM stored value
#define TEMP_CAL_OFFSET 0.0   // default Temperature Calibration offset but priority is in EEPROM stored value

#define SENSOR_TYPE_BME280

#define SLEEP_SEC 300

#define ESP01_ON  // For ESP-01  is _ON   for  ESP8266 in _OFF 

const char* host = "my.cloud.iot";
const char* ssid     = "my_ssid";
const char* password = "my_password";
