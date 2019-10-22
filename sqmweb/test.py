import os
import time
import logging 
import argparse
import mysqm
from bottle import route, run, template,static_file

#pkg_path, _  = os.path.split(os.path.abspath(__file__))
#iews_path = os.path.join(pkg_path, 'views')

views_path ='views' 

# default settings
WEB_HOST = '0.0.0.0'
WEB_PORT = 8080


sqm = mysqm.MySQM('/dev/ttyUSB1')





@route('/')
def main():    
#    return template('<p><p><center><h1> {{sqm_data}} </h1</center>', sqm_data=sqm.read_sqm_weather())
     return template( os.path.join(views_path, 'main.tpl') , sqm_data=sqm.read_sqm_weather())


@route('/static/<path:path>')
def callback(path):
    """Serve static files"""
    return static_file(path, root=views_path)


run(host=WEB_HOST, port=WEB_PORT)



