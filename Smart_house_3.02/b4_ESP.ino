void ESP_parsing() {
  if (ESP_serial.available()) {     // если данные получены
    int am = data_esp.split(); 
    char type = data_esp[0];
    switch(type)
    {
      case('t'):
      now.second = setTime_second; now.minute = setTime_minute; now.hour = setTime_hour;
      rtc.setTime(now);  
      break;
      default:
      break;
    }
  }
}