#!/usr/bin/python
#
#

import time 
import mysqm

sqm = mysqm.MySQM('/dev/ttyUSB0')

print('Read box info')
print( sqm.read_device_info() )
time.sleep(2)

print('Unitherdrom request - read sqm data')
print( sqm.read_sqm_data() )
time.sleep(2)

print ('Unitherdrom request - read unaveraged dat') 
print( sqm.read_sqm_unaveraged() )
time.sleep(2)

print ('Extension request - read data with weather informations') 
print ( sqm.read_sqm_weather())
time.sleep(2)

print ('Read config data form EEPROM')
print(sqm.read_config())
time.sleep(2)

print ('Read OLED status') 
print(sqm.read_oled_status())
time.sleep(2)






