// MyLib.ino
// Librarry function for SQM
//
// Copyright (c) 2018 Roman Hujer   http://hujer.net
//

void _blk_change_status()
{
  if (Blik) {
    Blik = false;
  } else  {
    Blik = true;
  }
}

#ifdef BUZZER_ON
void buzzer( int _long ) {
  for (signed int _i = 0; _i < _long / 2; _i++) {
    digitalWrite(BuzzerPin, 1);
    delay(1);
    digitalWrite(BuzzerPin, 0);
    delay(1);
  }
}
#endif

#ifdef USE_WEATHER_SENSOR_ON
void ReadWeather()
{
  if ( bme.measure() ) {

    do {
      delay(100);
    } while (!bme.hasValue());
    pres = bme.getPressure();
    temp = bme.getTemperature();
    hum =  bme.getHumidity();
  #ifdef CALIBRATION_ON
    temp = temp + TempCalOffset;
  #endif

  }
}
#endif

#ifdef USE_OLED_ON

void DisplFirstPage( String _tsl, String _bme) {
    OledDisp.setContrast(DEFALUT_CONTRAS);
  #ifdef USE_U8x8_ON
    if (page != 1) {OledDisp.clear();}
    page=1;
    OledDisp.setCursor(0, 0);
    OledDisp.print("SQM Ready V");
    OledDisp.print(Version);
    OledDisp.setCursor(0, 3);
    OledDisp.print(_tsl);
    OledDisp.setCursor(0, 5);
    OledDisp.print(_bme);
    //    OledDisp.refreshDisplay();
  #else
    OledDisp.firstPage();
    do {
    #ifdef USE_U8G2_ON
      OledDisp.setCursor(1, 15);
    #else
      OledDisp.setPrintPos(1, 15);
    #endif
      OledDisp.print("SQM Ready V");
      OledDisp.print(Version);
    #ifdef USE_U8G2_ON
      OledDisp.setCursor(1, 37); 
    #else
      OledDisp.setPrintPos(1, 37);
    #endif
      OledDisp.print(_tsl);
    #ifdef USE_U8G2_ON
      OledDisp.setCursor(1, 37);
    #else
      OledDisp.setPrintPos(1, 60);
    #endif
      OledDisp.print(_bme);
    } while ( OledDisp.nextPage() );
  #endif
}
#endif

void DisplSqm(  double mpsas, double dmpsas, signed char temp, byte hum , int pres, char blk) {
  
#ifdef USE_OLED_ON
  #ifdef OLED_AUTO_CONTRAS_ON
    if ( mpsas < 10.) {
      OledDisp.setContrast(150);
    } else if ( mpsas < 15.) {
      OledDisp.setContrast(50);
    } else {
      OledDisp.setContrast(0);
    }
  #else
    OledDisp.setContrast(DEFALUT_CONTRAS);
  #endif
  #ifdef USE_U8x8_ON
    if (page != 2) {OledDisp.clear();}
    page=2;
    OledDisp.setCursor(0, 0);
    OledDisp.print("Mag/Arc-Sec T ");
    OledDisp.print(Blik ? blk : ' ' );
    OledDisp.print(" ");
    OledDisp.setCursor(0, 3);
    if (mpsas < 10) OledDisp.print(" ");
    OledDisp.print(mpsas);
    OledDisp.print(char(0xb1));
    OledDisp.print(dmpsas);
    if (abs(temp) < 10) OledDisp.print(" ");
    OledDisp.print( (temp < 0) ? '-' : ' ' );
    OledDisp.print( temp);
    OledDisp.print(char(0xb0));
    OledDisp.print("C ");
    OledDisp.setCursor(0, 5);
    OledDisp.print("H:");
    if (hum < 10)  { OledDisp.print("  "); } else if  (hum < 100) {OledDisp.print(" ");}
    OledDisp.print( hum);
    OledDisp.print("%  P:");
    if (pres < 1000) { OledDisp.print(" ");}
    OledDisp.print(pres);
  #else
    OledDisp.firstPage();
    do {
    #ifdef USE_U8G2_ON
      OledDisp.setCursor(1, 15);
    #else
      OledDisp.setPrintPos(1, 15);
    #endif
      OledDisp.print("Mag/Arc-Sec T ");
      OledDisp.print(Blik ? blk : ' ' );
    #ifdef USE_U8G2_ON
      OledDisp.setCursor(1, 37);
    #else
      OledDisp.setPrintPos(1, 37);
    #endif
      OledDisp.print(mpsas);
      OledDisp.print(char(0xb1));
      OledDisp.print(dmpsas);
      OledDisp.print( (temp < 0) ? '-' : ' ' );
      OledDisp.print( abs(temp) );
      OledDisp.print(char(0xb0));
      OledDisp.print("C");
    #ifdef USE_U8G2_ON
      OledDisp.setCursor(1, 60);
    #else
      OledDisp.setPrintPos(1, 60); 
    #endif
      OledDisp.print("H:");
      OledDisp.print( hum);
      OledDisp.print("% P:");
      OledDisp.print(pres);
    } while ( OledDisp.nextPage());
  #endif
    _blk_change_status();
#else
    Serial.print("Mag/Arc-Sec:");
    Serial.print(mpsas);
    Serial.print(" ");  
    Serial.print(char(0xb1));
    Serial.print(dmpsas);
    Serial.print(" ");  
    Serial.print( temp );
    Serial.print(char(0xb0));
    Serial.print("C ");
    Serial.print("H:");
    Serial.print( hum);
    Serial.print("% P:");
    Serial.println(pres);
#endif
}

#ifdef USE_OLED_ON
void DisplWaitUSB (char blk)
  {
    OledDisp.setContrast(DEFALUT_CONTRAS);
  #ifdef USE_U8x8_ON
    if (page != 3) {OledDisp.clear();}
    page=3;
    OledDisp.setCursor(0, 2);
    OledDisp.print("Wait USB data ");
    OledDisp.print(Blik ? blk : ' ' );
  //  OledDisp.refreshDisplay();
  #else
    OledDisp.firstPage();
    do {
      OledDisp.setContrast(0);
    #ifdef USE_U8G2_ON
      OledDisp.setCursor(1, 37);
    #else
      OledDisp.setPrintPos(1, 37);
    #endif
      OledDisp.print("Wait USB data ");
      OledDisp.print(Blik ? blk : ' ' );
    } while ( OledDisp.nextPage());
  #endif
    _blk_change_status();
  }
#endif
