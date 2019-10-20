#!/usr/bin/env python
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


class tty:
    def __init__(self, port = '/dev/ttyUSB0', baud=115200, timeout=2):
        try: 
            print ('Init serial port') 
            self.ser = serial.Serial( port, baud)
        except serial.SerialException as e:
            sys.stderr.write('ERROR: Could not open port: ' + port + '\n')
            sys.exit(1)     

    def open(self):
        self.ser.isOpen()

    def send(self, ss):
        print('Requet:', ss)
        self.ser.write(ss)

    def recv(self):
        return self.ser.readline()

    def close(self):
        self.ser.close()


class  MySQM:
    def __init__(self, port='/dev/ttyUSB0'):    
        self.sqm = tty(port); 
        self.sqm.open()

# Read box info 
    def read_device_info(self):    
        self.sqm.send('ix\r')
        return self.sqm.recv()

# Unitherdrom request - read sqm data 
    def read_sqm_data(self):
        self.sqm.send('rx\r')
        return self.sqm.recv()

# Unitherdrom request - read unaveraged data 
    def read_sqm_unaveraged(self):
        self.sqm.send('ux\r')
        return self.sqm.recv()

# Extension request - read data with weather informations
    def read_sqm_weather(self):
        self.sqm.send('wx\r')
        return self.sqm.recv()

# Read config data form EEPROM
    def read_config(self):
        self.sqm.send('gx\r')
        return self.sqm.recv()

# Read OLED status
    def read_oled_status(self):
        self.sqm.send('A5x\r')
        return self.sqm.recv()


