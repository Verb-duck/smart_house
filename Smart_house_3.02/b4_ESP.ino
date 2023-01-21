void ESP_parsing() {
  if (ESP_serial.available()) {     // если данные получены
    data_esp.split(); 
    char type = data_esp[0];
    PRINT (" type: ", type);
    switch(type)
    {
      case('t'):
      DateTime timeSet;
      timeSet.hour = data_esp.getInt(1);
      timeSet.minute = data_esp.getInt(2); 
      timeSet.second = data_esp.getInt(3); 
      rtc.setTime(timeSet);  
      break;
      default:
      break;
    }
  }
}