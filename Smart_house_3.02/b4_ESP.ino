//связь с esp 
 void serialReadEsp() 
 {
  if(esp_uart.read())
  {
    switch(esp_uart.category())
    {
    case(TEMPERATURE):          
      switch(esp_uart.variable())
      {
        case(DAY):
          temperature_day = esp_uart.getInt(3);
          break;
        case(SLEEP):
          temperature_night = esp_uart.getInt(3);
          break;
        case(DAY_CAME_OUT):
          temperature_day_off = esp_uart.getInt(3);
          break;
        case(SUNRISE):
          temperature_sunrise = esp_uart.getInt(3);
          break;
        case(LEFT_HOME):
          temperature_left_home = esp_uart.getInt(3);
          break;
        case(TEMPERATURE_RAISE):
          temperature_day += 5;
          break;
        case(TEMPERATURE_REDUCE):
          temperature_day -= 5;
          break;  
        default:
          break;
      }        
    case(TIME):          //time, alarm clock
      switch(esp_uart.variable())
      {
        case (SINCHRONIZATION):
          timeNow.hour = esp_uart.getInt(3);
          timeNow.minute = esp_uart.getInt(4); 
          timeNow.second = esp_uart.getInt(5); 
          rtc.setTime(timeNow);  
          break;
        case (ALARM_CLOCK):
          alarmClockH = esp_uart.getInt(3);
          alarmClockM = esp_uart.getInt(3); 
          sunriseStartSet();                  //расчёт включения режима рассвет до будильника
          work_alarm_clock = true;
          break;
        case (ALARM_OFF) :
          work_alarm_clock = false;
          break;
        default:
          break;  
      }
    case(SCRIPT_HOUSE_NOW):          //script house
      switch(esp_uart.variable())
      {
        case(DAY):
          script_house = DAY;
          break;
        case(SLEEP):
          script_house = SLEEP;
          break;
        case(LEFT_HOME):
          script_house = LEFT_HOME;
          break;
        case(VIEWING_FILM):
          script_house = VIEWING_FILM;
          break;
        case(NIGHT_MOVE):
          script_house = NIGHT_MOVE;
          break;
        case(DAY_CAME_OUT):
          script_house = DAY_CAME_OUT;
          break;
        default:
          break;
      } 
    default:
      break;
    }    
  }
 }

  
 
//связь с нано
 void serialWriteNano (const uint8_t value, char name){
    lengthOutMessageNano = 0;     
    outBuffNano[lengthOutMessageNano++] = name;
    outBuffNano[lengthOutMessageNano++] = '^';
    outBuffNano[lengthOutMessageNano++] = value;
    outBuffNano[lengthOutMessageNano++] = '^';
    byte crc = crc8_bytes((byte*)&outBuffNano,lengthOutMessageNano);
    outBuffNano[lengthOutMessageNano++] = crc;
    outBuffNano[lengthOutMessageNano++] = ';';
    Serial2.write(outBuffNano, lengthOutMessageNano);     //send messege  
      Serial.print("message send nano = "); 
      Serial.print(outBuffNano[0]);
      Serial.print(" "); 
      Serial.println((byte)outBuffNano[2]);
  }
 void serialReadNano() {
   if (Nano_serial.available()) 
   {
     char command = Nano_serial.buf[0];
     switch (command) {
      //повтораная отправка
      case('*'):
        Serial2.write(outBuffNano, lengthOutMessageNano); 
        break;       
      default:
        break;
     }
   }   
  } 
// функция для расчёта crc
 byte crc8_bytes(byte *buffer, byte size) {
  byte crc = 0;
  for (byte i = 0; i < size; i++) {
    byte data = buffer[i];
    for (int j = 8; j > 0; j--) {
      crc = ((crc ^ data) & 1) ? (crc >> 1) ^ 0x8C : (crc >> 1);
      data >>= 1;
    }
  }
  return crc;
 }
//функция добавления числа в собщение на отправку
  void writeIntInBuff ( byte value, char* outBuff){
    strcat(outBuff, "^");  //дописываем разделитель в буфер
    char temp[4];
    itoa(value,temp, DEC);  //преобразовываем число в char
    strcat(outBuff ,temp); //дописываем число в буфер
  }
  void writeIntFloatBuff ( float value, char* outBuff){
    strcat(outBuff, "^");  //дописываем разделитель в буфер
    char temp[4];
    //dtostrf(value,temp, DEC);  //преобразовываем число в char
    strcat(outBuff ,temp); //дописываем число в буфер
  }