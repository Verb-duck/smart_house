bool pir_sensor () {
  return (digitalRead(PIR_SENSOR));  
}


uint8_t mic_sensor_clap() {
  //if (analogRead(SOUND_R) || analogRead(SOUND_R_FREQ) || analogRead(SOUND_L)) return 254;
  static uint8_t clap;
  static uint32_t timer1;
  static uint32_t timer2;
  int status_mic = digitalRead(MIC_SENSOR);
  if (status_mic) {
    if(!clap) {
      clap++;
      timer1 = timer2 = millis();
    }
    if( millis() - timer1 >=100 && clap) {
      clap++;
      timer1 = millis(); 
    }
  }
  if (millis() - timer2 >=700 && clap) {
    PRINT("\nclap" , clap);
    uint8_t clap1 = clap;
    clap = 0;
    return(clap1 );
    
  }
/*  else if {
    PRINT (" PROBLEM: ", " mic") ;
    return 0;
  } */
}
