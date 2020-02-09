# SQM
Arduino compatible Sky Quality Meter using the TSL2591
   
  base in https://github.com/gshau/SQM_TSL2591/
 
  and BME280 weather sensor 
  
  and 128x64 OLED display 0.96" (SSD1306) or 1.3" (SH1106)

  Wiring diagram a PCB  on   https://easyeda.com/hujer.roman/sqm-hr


## Features

### USB Control mode use derived Unihedron serial protokol

#### Box info (Original Unihedron protokol)
* Request: ix 
* Response:  i,00000002,00000003,00000001,20191012

#### Read data  (Original Unihedron protokol)
* Request: rx  
* Response: r, 10.28m,0000000000Hz,0000000002c,000005.000s, 026.2C
or
* Request: ux  
* Response: u, 10.33m,0000000000Hz,0000000004c,000005.000s, 026.4C

#### Read extension data with weather informations (is my extension)
* Request: wx  
* Response: w, 11.87m,0.04e,00121i,00834v,0000000002c,A5,11,040h,0952p, 026.1C
 
#### Read config data  
* Request:  gx
* Response: g, 000.00m, 000.0C,TC:Y,A5,11,DC:0
 
#### Write SQM offset to EEPROM value range (-25m ... 25m)
Negative value: 
* Request:  zcal1-0.05x
* Response: z,1,-00.05m

Positive value:  
* Request:  zcal100.01x
* Response: z,1, 00.01m 

#### Write Temperature offset to EEPROM value range (-50°C ... 50°C)
Negative value: 
* Request:  zcal2-1.5x
* Response: z,2,-01.5C 

Positive value:  
* Request:  zcal2 00.5x
* Response: z,2, 00.5C 


#### Write default display contras to EEPROM value range (0-255) 
* Request:  zcal3 005x
* Response: z,3,005

#### Enabel SQM  temperature calibration 

* Request: zcalex
* Response: zxeaL 

#### Disable SQM  temperature calibration   (note lower case "d")

* Request:  zcaldx
* Response: zxdaL 

#### Erase EEPROM set to default value 

* Request: zcalDx
* Response: zxdL 

#### Oled display mode

* Request: A50x   Disable OLED display in USB mode
* Request: A51x   Enable OLED display in USB mode
* Request: A5dx   Disable auto dimmer (contras) for OLED display
* Request: A5ex   Enable auto dimmer (contras) for OLED display
* Request: A5x    Query for current OLED display mode

* Response: A5,xy 

  x... 0 - disable 1 - enable OLED Display in USB mode 
  
  y... 0 - disable 1 - enable auto Dimmer (contras) for OLED Display    


