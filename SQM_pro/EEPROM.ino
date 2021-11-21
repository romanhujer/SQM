// EEPROM.ino
// User specific configuration for SQM
// 
// Copyright (c) 2019 Roman Hujer   http://hujer.net
//

// EEPROM data position 
#define EEPROM_SQM_CAL_INDEX_C       1
#define EEPROM_SQM_CAL_INDEX_F       3
#define EEPROM_TEMP_CAL_INDEX_C      7
#define EEPROM_TEMP_CAL_INDEX_F      8 
#define EEPROM_AUTO_TEMP_INDEX_C     14
#define EEPROM_AUTO_CONTRAS_INDEX_C  15
#define EEPROM_CONTRAS_INDEX_C       17
#define EEPROM_CONTRAS_INDEX_B       20


// read SQMCalOffset from EEPROM
float ReadEESqmCalOffset(){
  float f;
  if ( EEPROM.read(EEPROM_SQM_CAL_INDEX_C) == 'M') {
    f = EEPROM_readFloat(EEPROM_SQM_CAL_INDEX_F);
  }
  else {
   f = SQM_CAL_OFFSET;  
  } 
  return f;
}

// Write SQMCalOffset to EEPROM
void WriteEESqmCalOffset( float f){
   if ( ( f > 25 )|| ( f < -25) ) return;  // value out of range 
   EEPROM.write(EEPROM_SQM_CAL_INDEX_C,'M');
   EEPROM_writeFloat(EEPROM_SQM_CAL_INDEX_F, f);
}


// Read Temperature Calibration offset from EEPROM
float ReadEETempCalOffset(){
  float f;
    if ( EEPROM.read(EEPROM_TEMP_CAL_INDEX_C) == 't' ) {
    f = EEPROM_readFloat(EEPROM_TEMP_CAL_INDEX_F);
  }
  else {
    f = TEMP_CAL_OFFSET;   
  }
  return f;
}

// Write Temperature Calibration offset to EEPROM
void WriteEETempCalOffset( float f) {
  if ( ( f > 50 )|| ( f < -50) ) return;  // value out of range 
   EEPROM.write(EEPROM_TEMP_CAL_INDEX_C,'t'); 
   EEPROM_writeFloat(EEPROM_TEMP_CAL_INDEX_F, f);
}

// Read AutoContras
boolean ReadEEAutoContras() {
  if ( EEPROM.read(EEPROM_AUTO_CONTRAS_INDEX_C) == 'N' )  
     return false;
  else 
     return true;
}

// Write AutoContras
void WriteEEAutoContras( boolean _b ) {
  if ( _b )
    EEPROM.write(EEPROM_AUTO_CONTRAS_INDEX_C,'Y');
  else 
    EEPROM.write(EEPROM_AUTO_CONTRAS_INDEX_C,'N');
}

// Read AutoTemperature Calibratiosn
boolean ReadEEAutoTempCal() {
  if ( EEPROM.read(EEPROM_AUTO_TEMP_INDEX_C) == 'Y' )  
     return true;
  else 
     return false;
}

// Write 
void WriteEEAutoTempCal( boolean _b ) {
  if ( _b )
    EEPROM.write(EEPROM_AUTO_TEMP_INDEX_C,'Y');
  else 
    EEPROM.write(EEPROM_AUTO_TEMP_INDEX_C,'N');
}

// Read Display contras from EEPROM
uint8_t ReadEEcontras(){
   uint8_t _f;
  if ( EEPROM.read(EEPROM_CONTRAS_INDEX_C) == 'C') {
    _f = EEPROM.read(EEPROM_CONTRAS_INDEX_B);
  }
  else {
   _f = DEFALUT_CONTRAS;  
  } 
  return _f;
}

// Write Display Contras to EEPROM
void WriteEEScontras( uint8_t _f){
   EEPROM.write(EEPROM_CONTRAS_INDEX_C,'C');
   EEPROM.write(EEPROM_CONTRAS_INDEX_B, _f);
}


// write 4 byte variable into EEPROM at position i (4 bytes)
void EEPROM_writeQuad(byte i,byte *v) {
  EEPROM.write(i+0,*v); v++;
  EEPROM.write(i+1,*v); v++;
  EEPROM.write(i+2,*v); v++;
  EEPROM.write(i+3,*v);
}

// read 4 byte variable from EEPROM at position i (4 bytes)
void EEPROM_readQuad(int i,byte *v) {
  *v=EEPROM.read(i+0); v++;
  *v=EEPROM.read(i+1); v++;
  *v=EEPROM.read(i+2); v++;
  *v=EEPROM.read(i+3);  
}

// write 4 byte float into EEPROM at position i (4 bytes)
void EEPROM_writeFloat(byte i,float f) {
  EEPROM_writeQuad(i,(byte*)&f);
}

// read 4 byte float from EEPROM at position i (4 bytes)
float EEPROM_readFloat(byte i) {
  float f;
  EEPROM_readQuad(i,(byte*)&f);
  return f;
}
