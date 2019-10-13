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
* Response: r, 10.28m,0000005915Hz,0000000002c,000000.200s, 026.2C

or
 
* Request: ux  
* Response: u, 10.33m,0000005915Hz,0000000004c,000000.200s, 026.4C

#### Read extension data with weather informations (is my extension)

* Request: wx  
* Response> w, 10.82m,0.02e,02577i,02209v, 026.7C,037h,0947p

#### Read calibratin data  (is my modication)

* Request: cx  
* Response: c, 0.01d,-0.5t
 
#### Write SQM calibration offset to EEPROM (is my modication)
Negative value: 
* Request:  zdx-0.05<enter>
* Response: zd,-0.05

Positive value:  
* Request:  zdx0.01<enter>
* Response: zd, 0.01 

#### Write Temperature offset to EEPROM (is my modication)
Negative value: 
* Request:  ztx-0.8<enter>
* Response: zt,-0.8

Positive value:  
* Request:  ztx0.5<enter>
* Response: zt, 0.8 

