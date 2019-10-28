#!/usr/bin/env python
#
#   main.py   Sky Quality Meter web manager 
#
#   Version 1.0
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

import mysqm


from bottle import Bottle, route, run, template, static_file, get, post, request


views_path ='views' 

#pkg_path, _ = os.path.split(os.path.abspath(__file__))
#views_path = os.path.join(pkg_path, 'views')



# default settings

WEB_HOST = '0.0.0.0'
WEB_PORT = 8080
COM_PORT = '/dev/ttyUSB0'


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

sqm = mysqm.MySQM(args.com) 



#
# Web pages rendering
#

def main_page():
    s = sqm.read_sqm_weather().split(',')
    mpsas       = float(s[1].split('m')[0])
    dmpsas      = float(s[2].split('e')[0])
    ir          = int(s[3].split('i')[0]) 
    vis         = int(s[4].split('v')[0]) 
    full        = vis + ir
    count       = long(s[5].split('c')[0])
    oled        = int(s[7][0:1])
    humidity    = int(s[8].split('h')[0])
    pressure    = int(s[9].split('p')[0])
    temperature = float(s[10].split('C')[0])
    devpoint    = round(243.04 * (math.log(humidity/100.0) +
                                ((17.625 * temperature)/(243.04 + temperature))) /
                       (17.625 - math.log(humidity/100.0) -
                                ((17.625 * temperature)/(243.04 + temperature))),1)

    return template( os.path.join(views_path, 'main.tpl'), 
                        mpsas       = '%5.2f' % mpsas, 
                        dmpsas      = '%4.2f' % dmpsas,
                        ir          = '%04d' % ir,
                        vis         = '%04d' % vis, 
                        full        = '%04d' % full, 
                        count       = '%d' % count,
                        oled        = '%d' % oled,
                        humidity    = '%3d' % humidity, 
                        pressure    = '%4d' % pressure,
                        temperature = '%4.1f' % temperature,
                        devpoint    = '%4.1f' % devpoint 
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
    s = sqm.read_device_info().split(',')
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
    s = sqm.read_config().split(',')
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

@app.route('/')
@app.route('/main')
def main(): 
    """main page"""
    return main_page()

@app.route('/main', method='POST') 
def do_main():
    oled_off = int(request.forms.get('sled'))
#    print oled_off
    if oled_off == 0:
      sqm.disable_oled()
    elif  oled_off == 1:
      sqm.enable_oled()
    return main_page()

@app.route('/info')
def info():
    """info page"""
    return info_page()

@app.route('/info', method='POST')
def do_info():
    """info page"""
    form_id=request.forms.get('id')

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
    return info_page()

 
@app.route('/config')
def config():
   """config page"""	
   return config_page()

 
@app.route('/config', method='POST')
def do_config():
    """config page"""
    form_id=request.forms.get('id')
#    print form_id
    if form_id == 'tc':
        tc_yes = int(request.forms.get('stc'))
#       print tc_yes
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

