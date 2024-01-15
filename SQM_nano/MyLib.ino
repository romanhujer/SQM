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
    OledDisp.print("Mag/ArcSec\xb2 T");
    OledDisp.print(Blik ? blk : ' ' );
    OledDisp.print(' ');
    OledDisp.setCursor(0, 3);
    if (mpsas < 10) OledDisp.print('0');
    OledDisp.print(mpsas);
 //   OledDisp.print(char(0xb1));
//    OledDisp.print(dmpsas);
    OledDisp.print(" M/as\xb2");
    if ( ( ( temp < 0 ) ? -temp : temp) < 10) OledDisp.print(' ');
    if ( temp >= 0 )  OledDisp.print(' ');
    OledDisp.print( temp );
    OledDisp.print(char(0xb0));
    OledDisp.print('C');
    OledDisp.setCursor(0, 5);
    OledDisp.print("H:");
    if (hum < 10)  { OledDisp.print("  "); } else if  (hum < 100) {OledDisp.print(' ');}
    OledDisp.print( hum);
    OledDisp.print("% P:");
    if (pres < 1000) { OledDisp.print(' ');}
    OledDisp.print(pres);
    OledDisp.print("hPa");
    _blk_change_status();
 }

void DisplWait(char blk)  {
    OledDisp.setContrast(ReadEEcontras());
    if (page != 4) {
      OledDisp.clear();
      buzzer(200);
    }
    page=4;
    OledDisp.setCursor(0, 3);
    if ( blk == '#') 
       OledDisp.print("Wait first data");
    else 
       OledDisp.print("Wait USB data ");
    OledDisp.print(Blik ? blk : ' ' );
    _blk_change_status();
}


void mySQM() {
  String gainString = "Max gain";
  delay(50); // display cooldown
  tsl.setGain(TSL2591_GAIN_HIGH);
  luminosity = tsl.getFullLuminosity();
  delay(50);
  luminosity = tsl.getFullLuminosity(); // Read twice so value can stabilize.
  ir = luminosity >> 16;
  full = luminosity & 0xFFFF;
  visible = full - ir;
  if (visible== 0xFFFF||ir==0xFFFF) {
    gainString = "High gain";
    tsl.setGain(TSL2591_GAIN_HIGH);
    gainscale = HIGHSCALE;
    luminosity = tsl.getFullLuminosity();
    delay(50);
    luminosity = tsl.getFullLuminosity();
    ir = luminosity >> 16;
    full = luminosity & 0xFFFF;
    visible = full - ir;
    if (visible == 0xFFFF || ir == 0xFFFF) { // look, dude. It's daylight at this point. Knock it off
      gainString = "Med gain";
      tsl.setGain(TSL2591_GAIN_MED);
      gainscale = MEDSCALE;
      luminosity = tsl.getFullLuminosity();
      delay(50);
      luminosity = tsl.getFullLuminosity();
      ir = luminosity >> 16;
      full = luminosity & 0xFFFF;
      visible = full - ir;
      if (visible == 0xFFFF || ir == 0xFFFF) { // ARE YOU ON THE SUN?
        gainString = "Low gain";
        tsl.setGain(TSL2591_GAIN_LOW);
        gainscale = LOWSCALE;
        luminosity = tsl.getFullLuminosity();
        delay(50);
        luminosity = tsl.getFullLuminosity();
        ir = luminosity >> 16;
        full = luminosity & 0xFFFF;
        visible = full - ir;

      }
    }
  }
  adjustedIR = (float)ir / gainscale;
  adjustedVisible = (float)visible / gainscale;
  mag = -1.085736205*log(.925925925 * pow(10,-5.)*adjustedVisible);
  if (isinf(mag)) {
    mag = 25.0;
  }
  mag = mag + ReadEESqmCalOffset();
  mag = float (int ( mag * 100 + 0.5) ) /100;
}
