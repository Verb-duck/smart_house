void ESP_parsing() {
  if (ESP_serial.available()) {     // если данные получены
  PRINT(ESP_serial.buf, " ");
    ESP_parser.split();
    char set = ESP_serial.buf[0];
    if (set == '/')
    { 
      char command = ESP_serial.buf[1];  
      switch(command)
      {
      case('a'):      
        if(ESP_serial.buf[3] == 'a') temperature_day = ESP_parser.getInt(1);
        if(ESP_serial.buf[3] == 'b') temperature_night = ESP_parser.getInt(1);
        if(ESP_serial.buf[3] == 'c') temperature_day_off  = ESP_parser.getInt(1);
        if(ESP_serial.buf[3] == 'd') temperature_sunrise = ESP_parser.getInt(1);
        if(ESP_serial.buf[3] == 'e') temperature_our_house = ESP_parser.getInt(1);
      case('t'):
        timeNow.hour = ESP_parser.getInt(1);
        timeNow.minute = ESP_parser.getInt(2); 
        timeNow.second = ESP_parser.getInt(3); 
        rtc.setTime(timeNow);  
        break;
      case('s'):
        script_house = SLEEP;
        break;
        case('m'):
        script_house = MOVE;
        break;
      case('o'):
        script_house = OUTSIDE_THE_HOME;
        break;
      default:
        break;
    }
  }
  }
  if(Serial.available()) {
    char type = Serial.read();
    PRINT( " serial out, char = ", type);
    Serial3.write(type);
  }
}

