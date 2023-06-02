#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Arduino.h>

#define version "3.04 serial esp "
#define DEBUGING 1
 
#define ssid  "5G OBLUCHATEL"
#define passwordW "00000000"

//http time
#include <NTPClient.h>
const long utcOffsetInSeconds = 10800;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

#include <FastBot.h>
FastBot bot("6130227271:AAHMnTK8NFMRnXyOqVDUXeMPnWHVo__k3nI"); 
bool flagExpectMessage = false;     //expect message service
int timeRestartExpect;
char outBuff[20];             //buff save and send message
byte command;
bool repeatOutMessage = false;
byte lengthOutMessage = 0;

#include <SoftwareSerial.h>
SoftwareSerial mySerial(D3 , D4);   // RX, TX  
#include "AsyncStream.h"
#include "GParser.h"
//макросы
  #if (DEBUGING == 1)
  #define PRINT(title, y) \
    Serial.print(title); \
    Serial.print(": "); \
    Serial.println(y);
  #define LCDdebug "DEBUG"
  #else
  #define PRINT(titly, y)
  #define LCDdebug ""
  #endif
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
  UartSerial<50> serial(&mySerial);

//enum
  //пронумерованны первые, что бы не пересеkалась нумераця при отправке по uart 
  enum SCRIPT_HOUSE { 
    FIRST_SCRIP = 0, SCRIPT_HOUSE_NOW, DAY,  SLEEP, VIEWING_FILM, LEFT_HOME, NIGHT_MOVE , DAY_CAME_OUT, SUNRISE, SUNSET, LAST_SCRIP, 
  };

  enum MODE_LIGHT_BEDROM {   //нулевой не отправляет по сериал, FIRST_LIGHT как заглушка
    FIRST_LIGHT = 20, MODE_LIGHT_BEDROM_NOW, OFF_LIGHT,  START_LIGHT,  NORMAL_LIGHT,  BLACKOUT_LIGHT,
    NIGHT_LIGHT,  SUNRISE_LIGHT, COLOR_MUSIC, SUNSET_LIGHT,  LAST_LIGHT
  };

  enum {
    off, on,
  };

  enum SERIAL_CODE_BYTE {
    FIRST = 40,
    TEMPERATURE, TEMPERATURE_RAISE, TEMPERATURE_REDUCE,
    TIME, SINCHRONIZATION, ALARM, ALARM_OFF , HOUR, MINUTE, SECOND,
  };

void setup() { 
  PRINT("version " , version);
  Serial.begin(115200);
  mySerial.begin(115200);
  bot.setChatID(284342215);  
  bot.attach(newMsg);  
 //wifi connect
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, passwordW);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  ArduinoOTA.setPassword(passwordW);
  
  delay(2000);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  timeClient.begin();  
  timeClient.update(); 
  delay(10); 
 //updateTime mega
  serial.send<TIME ,SINCHRONIZATION> (timeClient.getHours(), timeClient.getMinutes(), timeClient.getSeconds()); 
  
}

void loop() {
  ArduinoOTA.handle();
  timeClient.update();    //getSeconds() getMinutes() getHours()
  bot.tick();                         //обработчик телеграмм бота  
  static uint16_t timer = 0;
  if(millis() - timer >= 20000)
  {
    timer += 20000;
    serial.send<TIME ,SINCHRONIZATION> (timeClient.getHours(), timeClient.getMinutes(), timeClient.getSeconds()); 
  
  } 

}
//функции-обработчика сообщений телеграмм бота 
  void newMsg(FB_msg& msg){
    if (msg.OTA) bot.update();
  //обработка и запись в буфер цифр из чата
    // lengthOutMessage = strlen(outBuff);
    // outBuff[lengthOutMessage++] = '^';
    // const char* inMessageBot = msg.text.c_str();
    // for (int i = 0; inMessageBot[i] != '\0' ; i++)    //пробегаем по сообщению
    // {	    
    //   if (inMessageBot[i] >= '0' && inMessageBot[i] <= '9') //выписываем все цифры в строку, 
    //     outBuff[lengthOutMessage++] = inMessageBot[i];      //обработка и разделение на стороне приёмника
    // }
    // flagExpectMessage = false;
  //записываем в буффер сообщение
    if(msg.text[0] == '/')      
    {      
      PRINT(" new message bot " , msg.text );
    //ищем совпадения и записываем в буфер команду
      //------temperature--------
        if(msg.text.lastIndexOf("/temperature_day") != -1)
        {
          startExpectMessage();
        }
        if(msg.text.lastIndexOf("/temperature_night") != -1)
        {
          startExpectMessage();
        }
        if(msg.text.lastIndexOf("/temperature_day_off") != -1)
        {
          startExpectMessage();
        }
        if(msg.text.lastIndexOf("/temperature_sunrise") != -1)
        {
          startExpectMessage();
        }
        if(msg.text.lastIndexOf("/temperature_our_house") != -1)
        {
          startExpectMessage();
        }
        if(msg.text.lastIndexOf("/temperature_raise") != -1)
        {
          serial.send<TEMPERATURE, TEMPERATURE_RAISE>();
        }
        if(msg.text.lastIndexOf("/temperature_reduce") != -1)
        {
          serial.send<TEMPERATURE, TEMPERATURE_REDUCE>();
        }
      //------mode house---------
        if(msg.text.lastIndexOf("/normal") != -1)
        { 
          serial.send<SCRIPT_HOUSE_NOW, DAY>();
        }
        if(msg.text.lastIndexOf("/sleep") != -1)
        { 
          serial.send<SCRIPT_HOUSE_NOW, SLEEP>();
        }
        if(msg.text.lastIndexOf("/leave_home") != -1)
        { 
          serial.send<SCRIPT_HOUSE_NOW, LEFT_HOME>();
        }
        if(msg.text.lastIndexOf("/vieving_film") != -1)
        { 
          serial.send<SCRIPT_HOUSE_NOW, VIEWING_FILM>();
        }
        if(msg.text.lastIndexOf("/return_home") != -1)
        { 
          serial.send<SCRIPT_HOUSE_NOW, DAY>();
        }
      //------time---------------
        if(msg.text.lastIndexOf("/time_alarm") != -1)
        { 
          startExpectMessage();
        }
        if(msg.text.lastIndexOf("/off_alarm") != -1)
        { 
          serial.send<TIME, ALARM_OFF>();
        }
    }  
  }


//открываем возможность для приёма сообщений из чата бота без '/'  
  void startExpectMessage() {
    flagExpectMessage = true;            //откладываем отправление, ждём данных
    timeRestartExpect = millis();        //запускаем счётчик ожидания сообщения без '/'
  }
//синхронизация времени с мегой в полночь
  void updateTimeMidnight(){
  //static bool updateTime = false;
  if (timeClient.getHours() == 0 && timeClient.getMinutes() == 0 &&
      timeClient.getSeconds() == 0 ) 
    {
      serial.send<TIME, SINCHRONIZATION>(0,0,0);
      delay(1000);   //задержка, что бы не было кучи обновлений времени в течении этой секунды при 00.00.00
    }  
  }
