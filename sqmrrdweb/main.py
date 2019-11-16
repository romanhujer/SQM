#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#   main.py   Sky Quality Meter web manager 
#
#   Version 1.0a
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

import os
import glob 
import time
import logging 
import argparse
import math
import sys 
import trace 
import threading 
import mysqm
import sqmrrd
import time 

from bottle import Bottle, route, run, template, static_file, get, post, request


views_path ='views' 

pkg_path = os.path.split(os.path.abspath(__file__))
print pkg_path
#views_path = os.path.join(pkg_path, 'views')



# default settings

WEB_HOST = '0.0.0.0'
WEB_PORT = 8080
COM_PORT = '/dev/ttyUSB0'

daemon_exit_flag = False
lock_serial_port = False
graph_start_time = '-1h' 

debug = True


rrd_database='data/sqm.rrd'

parser = argparse.ArgumentParser(description='SQM web manager.'
                                'A simple web application to manage SQM')

parser.add_argument('--port', '-P', type=int, default=WEB_PORT,
                    help='web server port (default: %d)' % WEB_PORT)
parser.add_argument('--host', '-H', default=WEB_HOST,
                    help='bind web server to this interface (default: %s)' % WEB_HOST)
parser.add_argument('--com', '-c', default=COM_PORT, 
                    help='SQM serial (USB) port (default: %s)' % COM_PORT)
parser.add_argument('--verbose', '-v', action='store_true',
                    help='print more messages')
parser.add_argument('--logfile', '-l', help='log file name')

args = parser.parse_args()

logging.debug("command line arguments: " + str(vars(args)))


app = Bottle()
logging.info('using Bottle as standalone server')

#
serial_port = args.com


# 
#  Define sqm Thread daemon
#
class sqmThread(threading.Thread):
  def __init__(self, name): 
    threading.Thread.__init__(self) 
    self.name = name
    daemon_exit_flag = False
  
  def run(self): 
    print "Thread Starting" + self.name
    sqm_daemon(self.name)
    print "Thread Stop" + self.name

  def stop(self): 
    daemon_exit_flag = True

def sqm_daemon(threadName): 
    daemon_exit_flag = False
    while not daemon_exit_flag:
      try:         
        if  myrrd.sqm.open_ser :
            if not  myrrd.lock_serial : 
                myrrd.read_sqm_current_data()
                time.sleep(15)
            else:   
                if myrrd.debug :
                     print "sqm_daemon() serial port is lock"
                     time.sleep(1)
 
        else:
            if myrrd.debug :
                print "sqm_daemon() wiat serial port open" 
                time.sleep(1)
      except:
        print "sqm_daemon() Serial port error"
        myrrd.sqm.open_ser = False
        myrrd.lock_serial = False
    threadName.exit()
     

#
# Init MySQM class defaut is com open and debug off
#
sqm = mysqm.MySQM(port=serial_port,debug=debug) 
#
# Init MySQMMrrd class
#
myrrd = sqmrrd.MySQMrrd(port=serial_port, database=rrd_database, debug=debug )

if  not os.path.exists(rrd_database) :
    print "Create new rrd database"
    myrrd.create_database()
else : 
    print "Use old rrd database"


print "SQM daemon starting"  
mydaemon = sqmThread(sqm_daemon)
mydaemon.start()
print "OK"


#
# Web pages rendering
#
def init_page():
    ports=glob.glob('/dev/ttyUSB*')
    return template( os.path.join(views_path, 'init.tpl'),
                     ports =  ports,
                     port = 'none'
                   )

#
# Main page
#
def main_page(start_time=graph_start_time):
    if not sqm.open_ser : 
        return init_page()
    myrrd.generate_graph(start=start_time)   
    return template( os.path.join(views_path, 'main.tpl'), 
                        mpsas       = '%5.2f' % myrrd.mpsas, 
                        dmpsas      = '%4.2f' % myrrd.dmpsas,
                        ir          = '%04d' % myrrd.ir,
                        vis         = '%04d' % myrrd.vis, 
                        full        = '%04d' % myrrd.full, 
                        count       = '%d' % myrrd.count,
                        oled        = '%d' % myrrd.oled,
                        humidity    = '%3d' % myrrd.humidity, 
                        pressure    = '%4d' % myrrd.pressure,
                        temperature = '%4.1f' % myrrd.temperature,
                        dewpoint    = '%4.1f' % myrrd.dewpoint,
                        start_time  = start_time 
                    )
                    

