// GPS.ino
//
// Copyright (c) 2020 Roman Hujer   http://hujer.net
//

#ifdef GPS_ON

bool GPS_sync  = false;
bool GPS_wiring_OK = true;

byte  g_sat = 0;
float g_alt = 0;

int  g_year = 0;
byte g_month = 0;
byte g_day = 0;
byte g_hour = 0;
byte g_minute = 0;
byte g_second = 0;
float g_lat = 0;
float g_lng = 0;



void sqmGPS() {

  //read GPS data
  for (unsigned long start = millis(); millis() - start < 1000;)
    while (gpsSerial.available())
      if (gps.encode(gpsSerial.read())) ProcGPSData();

  if ( millis() > 5000 && gps.charsProcessed() < 10) {
    GPS_wiring_OK = false;
    GPS_sync = false;
#ifdef DEBUG_ON
    Serial.println("No GPS detected: check wiring.");
#endif

  } else GPS_wiring_OK = true;

}
void ProcGPSData () {
  char _tmp[20];
  g_sat = 0;
  GPS_sync  = false;
  if (gps.satellites.isValid()) g_sat  = gps.satellites.value();
  if (gps.altitude.isValid())   g_alt  = gps.altitude.meters();
  if (gps.location.isValid()) {
      g_lat    = gps.location.lat();
      g_lng    = gps.location.lng();
      GPS_sync  = true;
  }
  if (gps.date.isValid())  {
    g_year   = gps.date.year();
    g_month  = gps.date.month();
    g_day    = gps.date.day();
  }
  if (gps.time.isValid())  {
    g_hour   = gps.time.hour();
    g_minute = gps.time.minute();
    g_second = gps.time.second();
  }
#ifdef DEBUG_ON
  if ( GPS_sync ) {
    Serial.println("GPS sync:");
  } else {
    Serial.println("NOT sync:");
  }
  Serial.print(" SAT: " );
  Serial.print( g_sat  );
  Serial.print(" LAT: " );
  Serial.print( g_lat  );
  Serial.print(" LNG: " );
  Serial.print( g_lng );
  Serial.print(" ALT: " );
  Serial.println( g_alt );
  sprintf(_tmp, "%02d/%02d/%02d", g_day, g_month, g_year );
  Serial.print(" Date: " );
  Serial.println( _tmp );
  sprintf(_tmp, "%02d:%02d:%02d", g_hour, g_minute, g_second);
  Serial.print(" Time: " );
  Serial.println( _tmp );
  
#endif

} //  end of void ProcGPSData ()



int i_g_min ( float g_in)
{
  
  return int((g_in - int(g_in) * 1.) * 60. + 0.5);
}

int i_g_min_s ( float g_in)
{
  
  return int((g_in - int(g_in) * 1.) * 60. );
}

int i_g_sec_s ( float g_in)
{
  
  return int(((g_in - int(g_in) * 1.) * 3600. + 0.5 ) - int((g_in - int(g_in) * 1.) * 60.) * 60);
}

#endif
