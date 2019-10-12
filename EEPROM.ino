// EEPROM.ino
// User specific configuration for SQM
// 
// Copyright (c) 2019 Roman Hujer   http://hujer.net
//


#ifdef CALIBRATION_ON 

// read SQMCalOffset from EEPROM
float ReadEESqmCalOffset(){
  float f;
  if ( EEPROM.read(1) == 'd') {
    f = EEPROM_readFloat(2);
  }
  else {
   f = SQM_CAL_OFFSET;  
  } 
  return f;
}

// read Temperature Calibration offset from EEPROM
float ReadEETempCalOffset(){
  float f;
    if ( EEPROM.read(6) == 't' ) {
    f = EEPROM_readFloat(7);
  }
  else {
    f = TEMP_CAL_OFFSET;   
  }
  return f;
}
void WriteEESqmCalOffset( float f){
   EEPROM.write(1,'d');
   EEPROM_writeFloat(2, f);
}

void WriteEETempCalOffset( float f) {
   EEPROM.write(6,'t');
   EEPROM_writeFloat(7, f);
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
#endif
