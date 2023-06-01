

//-------- iremote-----------
 #include "IRLremote.h"
  CHashIR IRLremote;
  uint32_t IRdata;
  //
  //если удалить эти строки, не компелируется!!!!
  //хз
  //
//---------дисплей-----------
  #include <Wire.h>                       // библиотека для протокола I2C 
  #include <LiquidCrystal_I2C.h>          // подключаем библиотеку для LCD 1602
  LiquidCrystal_I2C LCD(0x27, 20, 4);     // присваиваем имя LCD для дисплея

//---------модуль времени----
  #include <microDS3231.h>
  MicroDS3231 rtc;                        // SDA A4 SCL A5  
  DateTime timeNow;
  auto alarmClockH(create(10, INT));
  auto alarmClockM(create(50, INT));
  DateTime sunriseStartTime;
  bool operator == (DateTime & t1 , DateTime &t2) {
    if (t1.minute == t2.minute && t1.hour == t2.hour) return true;
    return false;
  }
  int time_sunrise = TIME_SUNRISE * 60000 / 256; // вычесление периода добавления яркости рассвета

//---------btn---------------
  #include "Joystick.h"
  Joystick joystick(STIK_X_PIN, STIK_Y_PIN, STIK_KEY_PIN );

//---------heater------------
  uint8_t s1_addr_dimmer[] = {0x28, 0x60, 0xB9, 0x48, 0xF6, 0xE7, 0x3C, 0xC7};
  uint8_t s2_addr_dimmer[] = {0x28, 0x30, 0x6E, 0x48, 0xF6, 0x6, 0x3C, 0x57}; 
  Heater< HEATER_PIN_1,DC18B20_PIN, s1_addr_dimmer> heater_room_1 ;
  Heater< HEATER_PIN_2,DC18B20_PIN, s2_addr_dimmer> heater_room_2 ;
  
//---------dht11-------------
  #include "DHT.h"
  DHT dht(DHT_PIN, DHT11);
  
//---------fun---------------
  Fun_cleaner fun(CLEANER_FUN_PIN) ;
  
