# OnStep WiFi Server
This is the EPS-01 and ESP8266 SQM 

# Installing the ESP8266 Platform
Before you can flash the firmware on your ESP8266 device, you need to install the ESP8266
platform for the Arduino IDE.

Under Preferences add this line in the additional "Boards Manager" area:
  http://arduino.esp8266.com/stable/package_esp8266com_index.json

Then from the "Tools, Boards, Boards Manager" menu select and install the ESP8266 add-on.

# Flashing The WiFi
The exact flashing procedure depends on which ESP8826 device you will be using.

In the Arduino IDE, pick your device from "Tools, Boards". 

For an ESP-01, select "Generic 8266 Module".

Pin CHPD/EN must be HIGH (connected to 3.3V)
Pin GPIO0 must be LOW (connected to GND).  Use flash swich on PCB aan reset



For the ESP-01, use the following parameters under Tools:

- Board: "Generic ESP8266 Module"
- Flash Mode: "DIO"
- Erase Flash: "Sketch + WiFi Settings"
- Flash Size: "1M (64K SPIFFS)" (Most ESP01 have 1M, older ones have 512K)
- Flash Frequency: "40MHz"
- CPU Frequency: "80 MHz"
- Reset Method: "ck"
- Upload Speed: "115200"
- Debug Port: "Disabled"
- Debug Level: "None"

Open the SQM_esp.ino file inside this folder. Check the "Config.h" file and set your vaules  for 
<pre>
 const char* sensor_key = "xxxxxxxxxxxxxxxxxx";  // For auth on web server
 const char* host = "my.cloud.iot";
 const char* ssid     = "my_ssid";
 const char* password = "my_password";
</pre>

# Wiring and PCB 
see
* https://easyeda.com/hujer.roman/esp-01-sqm
* https://easyeda.com/hujer.roman/sqm-esp-v2
