#!/usr/bin/env python
# -*- coding: utf-8 -*-

import math
import sqlite3
import mysqm    


class MySQMsql:
    def __init__(self, database='data/sqm.db', debug=False):
        self.database = database
        self.debug = debug
        

    def connect(self):
        self.sql =  sqlite3.connect(self.database, timeout=10)


    def create_table(self):
        cmd = '''CREATE TABLE IF NOT EXISTS  SQM_DATA(
                date_time  datetime PRIMARY KEY,
                mpsas     REAL, 
                humidity  REAL,  
                pressure  REAL,
                temperature REAL,
                dewpoint  REAL, 
                gps       TEXT
                ); '''
        cursor = self.sql.cursor()
        cursor.execute(cmd)
        self.sql.commit()
        cursor.close()

    def insert_data(self, mpsas, humidity, preasuere, temperature, dewpoint, gps='none'):
        cmd = '''INSERT INTO  SQM_DATA 
                       ( date_time,
                          mpsas,
                          humidity,
                          pressure,
                          temperature,
                          dewpoint,
                          gps
                        ) VALUES ( sysdate, %f, %f, %f , %f, %f, %s); 
                        ''' % ( mpsas, humidity, preasuere, temperature, dewpoint, gps ) 





