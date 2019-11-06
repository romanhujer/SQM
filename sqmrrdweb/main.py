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

#pkg_path, _ = os.path.split(os.path.abspath(__file__))
#views_path = os.path.join(pkg_path, 'views')



# default settings

WEB_HOST = '0.0.0.0'
WEB_PORT = 8080
COM_PORT = '/dev/ttyUSB0'


debug = 1

lock_serial_port = 0
rrd_database='data/sqm.rrd'

parser = argparse.ArgumentParser(
    description='SQM web manager.'
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
parser.add_argument('--server', '-s', default='standalone',
                    help='http server [standalone|apache] (default: standalone')

args = parser.parse_args()

logging.debug("command line arguments: " + str(vars(args)))


if args.server == 'standalone':
    app = Bottle()
    logging.info('using Bottle as standalone server')
else:
    app = default_app()
    logging.info('using Apache web server')

#
serial_port = args.com



# 
#  Create new rrd database
#
def new_rrd_database():
    sqmr.create_database()


class sqmThread(threading.Thread):
  def __init__(self, *args, **keywords): 
    threading.Thread.__init__(self, *args, **keywords) 
    self.killed = False
  
  def start(self): 
    self.__run_backup = self.run 
    self.run = self.__run       
    threading.Thread.start(self) 
  
  def __run(self): 
    sys.settrace(self.globaltrace) 
    self.__run_backup() 
    self.run = self.__run_backup 
  
  def globaltrace(self, frame, event, arg): 
    if event == 'call': 
      return self.localtrace 
    else: 
      return None
  
  def localtrace(self, frame, event, arg): 
    if self.killed: 
      if event == 'line': 
        raise SystemExit() 
    return self.localtrace 
  
  def kill(self): 
    self.killed = True


def sqm_daemon():
    while True :
       if sqmr.sqm.open_ser == 1 :
            if  sqmr.lock_serial == 1 : 
                if sqmr.debug == 1:
                    print "sqm_daemon() serial port is lock"
                    time.sleep(1)
                continue
            sqmr.read_sqm_current_data()
            time.sleep(15)
       else:
            print "sqm_daemon() wiat serial port open" 
            time.sleep(1)

#
# Init MySQM class defaut is com open and debug off
#
sqm = mysqm.MySQM(port=serial_port,debug=debug) 
#
# Init MySQMMrrd class
#
sqmr = sqmrrd.MySQMrrd(port=serial_port, database=rrd_database, debug=debug )

if  not os.path.exists(rrd_database) :
    print "Create new rrd database"
    new_rrd_database()



print "SQM daemon starting"  
# thread.start_new_thread( sqm_daemon,() )
sqmt = sqmThread(target=sqm_daemon)
sqmt.start()

print "OK"


#
# Web pages rendering
#
def init_page():
    return template( os.path.join(views_path, 'init.tpl'),
                     com  = serial_port
                   )


def main_page():
    if sqm.open_ser == 0: 
        return init_page()
    sqmr.generate_graph()   
    return template( os.path.join(views_path, 'main.tpl'), 
                        mpsas       = '%5.2f' % sqmr.mpsas, 
                        dmpsas      = '%4.2f' % sqmr.dmpsas,
                        ir          = '%04d' % sqmr.ir,
                        vis         = '%04d' % sqmr.vis, 
                        full        = '%04d' % sqmr.full, 
                        count       = '%d' % sqmr.count,
                        oled        = '%d' % sqmr.oled,
                        humidity    = '%3d' % sqmr.humidity, 
                        pressure    = '%4d' % sqmr.pressure,
                        temperature = '%4.1f' % sqmr.temperature,
                        devpoint    = '%4.1f' % sqmr.devpoint 
                    )
 
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
    while sqmr.lock_serial == 1 :
        if debug == 1 :
            print "info_page() serial port si lock"
            time.sleep(1)
    sqmr.lock_serial = 1
    s = sqm.read_device_info().split(',')
    sqmr.lock_serial = 0
    protokol = long(s[1])
    model    = long(s[2])
    feature  = long(s[3])
    serial   = s[4].split('\r')[0]
    s = sqm.read_config().split(',')
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
    while sqmr.lock_serial == 1 :
        if debug == 1:
            print "config_page() serial is lock"
            time.sleep(1)
    sqmr.lock_serial = 1
    s = sqm.read_config().split(',')
    sqmr.lock_serial = 0
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
#        sqmt.kill()
        if sqmr.sqm.open_ser == 1:
           sqmr.stop_serial() 
        if sqm.open_ser == 1:
           sqm.close_serial() 
        print "Open serial"   
        sqm.open_serial(scom)
        sqmr.start_serial(scom)
#        print "start deamon"
#        sqmt.start()
        while not sqmr.first_data :
            if debug : 
                print "do_init() wait fist data" 
                time.sleep(1)
    return main_page()

@app.route('/')
@app.route('/main')
def main(): 
    """main page"""
    if sqm.open_ser == 1:
        return main_page()
    return init_page()

@app.route('/main', method='POST') 
def do_main():
    oled_off = int(request.forms.get('sled'))
    while sqmr.lock_serial == 1 :
       if debug == 1 :
         print "do_main() Serial is lock"
         time.sleep(1)     
    sqmr.lock_serial = 1
    if oled_off == 0:
      sqm.disable_oled()
    elif  oled_off == 1:
      sqm.enable_oled()
    sqmr.lock_serial = 0  
    return main_page()

@app.route('/info')
def info():
    """info page"""
    return info_page()

@app.route('/info', method='POST')
def do_info():
    """info page"""
    form_id=request.forms.get('id')
    while sqmr.lock_serial == 1 :
       if debug == 1 :
         print "do_info() Serial is lock"
         time.sleep(1)
    sqmr.lock_serial = 1      
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
    sqmr.lock_serial = 0
    return info_page()

 
@app.route('/config')
def config():
   """config page"""	
   return config_page()

 
@app.route('/config', method='POST')
def do_config():
    """config page"""
    form_id=request.forms.get('id')
    while sqmr.lock_serial == 1 :
        if debug == 1 :
            print "do_config() Serial is lock "
            time.sleep(1)
    sqmr.lock_serial = 1
    
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
    sqmr.lock_serial = 0
    return config_page()


###############################################################################
# Startup standalone server
###############################################################################

def main():
    """Start autostart profile if any"""
    run(app, host=args.host, port=args.port, quiet=args.verbose)
    logging.info("Exiting")


if __name__ == '__init__' or __name__ == '__main__':
    main()

