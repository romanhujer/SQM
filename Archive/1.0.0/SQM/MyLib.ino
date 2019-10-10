// MyLib.ino
// Librarry function for SQM
// 
// Copyright (c) 2018 Roman Hujer   http://hujer.net
//


#ifdef BUZZER_ON 
void buzzer( int _long ) {
  tone(BuzzerPin, BuzzerTone );
  delay(_long);
  noTone(BuzzerPin);
}
#endif
