#!/usr/bin/python
#
# test mysqm.py module 
#

import time 
import mysqm

sqm = mysqm.MySQM('/dev/ttyUSB0',1,1)
sqm.debug = 1

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

print('Disable OLED in USB mode')
print(sqm.disable_oled())
print('Sleep 5sec') 
time.sleep(5)

print('Enable OLED in USB mode')
print(sqm.enable_oled())
time.sleep(2)

print('Disable OLED dimmer')
print(sqm.disable_dimmer())
time.sleep(2)

print('set_oled_contras to 150') 
print(sqm.set_oled_contras(150))
time.sleep(5)

print('set_oled_contras  to 0')
print(sqm.set_oled_contras(0))
time.sleep(5)

print('Enable OLED dimmer')
print(sqm.enable_dimmer())
time.sleep(2)

print('Set SQM offset -1.05')
print(sqm.set_sqm_offset(-1.05))
print ('Read config')
print(sqm.read_config())
time.sleep(2)

print('Set temperature offset -2.1')
print(sqm.set_temp_offset(-2.1))
print ('Read config')
print(sqm.read_config())
time.sleep(2)

print('Disable SQM Temperature callibration')
print(sqm.disable_sqm_temp_cal())
print('Read config')
print(sqm.read_config())
print ('Read sqm data')
print (sqm.read_sqm_data())
time.sleep(2)


print('Enable temperature callibration')
print(sqm.enable_sqm_temp_cal())
print('Read config')
print(sqm.read_config())
print ('Read sqm data')
print (sqm.read_sqm_data())
time.sleep(2)

print('Set EEPROM to default value')
print(sqm.set_EEPROM_default_vaule())
print('Read config')
print(sqm.read_config())
time.sleep(2)

print('End test')

