#!/usr/bin/env python
# -*- coding: utf-8 -*-

import math
import rrdtool
import mysqm    


class MySQMrrd:
    def __init__(self, port='/dew/ttyUSB0', database='data/sqm.rrd', debug=False):
        self.database = database
        self.debug = debug
        self.first_data = False
        self.lock_serial = True
        self.sqm = mysqm.MySQM(port,False, debug) 
        self.lock_serial = False

# start serial port 
    def start_serial(self, port='/dew/ttyUSB0'): 
        if not self.sqm.open_ser : 
            self.lock_serial = True
            self.sqm.open_serial(port)
            self.lock_serial = False 
        else:
            print ('Serial is port alredy open!')

# stop serial port
    def stop_serial(self):
        if self.sqm.open_ser :  
            self.lock_serial = True
            self.sqm.close_serial()    
            self.lock_serial = False
        self.sqm.open_ser = False    

        

# Create Round Robin Database
    def create_database(self):
        rrdtool.create( self.database,
                '--start', 'now', 
	              '--step', '30',
	              'RRA:AVERAGE:0.5:1:1200', 
		            'DS:mpas:GAUGE:600:0:25',
		            'DS:hum:GAUGE:600:0:100',
		            'DS:temp:GAUGE:600:-50:100',
		            'DS:pres:GAUGE:600:500:2000',
		            'DS:dewp:GAUGE:600:-50:100'	
		 )

# Read current data
    def read_sqm_current_data(self):
        while  self.lock_serial :
            if self.debug :
                print "Serial is lock" 
        self.lock_serial = True
        s = self.sqm.read_sqm_weather().split(',')
        self.lock_serial = False
        if self.debug : 
             print s
        self.mpsas       = float(s[1].split('m')[0])
        self.dmpsas      = float(s[2].split('e')[0])
        self.ir          = int(s[3].split('i')[0])
        self.vis         = int(s[4].split('v')[0])
        self.full        = self.vis + self.ir
        self.count       = long(s[5].split('c')[0])
        self.oled        = int(s[7][0:1])
        self.humidity    = int(s[8].split('h')[0])
        self.pressure    = int(s[9].split('p')[0])
        self.temperature = float(s[10].split('C')[0])
        self.dewpoint    = round(243.04 * (math.log(self.humidity/100.0) +
                                ((17.625 * self.temperature)/(243.04 + self.temperature))) /
                            (17.625 - math.log(self.humidity/100.0) -
                                ((17.625 * self.temperature)/(243.04 + self.temperature))),1)
        self.first_data = True 

# Feed updates to the RRD
        c =  ( 'N' + ':%.2f' % self.mpsas + 
                    ':%d'    % self.humidity +
                    ':%.1f'  % self.temperature  +
                    ':%d'    % self.pressure  +
                    ':%.1f'  % self.dewpoint
             )
        if self.debug :
            print c 
        rrdtool.update( self.database, c )
#               
# Generate graph
    def generate_graph(self, graf='views/sqm.png', start='-1h'):
        print "generate_graph()" + start
        rrdtool.graph ( graf,   "--title=Sky Quality Graph",
                                "--start","%s" % start,  
                                "--vertical-label", "m/as² °C 100xhPa 10x%", 
                                "--width","400",
                                "--height","120",
                                "--color","BACK#26262A", 
                                "--color","CANVAS#262626", 
                                "--color","FONT#FFFFFF", 
                                "--rigid",
                                "DEF:mpas=%s:mpas:AVERAGE"  % self.database, 
                                "DEF:humi=%s:hum:AVERAGE"   % self.database, 
                                "DEF:temp=%s:temp:AVERAGE"  % self.database, 
                                "DEF:pres=%s:pres:AVERAGE"  % self.database, 
                                "DEF:dewp=%s:dewp:AVERAGE"  % self.database, 
                                "CDEF:p02=pres,100,/", 
                                "CDEF:h02=humi,10,/",
                                "AREA:dewp#8080D0:Dew p.°C",
                                "LINE1:mpas#FF0000:SQM  m/as²", 
                                "LINE1:temp#A000A0:Temp °C",
                                "LINE1:h02#0000FF:Humidity",
                                "LINE1:p02#00FF00:Press hPa",
                                "COMMENT:\n",
                                "GPRINT:dewp:LAST:Now\: %5.1lf\t",
                                "GPRINT:mpas:LAST:%2.2lf\t",
                                "GPRINT:temp:LAST:%5.1lf\t",
                                "GPRINT:humi:LAST:%6.0lf\t",
                                "GPRINT:pres:LAST:%8.0lf",
                                "COMMENT:\n",
                                "GPRINT:dewp:MAX:Max\: %5.1lf\t",
                                "GPRINT:mpas:MAX:%2.2lf\t",
                                "GPRINT:temp:MAX:%5.1lf\t",
                                "GPRINT:humi:MAX:%6.0lf\t",
                                "GPRINT:pres:MAX:%8.0lf",
                                "COMMENT:\n",
                                "GPRINT:dewp:MIN:Min\: %5.1lf\t",
                                "GPRINT:mpas:MIN:%2.2lf\t",
                                "GPRINT:temp:MIN:%5.1lf\t",
                                "GPRINT:humi:MIN:%6.0lf\t",
                                "GPRINT:pres:MIN:%8.0lf",
                                "COMMENT:\n"
                                )
                                
