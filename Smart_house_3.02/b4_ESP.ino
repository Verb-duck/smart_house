void ESP_parsing() {
  if (ESP_serial.available()) {     // если данные получены
  PRINT(ESP_serial.buf, " ");
    ESP_parser.split();
    char set = ESP_serial.buf[0];
    if (set == '/')
    { 
      char command = ESP_serial.buf[3];  
      switch(command)
      {
      case('a'):

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

