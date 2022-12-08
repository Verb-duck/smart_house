void sunriseStartSet() {
  if(alarmTime.minute - TIME_SUNRISE > 0) {
    sunriseStartTime.minute = alarmTime.minute - TIME_SUNRISE;
    sunriseStartTime.hour = alarmTime.hour;
  }
  else {
    sunriseStartTime.minute = 60 + alarmTime.minute  - TIME_SUNRISE;
    if ( alarmTime.hour == 0) 
      sunriseStartTime.hour = 23;
    else
      sunriseStartTime.hour = alarmTime.hour - 1;
  } 
}