def long_time_page():
    myrrd.generate_graph(graf='views/sqm.png', start='-1h')
    myrrd.generate_graph(graf='views/day.png', start='-1d')
    myrrd.generate_graph(graf='views/week.png', start='-1w')
    myrrd.generate_graph(graf='views/monht.png', start='-1m')
    myrrd.generate_graph(graf='views/year.png', start='-1y')   
    return template( os.path.join(views_path, 'longtime.tpl'))                     
 
def info_page():
    def _on_off(c):
        if (c == 1 ):
            return 'On'
        else:
            return 'Off'

    def _yes_no(c):
        if (c == 'Y'):
            return 'Yes'
        else: 
            return 'No'
    while myrrd.lock_serial :
        if debug :
            print "info_page() serial port si lock"
            time.sleep(1)
    myrrd.lock_serial = True
    s = sqm.read_device_info().split(',')
    if debug:
        print s
    protokol = long(s[1])
    model    = long(s[2])
    feature  = long(s[3])
    serial   = s[4].split('\r')[0]
    s = sqm.read_config().split(',')
    if debug:
        print s
    myrrd.lock_serial = False
    m_offset = float(s[1].split('m')[0])
    t_offest = float(s[2].split('C')[0])
    tc       = s[3].split(':')[1]
    oled     = int(s[5][0:1])
    dimmer   = int(s[5][1:2])
    contras  = int(s[6].split(':')[1])  
    return template( os.path.join(views_path, 'info.tpl'),
                        protokol = protokol,
                        model    = model,
                        feature  = feature,
                        serial   = serial,
                        moffset  = '%6.2f' % m_offset,
                        toffset  = '%5.1f' % t_offest,
                        tc       = _yes_no(tc),
                        oled     = _on_off(oled),
                        dimmer   = _on_off(dimmer),
                        contras  = '%d' % contras
                    )


def config_page():
    def _on_off(c):
        if (c == 1 ):
            return 'On'
        else:
            return 'Off'
    
    def _yes_no(c):
        if (c == 'Y'):
            return 'Yes'
        else: 
            return 'No'
    while myrrd.lock_serial :
        if debug :
            print "config_page() serial is lock"
            time.sleep(1)
    myrrd.lock_serial = True
    s = sqm.read_config().split(',')
    myrrd.lock_serial = False
    m_offset = float(s[1].split('m')[0])
    t_offest = float(s[2].split('C')[0])
    tc       = s[3].split(':')[1]
    oled     = int(s[5][0:1])
    dimmer   = int(s[5][1:2])
    contras  = int(s[6].split(':')[1])
    return template( os.path.join(views_path, 'config.tpl'),
                        moffset  = m_offset,
                        moffset_s  = '%6.2f' % m_offset,
                        toffset  =  t_offest,
                        toffset_s  = '%5.1f' % t_offest,
                        tc       = _yes_no(tc),
                        oled     = _on_off(oled),
                        dimmer   = _on_off(dimmer),
                        contras  = '%d' % contras
                    )

#
# Web pages routing 
#

@app.route('/static/<path:path>')
def callback(path):
    """Serve static files"""
    return static_file(path, root=views_path)

@app.route('/favicon.ico', method='GET')
def get_favicon():
    """Serve favicon"""
    return static_file('favicon.ico', root=views_path)

@app.route('/init')
def init(): 
    """init page"""
    return init_page()