//---------Uart---------------  
  #include "AsyncStream.h"
  #include "GParser.h"
  //----esp-----
  //----nano-----
  AsyncStream<20> Nano_serial(&Serial2,';',100); 
  GParser Nano_parser (Nano_serial.buf, '^');
  char outBuffNano[20];  
  bool repeatOutMessageNano = false;
  byte lengthOutMessageNano = 0;

 template<int Size> 
  class UartSerial : public GParser {
    private:
    Stream* serial;
    AsyncStream<Size> *iStream;
    char outBuff[Size];             //buff save and send message
    byte lengthOut = 0;
    uint8_t attempt{0} , max_attempt{50};
    public:
    UartSerial( Stream* serial) : GParser(iStream->buf, '^') {
      this -> serial = serial;
      iStream = new AsyncStream<Size>(serial,';',50);   //приём сообщения
      GParser::buf = iStream->buf;                      //ещё раз,так как при созданнии объекта, этого буфера ещё не существует
    }
    bool read() {
      if (iStream -> available()) 
      {
        PRINT("incoming message: " , iStream->buf);
        if(iStream->buf[0] == 33 && iStream->buf[1] == 33 )         //пришла команда на повторную отправку сообщения, почему 33? да просто так
        {
          if(attempt <= max_attempt)      
          {
            attempt++;
            serial->write(outBuff, lengthOut);     //заново отправляем 
            PRINT("replay send message", "");
            return false;
          }
       //добавить возврат сообщения о неудаче
          else          //сообщение не отправленно                                   
          {
            attempt = 0;  
            PRINT("message didnt send", "");                     
            return false;            
          }
        } 
        if(iStream->buf[0] == 33 && iStream->buf[0] == 32 )      //добавить выдачу сообщения на вверх
        {
         PRINT("its ok, message recieve" , "" );
         return false;
        }
        byte length = strlen(iStream->buf);        
        byte crc = crc8_bytes((byte*)iStream,length);
        if(crc != 0 )                                     //проверяем crc
        {                           
          if(attempt <= max_attempt)
          {
            attempt++;
            //отсылаем команду на повторную отправку сообщения
              serial->write(33);  serial->write(33);  serial->write(';');
            PRINT("problemm, attempt №" , attempt);
            return false;
          }
      //добавить возврат сообщения о неудаче
          else    //сообщение не полученно
          {
            attempt = 0;                       
            return false;            
          }

        }
        split();
        //подтверждаем, что сообщение принято
          serial->write(33);  serial->write(32);  serial->write(';'); 
        attempt = 0;              //сбрасываем счётчик попыток повторной отправки/приёма
        PRINT("message ok" , "");
        return true;
      }
      return false;
    }
    byte category () {
      return (byte)iStream->buf[0];
    }
    byte variable() {
      return (byte)iStream->buf[2];
    }
    bool getBool(int num) {
        return atol(str[num]);
    }
    void setMaxAttempt( uint8_t set) {        //максимальное колличетсво попыток повторной отправки сообщения
      max_attempt = set;
    }
    //отправка сообщения 
     template<byte category = 255, byte variable = 255>
     void send() {
      lengthOut = 0;  
      if(category != 255)
      {
        outBuff[lengthOut++] = category;  outBuff[lengthOut++] = '^';
      }
      if(variable != 255) 
      {
        outBuff[lengthOut++] = variable;  outBuff[lengthOut++] = '^';
      }
      byte crc = crc8_bytes((byte*)&outBuff,lengthOut);
      outBuff[lengthOut++] = crc;
      outBuff[lengthOut++] = ';';
      serial->write(outBuff, lengthOut);         //send messege  
      PRINT("message send: ", outBuff);
     }      
     template<byte category = 255, byte variable = 255, class type1>
     void send(type1 value1) {
      lengthOut = 0;  
      if(category != 255)
      {
        outBuff[lengthOut++] = category;  outBuff[lengthOut++] = '^';
      }
      if(variable != 255) 
      {
        outBuff[lengthOut++] = variable;  outBuff[lengthOut++] = '^';
      }
      write_value_buff(value1);
      byte crc = crc8_bytes((byte*)&outBuff,lengthOut);
      outBuff[lengthOut++] = crc;
      outBuff[lengthOut++] = ';';
      serial->write(outBuff, lengthOut);         //send messege  
      PRINT("message send: ", outBuff);
     }
     template<byte category = 255, byte variable = 255, class type1, class type2>
     void send(type1 value1, type2 value2) {
      lengthOut = 0;  
      if(category != 255)
      {
        outBuff[lengthOut++] = category;  outBuff[lengthOut++] = '^';
      }
      if(variable != 255) 
      {
        outBuff[lengthOut++] = variable;  outBuff[lengthOut++] = '^';
      }
      write_value_buff(value1);
      write_value_buff(value2);
      byte crc = crc8_bytes((byte*)&outBuff,lengthOut);
      outBuff[lengthOut++] = crc;
      outBuff[lengthOut++] = ';';
      serial->write(outBuff, lengthOut);         //send messege  
      PRINT("message send: ", outBuff);
     }
     template<byte category = 255, byte variable = 255, class type1, class type2, class type3>
     void send(type1 value1, type2 value2, type3 value3) {
      lengthOut = 0;  
      if(category != 255)
      {
        outBuff[lengthOut++] = category;  outBuff[lengthOut++] = '^';
      }
      if(variable != 255) 
      {
        outBuff[lengthOut++] = variable;  outBuff[lengthOut++] = '^';
      }
      write_value_buff(value1);
      write_value_buff(value2);
      write_value_buff(value3);
      byte crc = crc8_bytes((byte*)&outBuff,lengthOut);
      outBuff[lengthOut++] = crc;
      outBuff[lengthOut++] = ';';
      serial->write(outBuff, lengthOut);         //send messege  
      PRINT("message send: ", outBuff);
     }
    private:    
    //функции записи значения в буфер
      void write_value_buff (int value) {
        char temp[5];
        itoa(value,temp, DEC);  //преобразовываем число в char
        for(char* ptr(temp); *ptr != '\0'; ptr++)
          outBuff[lengthOut++] = *ptr;
        outBuff[lengthOut++] = '^';
      }
      void write_value_buff (double value) {
        char temp[20];
        dtostrf(value,19,3,temp);  //преобразовываем число в char
        for(char* ptr(temp); *ptr != '\0'; ptr++)
        {
          if(*ptr == ' ') 
            continue;
          outBuff[lengthOut++] = *ptr;
        }       
        outBuff[lengthOut++] = '^';
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
  };
  UartSerial<50> esp_uart(&Serial3);
  UartSerial<50> nano_uart(&Serial2);