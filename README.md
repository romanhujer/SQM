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
* Response: r, 10.28m,0000002591Hz,0000000002c,000005.000s, 026.2C

or
 
* Request: ux  
* Response: u, 10.33m,0000002591Hz,0000000004c,000005.000s, 026.4C

#### Read extension data with weather informations (is my extension)

* Request: wx  
* Response: w, 10.82m,0.02e,02577i,02209v, 026.7C,037h,0947p

#### Read calibratin data  (is my modication)

* Request: cx  
* Response: c,0000000002c 10.28m,0000002591Hz,0000000002c,000005.000s, 026.2C
 
#### Write SQM calibration offset to EEPROM (is my modication)
Negative value: 
* Request:  zcal5-0.05x
* Response: z,5,-000000.05m

Positive value:  
* Request:  zcal50.01x
* Response: z,5,0000000.01m 

#### Write Temperature offset to EEPROM (is my modication)
Negative value: 
* Request:  zcal6-0.5x
* Response: z,5,-00.5C 

Positive value:  
* Request:  zcal600.5x
* Response: z,5, 00.5C 

