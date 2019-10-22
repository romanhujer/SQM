import os
import time
import logging 
import argparse
import math
import mysqm
from bottle import route, run, template,static_file

#pkg_path, _  = os.path.split(os.path.abspath(__file__))
#iews_path = os.path.join(pkg_path, 'views')

views_path ='views' 

# default settings
WEB_HOST = '0.0.0.0'
WEB_PORT = 8080


sqm = mysqm.MySQM('/dev/ttyUSB0')





@route('/')
def main():    
    s = sqm.read_sqm_weather().split(',')
    mpsas       =  float(s[1].split('m')[0])
    dmpsas      = float(s[2].split('e')[0])
    count       = long(s[5].split('c')[0])
    humidity    = int(s[6].split('h')[0])
    pressure    = int(s[7].split('p')[0])
    temperature = float(s[8].split('C')[0])
    devpoint    = round(243.04 * (math.log(humidity/100.0) +
                                ((17.625 * temperature)/(243.04 + temperature))) /
                      (17.625 - math.log(humidity/100.0) -
                                ((17.625 * temperature)/(243.04 + temperature))),1)
    return template( os.path.join(views_path, 'main.tpl'), 
                     mpsas='%5.2f' % mpsas,
                     dmpsas='%4.2f' % dmpsas,
                     count='%d' % count,
                     humidity='%3d' % humidity,
                     pressure='%4d' % pressure,
                     temperature='%4.1f' % temperature,
                     devpoint='%4.1f' % devpoint )


@route('/static/<path:path>')
def callback(path):
    """Serve static files"""
    return static_file(path, root=views_path)


run(host=WEB_HOST, port=WEB_PORT)



