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
  tone(BuzzerPin, BuzzerTone );
  delay(_long);
  noTone(BuzzerPin);
}
#endif


void ReadWeather() 
{
  BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
  BME280::PresUnit presUnit(BME280::PresUnit_Pa);
  bme.read(pres, temp, hum, tempUnit, presUnit);
}


void DisplFirstPage( String &tsl, String &bme) {
OledDisp.firstPage();
  do {
    OledDisp.setContrast(0);
    OledDisp.setFont(u8g_font_unifont);
    OledDisp.setPrintPos(1, 15);
    OledDisp.print("SQM Ready V");
    OledDisp.print(Version);
    OledDisp.setPrintPos(1, 37);
    OledDisp.print(tsl);
    OledDisp.setPrintPos(1, 60);
    OledDisp.print(bme);
  } while ( OledDisp.nextPage() );
}

void DisplSqm(  double mpsas, double dmpsas, int temp, int hum , int pres, char blk) 
{
      OledDisp.firstPage();
      do {
#ifdef OLED_AUTO_CONTRAS_ON        
        if ( mpsas < 10.) {
             OledDisp.setContrast(150);
        } else if ( mpsas < 15.) {
             OledDisp.setContrast(50);
        } else { 
             OledDisp.setContrast(0); 
        }
#else 
        OledDisp.setContrast(0);        
#endif
        OledDisp.setFont(u8g_font_unifont);
        OledDisp.setPrintPos(1, 15);
        OledDisp.print("Mag/Arc-Sec T ");
        if (Blik) {
          OledDisp.print(blk);
        }
        OledDisp.setPrintPos(1, 37);
        OledDisp.print(mpsas);
        OledDisp.print(char(0xb1));
        OledDisp.print(dmpsas);
        OledDisp.print(" ");
        OledDisp.print( temp);
        OledDisp.print(char(0xb0));
        OledDisp.print("C");
        OledDisp.setPrintPos(1, 60);
        OledDisp.print("H:");
        OledDisp.print( hum);
        OledDisp.print("% P:");
        OledDisp.print(pres);
      } while ( OledDisp.nextPage());
     _blk_change_status();      
}


void DisplWaitUSB (char blk)
{
   OledDisp.firstPage();
      do {
        OledDisp.setContrast(0);
        OledDisp.setFont(u8g_font_unifont);
        OledDisp.setPrintPos(1, 37);
        OledDisp.print("Wait USB data ");      
        if (Blik) {
          OledDisp.print(blk);
        }
      } while ( OledDisp.nextPage());
      _blk_change_status();    
}
