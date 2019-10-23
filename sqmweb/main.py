#!/usr/bin/env python


import os
import time
import logging 
import argparse
import math

import mysqm


from bottle import Bottle, route, run, template, static_file


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



@app.route('/static/<path:path>')
def callback(path):
    """Serve static files"""
    return static_file(path, root=views_path)

@app.route('/favicon.ico', method='GET')
def get_favicon():
    """Serve favicon"""
    return static_file('favicon.ico', root=views_path)


@app.route('/')
def main(): 
    """main page"""
    s = sqm.read_sqm_weather().split(',')
    mpsas       = float(s[1].split('m')[0])
    dmpsas      = float(s[2].split('e')[0])
    ir          = int(s[3].split('i')[0]) 
    vis         = int(s[4].split('v')[0]) 
    full        = vis + ir
    count       = long(s[5].split('c')[0])
    pressure    = int(s[9].split('p')[0])
    humidity    = int(s[8].split('h')[0])
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
                        humidity    = '%3d' % humidity, 
                        pressure    = '%4d' % pressure,
                        temperature = '%4.1f' % temperature,
                        devpoint    = '%4.1f' % devpoint 
                    )

    
@app.route('/info')
def info():
    """info page"""	  
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


@app.route('/config')
def info():
   """config page"""	
   return template( os.path.join(views_path, 'config.tpl'))


###############################################################################
# Startup standalone server
###############################################################################

def main():
    """Start autostart profile if any"""
    run(app, host=args.host, port=args.port, quiet=args.verbose)
    logging.info("Exiting")


if __name__ == '__init__' or __name__ == '__main__':
    main()

