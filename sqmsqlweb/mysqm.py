#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#   MySQM.py   Sky Quality Meter class lib
#
#   Copyright (c) 2019 Roman Hujer   http://hujer.net
#
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,ss
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#  Description:
#  Sky Quality Meter using the TSL2591
#
#  base on https://github.com/gshau/SQM_TSL2591/
#
#  and BME280 weather sensor
#
#  and 128x64 OLED I2C display 0.96" (SSD1306) or 1.3" (SH1106)
#
#  Wiring diagram a PCB  on   https://easyeda.com/hujer.roman/sqm-hr
#

import sys
import serial 

#
# Serial port operation
#
class tty:

    def __init__(self, port = '/dev/ttyUSB0', baud=115200, timeout=60):
        try: 
            print ('Init serial port') 
            self.ser = serial.Serial( port, baud)
            self.debug = False 
        except serial.SerialException as e:
            sys.stderr.write('ERROR: Could not open port: ' + port + '\n')
            sys.exit(1)     

    def open(self):
        self.ser.isOpen()


    def send(self, ss):
        if self.debug : 
            print('Request:', ss)
        self.ser.write(ss)

    def recv(self):
        return self.ser.readline()

    def close(self):
        self.ser.close()


# MySQM extension protokol
class  MySQM:

    def __init__(self, port='/dev/ttyUSB0', open=False, debug=False ) :
        self.debug = debug 
        self.open_ser = False
        if open :
            self.open_serial(port)

# Open serial port    
    def open_serial(self, port='/dev/ttyUSB0'):
        if not self.open_ser : 
            self.sqm = tty(port) 
            self.sqm.debug = self.debug
            self.sqm.open()
            self.open_ser = True
        else:
            print ('Serial is port alredy open!')

# Close serial port
    def close_serial(self): 
        if self.open_ser :
            self.sqm.close()
            self.open_ser = False
        self.open_ser = False
         
# Read box info 
    def read_device_info(self):    
        self.sqm.send('ix')
        return self.sqm.recv()

# Unitherdrom request - read sqm data 
    def read_sqm_data(self):
        self.sqm.send('rx')
        return self.sqm.recv()

# Unitherdrom request - read unaveraged data 
    def read_sqm_unaveraged(self):
        self.sqm.send('ux')
        return self.sqm.recv()

# Extension request - read data with weather informations
    def read_sqm_weather(self):
        self.sqm.send('wx')
        return self.sqm.recv()

# Read config data form EEPROM
    def read_config(self):
        self.sqm.send('gx')
        return self.sqm.recv()

# Read OLED status
    def read_oled_status(self):
        self.sqm.send('A5x')
        return self.sqm.recv()
        
# Disable OLED in usb mode
    def disable_oled(self):
        self.sqm.send('A50x')        
        return self.sqm.recv()
                                  
# Enable OLED in usb mode
    def enable_oled(self):
        self.sqm.send('A51x') 
        return self.sqm.recv()

# Disable OLED dimmer (Auto contras)
    def disable_dimmer(self):
        self.sqm.send('A5dx') 
        return self.sqm.recv()

# Enable OLED dimmer (Auto cotrass)
    def enable_dimmer(self):
        self.sqm.send('A5ex') 
        return self.sqm.recv()

# Set  oled contras 
    def set_oled_contras(self,c): 
        if c > 255 : 
            c = 255
        s = "zcal3%02d" % c
        self.sqm.send(s)
        return self.sqm.recv()

# Set sqm offset 
    def set_sqm_offset(self,c):
        s = "zcal1%07.3fx" % c
        self.sqm.send(s)
        return self.sqm.recv()

# Set Temperature offset
    def set_temp_offset(self,c):
        s = "zcal2%06.2fx" % c
        self.sqm.send(s)
        return self.sqm.recv()

# Disable SQM Temperature callibration
    def disable_sqm_temp_cal(self):
        self.sqm.send('zcaldx')
        return self.sqm.recv()

# Enabe SQM Temperature callibration
    def enable_sqm_temp_cal(self):
        self.sqm.send('zcalex')
        return self.sqm.recv()

# Set EEPROM to default value
    def set_EEPROM_default_vaule(self):
        self.sqm.send('zcalDx')
        return self.sqm.recv()


# Original Unihedrom protokol
class UnihedronSQM:
    def __init__(self, port='/dev/ttyUSB0', open=False,  debug=False):
        self.debug = debug

