void sunriseStartSet() {
  if(alarmClockM.value - TIME_SUNRISE > 0) {
    sunriseStartTime.minute = alarmClockM.value - TIME_SUNRISE;
    sunriseStartTime.hour = alarmClockH.value;
  }
  else {
    sunriseStartTime.minute = 60 + alarmClockM.value  - TIME_SUNRISE;
    if ( alarmClockH.value == 0) 
      sunriseStartTime.hour = 23;
    else
      sunriseStartTime.hour = alarmClockH.value - 1;
  } 
}
