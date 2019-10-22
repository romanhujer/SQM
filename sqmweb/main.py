#!/usr/bin/env python


import os
import time
import logging 
import argparse

import mysqm

from bottle import route, run, template, static_file

#pkg_path, _  = os.path.split(os.path.abspath(__file__))
#iews_path = os.path.join(pkg_path, 'views')

#views_path ='views' 

pkg_path, _ = os.path.split(os.path.abspath(__file__))
views_path = os.path.join(pkg_path, 'views')



# default settings
web_host = '0.0.0.0'
web_port = 8080


sqm = mysqm.MySQM('/dev/ttyUSB1')

parser = argparse.ArgumentParser(
    description='sqm web manager.'
    'a simple web application to manage sqm')

parser.add_argument('--port', '-p', type=int, default=web_port,
                    help='web server port (default: %d)' % WEB_PORT)
parser.add_argument('--host', '-h', default=web_host,
                    help='bind web server to this interface (default: %s)' % WEB_HOST)
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
    return template( os.path.join(views_path, 'main.tpl') , sqm_data=sqm.read_sqm_weather())


#run(host=WEB_HOST, port=WEB_PORT)


###############################################################################
# Startup standalone server
###############################################################################

def main():
    """Start autostart profile if any"""

    run(app, host=args.host, port=args.port, quiet=args.verbose)
    logging.info("Exiting")


if __name__ == '__init__' or __name__ == '__main__':
    main()

