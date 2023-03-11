//связь с esp 
 void serialReadEsp() {
  if (ESP_serial.available()) {    // если данные получены
  //  /^a^a^234^...^crc;   формат принимаемой строки 
    PRINT("new message ", ESP_serial.buf);    

  //обработка crc
    byte length = strlen(ESP_serial.buf);
    byte crc_mega = crc8_bytes((byte*)&ESP_serial, length-1);  //расчёт crc входящего сообщения без последнего байта
    byte crc_esp = (byte)(ESP_serial.buf[length-1]);   
    {
      static byte fail = 0;
      if(crc_mega != crc_esp) 
      {
        if(fail < 15)           //запрос на повторную отправку 
        {
          serialWriteEsp ("*");   
          fail++;
          PRINT( "return", fail);      
          return;
        }
        else                    //останавливаем попытки  
        {
          serialWriteEsp("#");  //fail
          PRINT( "fail", fail); 
          fail = 0;
          return;
        }
      } 
      else
      {
        fail = 0;
        PRINT( "OK" , ""); 
      }
    }
  //обработка сообщения
    char command1 = ESP_serial.buf[2]; 
    char command2 = ESP_serial.buf[4];
    byte am = ESP_parser.split();           //разделяeт строку на подстроки, заменяя разделители на NULL
    if (ESP_serial.buf[0] == '/') 
    { 
      switch(command1)
      {
      case('a'):          //temperature
        switch(command2)
        {
          case('a'):
            temperature_day = ESP_parser.getInt(3);
            break;
          case('b'):
            temperature_night = ESP_parser.getInt(3);
            break;
          case('c'):
            temperature_day_off = ESP_parser.getInt(3);
            break;
          case('d'):
            temperature_sunrise = ESP_parser.getInt(3);
            break;
          case('e'):
            temperature_our_house = ESP_parser.getInt(3);
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
        switch(command2)
        {
        case ('a'):
          timeNow.hour = ESP_parser.getInt(3);
          timeNow.minute = ESP_parser.getInt(4); 
          timeNow.second = ESP_parser.getInt(5); 
          rtc.setTime(timeNow);  
          break;
        case ('b'):
          alarmClockH = ESP_parser.getInt(3)/100;
          alarmClockM = ESP_parser.getInt(3)%100; 
          sunriseStartSet();
          work_alarm_clock = true;
          //отправим в чат сообщение
          char mess[33] = "the alarm will turn on in ";          
          char temp[2];
          itoa(alarmClockH.value, temp, DEC);
          strcat(mess,temp);
          strcat(mess,":");
          itoa(alarmClockM.value, temp, DEC);
          strcat(mess,temp);
          serialWriteEsp( mess);
          break;
        case ('c') :
          work_alarm_clock = false;
          serialWriteEsp( "alarm clock off");
          break;
        default:
          break;  
        }
        break;
      case('c'):          //script house
        switch(command2)
        {
          case('a'):
            script_house = MOVE;
            break;
          case('b'):
            script_house = SLEEP;
            serialWriteEsp( "sweet dreams");
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
 void serialWriteEsp (const char* mes) {
    Serial3.write(mes, strlen(mes));
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
    PRINT( "message nano", outBuffNano); 
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