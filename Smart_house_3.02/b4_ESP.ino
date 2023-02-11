void ESP_parsing() {
  if (ESP_serial.available()) {    // если данные получены
    //  /,a,a,234,6;   формат принимаемой строки      
    PRINT("new message ", ESP_serial.buf);
    char command1 = ESP_serial.buf[2]; 
    char command2 = ESP_serial.buf[4]; 
    ESP_parser.split();           //разделяeт строку на подстроки, заменяя разделители на NULL
    if (ESP_serial.buf[0] == '/') 
    { 
      switch(command1)
      {
      case('a'):          //temperature
        switch(command2)
        {
          case('a'):
            temperature_day = ESP_parser.getFloat(3) * 10;
            break;
          case('b'):
            temperature_night = ESP_parser.getFloat(3) * 10;
            break;
          case('c'):
            temperature_day_off  = ESP_parser.getFloat(3) * 10;
            break;
          case('d'):
            temperature_sunrise = ESP_parser.getFloat(3) * 10;
            break;
          case('e'):
            temperature_our_house = ESP_parser.getFloat(3) * 10;
            break;
          case('f'):
            temperature_day += 5;
            break;
          case('g'):
            temperature_day -= 5;
            break;  
          default:
            break;
        }        
      case('b'):          //time, alarm clock
        if(command2 == 'a')
        {
          timeNow.hour = ESP_parser.getInt(3);
          timeNow.minute = ESP_parser.getInt(4); 
          timeNow.second = ESP_parser.getInt(5); 
          rtc.setTime(timeNow);  
        }
        if (command2 == 'b')
        {
          alarmClockH = ESP_parser.getInt(3);
          alarmClockM = ESP_parser.getInt(4); 
          sunriseStartSet();
          work_alarm_clock = true;
        }
        if (command2 == 'c')
          work_alarm_clock = false;
        break;
      case('c'):          //script house
        switch(command2)
        {
          case('a'):
            script_house = MOVE;
            break;
          case('b'):
            script_house = SLEEP;
            break;
          case('c'):
            script_house = OUTSIDE_THE_HOME;
            break;
          case('d'):
            script_house = VIEWING_FILM;
            break;
          case('e'):
            script_house = NIGHT_MOVE;
            break;
          case('f'):
            script_house = NO_MOVE;
            break;
          default:
            break;
        } 
        break;
      case('d'):
        break;
      default:
        break;
      }
    }
  }  
}