@app.route('/init', method='POST')
def do_init():
    """init page"""
    form_id=request.forms.get('id')
    if form_id == 'com':
        scom=request.forms.get('scom')
        print scom       
        if scom == 'none':
            return init_page()
        if myrrd.sqm.open_ser :
           myrrd.stop_serial() 
        if sqm.open_ser :
           sqm.close_serial() 
        print "Open serial"   
        sqm.open_serial(scom)
        myrrd.start_serial(scom)
        if not mydaemon.isAlive():
            print('thread killed')
            print "restart deamon"
            mydaemon.start()
        if not myrrd.first_data :
            if debug :
                print "do_init() wait fist data"
            return template( os.path.join(views_path, 'wait.tpl'))
        else:
            return main_page()


@app.route('/wait')
def wait_first_data():
    if not myrrd.first_data :
        if debug :
                print "do_init() wait fist data"
        return template( os.path.join(views_path, 'wait.tpl'))
    else: 
        return main_page()
 


@app.route('/')
@app.route('/main')
def main(): 
    """main page"""
    if sqm.open_ser : 
        if not myrrd.first_data :   
            if debug :
                print "main() wait fist data"
            return template( os.path.join(views_path, 'wait.tpl'))
        return main_page()
    return init_page()

@app.route('/main', method='POST') 
def do_main():
    form_id=request.forms.get('id')
    print form_id
    if form_id == 'oled':
        oled_off = int(request.forms.get('sled'))
        while myrrd.lock_serial :
            if debug :
                 print "do_main() Serial is lock"
                 time.sleep(1)     
        myrrd.lock_serial = True
        if oled_off == 0:
            sqm.disable_oled()
        elif  oled_off == 1:
            sqm.enable_oled()
        myrrd.lock_serial = False
        return main_page()
    if form_id == 'graph':
        graph_time=request.forms.get('graph')
        if graph_time == 'long':
            return long_time_page()
        else:
            return main_page(start_time = graph_time)
        

@app.route('/info')
def info():
    """info page"""
    return info_page()

@app.route('/info', method='POST')
def do_info():
    """info page"""
    form_id=request.forms.get('id')
    while myrrd.lock_serial :
       if debug :
         print "do_info() Serial is lock"
         time.sleep(1)
    myrrd.lock_serial = True     
    if form_id == 'oled':
        oled_off = int(request.forms.get('sled'))
        if oled_off == 0:
            sqm.disable_oled()
        elif  oled_off == 1:
            sqm.enable_oled()
    if form_id == 'dimmer': 
        dimmer_off=int(request.forms.get('sdimmer'))
        if dimmer_off == 0:
            sqm.disable_dimmer()
        elif dimmer_off == 1:
            sqm.enable_dimmer()
    if form_id == 'contras':
        sqm.set_oled_contras(int(request.forms.get('scontras')))
    myrrd.lock_serial = False 
    return info_page()

 
@app.route('/config')
def config():
   """config page"""	
   return config_page()

 
@app.route('/config', method='POST')
def do_config():
    """config page"""
    form_id=request.forms.get('id')
    while myrrd.lock_serial :
        if debug  :
            print "do_config() Serial is lock "
            time.sleep(1)
    myrrd.lock_serial = True
    
    if form_id == 'tc':
        tc_yes = int(request.forms.get('stc'))
        if tc_yes == 1:
            sqm.enable_sqm_temp_cal()
        elif tc_yes == 0: 
            sqm.disable_sqm_temp_cal()
    
    if form_id == 'moffset':
        sqm.set_sqm_offset(float(request.forms.get('smoffset')))

    if form_id == 'toffset':
        sqm.set_temp_offset(float(request.forms.get('stoffset')))
    
    if form_id == 'reset':
        if int(request.forms.get('sreset')) == 4826157:
             sqm.set_EEPROM_default_vaule()

    if form_id == 'clear':    
        if int(request.forms.get('sclear')) == 2165394:
             myrrd.create_database()
            
    myrrd.lock_serial = False
    return config_page()
    
@app.route('/offline')
@app.route('/longtimegraph')
def long_time():
   """long time page"""	
   return long_time_page()
    
    


###############################################################################
# Startup standalone server
###############################################################################

def main():
    """Start autostart profile if any"""
    run(app, host=args.host, port=args.port, quiet=args.verbose)
    logging.info("Exiting")


if __name__ == '__init__' or __name__ == '__main__':
    main()

