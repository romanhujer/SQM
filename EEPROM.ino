// EEPROM.ino
// User specific configuration for SQM
// 
// Copyright (c) 2019 Roman Hujer   http://hujer.net
//


#ifdef CALIBRATION_ON 

// EEPROM data position 
#define EEPROM_SQM_CAL_INDEX_C 1
#define EEPROM_SQM_CAL_INDEX_F 2
#define EEPROM_TEMP_CAL_INDEX_C 6
#define EEPROM_TEMP_CAL_INDEX_F 7 



// read SQMCalOffset from EEPROM
float ReadEESqmCalOffset(){
  float f;
  if ( EEPROM.read(EEPROM_SQM_CAL_INDEX_C) == 'm') {
    f = EEPROM_readFloat(EEPROM_SQM_CAL_INDEX_F);
  }
  else {
   f = SQM_CAL_OFFSET;  
  } 
  return f;
}

// read Temperature Calibration offset from EEPROM
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
void WriteEESqmCalOffset( float f){
   EEPROM.write(EEPROM_SQM_CAL_INDEX_C,'m');
   EEPROM_writeFloat(EEPROM_SQM_CAL_INDEX_F, f);
}

void WriteEETempCalOffset( float f) {
   EEPROM.write(EEPROM_TEMP_CAL_INDEX_C,'t');
   EEPROM_writeFloat(EEPROM_TEMP_CAL_INDEX_F, f);
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
