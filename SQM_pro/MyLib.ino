// MyLib.ino
// Librarry function for SQM
//
// Copyright (c) 2018 Roman Hujer   http://hujer.net
//

void _blk_change_status() {
  if (Blik) {
    Blik = false;
  } else  {
    Blik = true;
  }
} // end of _blk_change_status()


void buzzer( int _long ) {
  for (signed int _i = 0; _i < _long / 2; _i++) {
    digitalWrite(BuzzerPin, 1);
    delay(1);
    digitalWrite(BuzzerPin, 0);
    delay(1);
  }
} // end of buzzer( int _long )



void ReadWeather() {
  if ( bme.measure() ) {

    do {
      delay(100);
    } while (!bme.hasValue());
    pres = bme.getPressure();
    temp = bme.getTemperature();
    if ( Humidity) hum =  bme.getHumidity();
     else hum = 0;
    temp = temp + TempCalOffset;  //calll

  }
}

void DisplFirstPage( ) {
    OledDisp.setContrast(ReadEEcontras());
    if (page != 1) {
      OledDisp.clear();
      buzzer(200);
    }
    page=1;
    OledDisp.setCursor(0, 0);
    OledDisp.print("SQM Ready V");
    OledDisp.print(Version);
    OledDisp.setCursor(0, 2);
    OledDisp.print("SN: ");
    OledDisp.print(SERIAL_NUMBER);
    OledDisp.setCursor(0, 4);
    OledDisp.print(TSL_Msg);
    OledDisp.setCursor(0, 6);
    OledDisp.print(BME_Msg);
}

void DisplCalData () {
  OledDisp.setContrast(ReadEEcontras());
  if (page != 2) {
    OledDisp.clear();
    buzzer(200);
  } 
  page=2;
  OledDisp.setCursor(0, 0);
  OledDisp.print("Calibration data");
  OledDisp.setCursor(0, 2);
  OledDisp.print("SQ offset:");
  if (SqmCalOffset >= 0)  OledDisp.print(' ');
  OledDisp.print( String(SqmCalOffset,2));
  OledDisp.print('M');
  OledDisp.setCursor(0, 4);
  OledDisp.print("TE offset:");
  if (TempCalOffset >= 0) OledDisp.print(' ');
  OledDisp.print(String(TempCalOffset,1));
  OledDisp.print(char(0xb0));
  OledDisp.print('C');
  OledDisp.setCursor(0, 6);
  OledDisp.print("TC:");
  OledDisp.print((ReadEEAutoTempCal()) ? 'Y': 'M');
  OledDisp.print(" DMMR:" );
  if (ReadEEAutoContras())
    OledDisp.print("Auto" ); 
   else {
    uint8_t _c = ReadEEcontras();   
    if (_c <10) OledDisp.print(" 00");
    else if ( _c <100) OledDisp.print(" 0");
    OledDisp.print( _c );
   } 
}   


void DisplSqm(  double mpsas, double dmpsas, int temp, byte hum , int pres, char blk) {
  char _tmp[20];
  float _lat, _lng;
  if ( ReadEEAutoContras()) {
     if ( mpsas < 10) {
        OledDisp.setContrast(150);
     } else if ( mpsas < 15) {
        OledDisp.setContrast(50);
     } else {
        OledDisp.setContrast(0);
     }
   } 
    else {
      OledDisp.setContrast(ReadEEcontras());
   }
   if (page != 3) {
      OledDisp.clear();
  
      buzzer(200);
  
    }
    page=3;
    OledDisp.setCursor(0, 0);
    sprintf(_tmp, "%02d/%02d/%02d", g_year-2000, g_month, g_day);
    OledDisp.print(_tmp);
    OledDisp.print(" UT");
    sprintf(_tmp, "%02d:%02d", g_hour,  g_minute);
    OledDisp.print(_tmp);
    OledDisp.setCursor(0, 2);
    OledDisp.print('M');
    OledDisp.print(Blik ? blk : ' ' );
    if (mpsas < 10) OledDisp.print('0');
    OledDisp.print(mpsas);
    OledDisp.print("mas");
    OledDisp.print(char(0xb2));
    if ( ( ( temp < 0 ) ? -temp : temp) < 10) OledDisp.print(' ');
    if ( temp >= 0 )  OledDisp.print(' ');
    OledDisp.print( temp );
    OledDisp.print(char(0xb0));
    OledDisp.print('C');
    OledDisp.setCursor(0, 4);
    OledDisp.print("H:");
    if (hum < 10)  { OledDisp.print("  "); } else if  (hum < 100) {OledDisp.print(' ');}
    OledDisp.print( hum);
    OledDisp.print("% P:");
    pres=int((pres+g_alt /8.3)+0.5);  //Přepočet na hladinu moře
    if (pres < 1000) { OledDisp.print(' ');}
    OledDisp.print(pres);
    OledDisp.print("hPa");
    if (GPS_sync) {
    OledDisp.setCursor(0, 5);
    OledDisp.print("Alt:" );
      if (g_alt < 10)  
         { OledDisp.print("   ");} 
      else if  (g_alt < 100) 
         { OledDisp.print("  ");}  
      else if  (g_alt < 1000)
         { OledDisp.print(' ');}      
      OledDisp.print( int(g_alt+.5));
      OledDisp.print("m Sat:");    
      OledDisp.print( g_sat < 10 ? g_sat : 9 );
      OledDisp.setCursor(0, 6);
      OledDisp.print("Lat: ");
      _lat= g_lat < 0 ? - g_lat : g_lat;
      OledDisp.print( int(_lat) );
      OledDisp.print(char(0xb0));
      OledDisp.print( i_g_min_s(_lat) );
      OledDisp.print('\'');
      OledDisp.print( i_g_min_s(_lat) );
      OledDisp.print('\"');
      OledDisp.print( g_lat < 0 ? 'S' : 'N' );
      OledDisp.setCursor(0, 7);
      OledDisp.print("Lon: " );
      _lng = g_lng < 0 ? - g_lng : g_lng;
      OledDisp.print( int(_lng) );
      OledDisp.print(char(0xb0));
      OledDisp.print( i_g_min_s(_lng) );
      OledDisp.print('\'');
      OledDisp.print( i_g_min_s(_lng) );
      OledDisp.print('\"');  
      OledDisp.print( g_lng < 0 ? 'W' : 'E' );
    }  
    else {
      OledDisp.setCursor(0, 6);
      OledDisp.print("GPS not sync "); 
      
    }
    _blk_change_status();
 }

void DisplWait(char blk)  {
    OledDisp.setContrast(ReadEEcontras());
    if (page != 4) {
      OledDisp.clear();
      buzzer(200);
    }
    page=4;
    OledDisp.setCursor(0, 2);
    if ( blk == '#') 
       OledDisp.print("Wait first data");
    else 
       OledDisp.print("Wait USB data ");
    OledDisp.print(Blik ? blk : ' ' );
    _blk_change_status();
  float battery = int(analogRead(A0) / 1023.0 * 11 * 100 + 0.5) / 100.;
  OledDisp.setCursor(0, 4);
  OledDisp.print("Batery: " ); 
  OledDisp.print( String(battery,2));
  OledDisp.print("V" );  
  #ifdef WIFI_ON
  OledDisp.setCursor(0, 6);
  
  if (  WiFiConnected ) {
     OledDisp.print(WiFi.localIP());
  } 
  else {
     OledDisp.print("Not connect!" ); 
  }  
#endif

}
