#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Arduino.h>

 
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
bool repeatOutMessage = false;
byte lengthOutMessage = 0;

//-------serial-----
  #include "AsyncStream.h"
  AsyncStream<40> Mega_serial(&Serial,';',100); 
  #include "GParser.h"
  GParser Mega_parser(Mega_serial.buf, '^');


void setup() { 
  bot.setChatID(284342215);  
  bot.attach(newMsg);  
  Serial.begin(115200);
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
  
//отправка времени на мегу
  delay(2000);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  timeClient.begin();  
  timeClient.update(); 
  delay(10); 
 //updateTime mega
  outBuff[0] = '\0';
  strcat(outBuff, "/^b^a"); 
  writeIntInBuff(timeClient.getHours());
  writeIntInBuff(timeClient.getMinutes());
  writeIntInBuff(timeClient.getSeconds());
  sendMessageMega();
}

void loop() {
  ArduinoOTA.handle();
  timeClient.update();    //getSeconds() getMinutes() getHours()
  bot.tick();             //обработчик телеграмм бота    
  SerialRead(); 
  updateTimeMidnight();   //синхронизация в полночь  
}
//функции-обработчика сообщений телеграмм бота 
  void newMsg(FB_msg& msg){
    if (msg.OTA) bot.update();
  //обработка и запись в буфер цифр из чата
    if (flagExpectMessage)    
    {
      lengthOutMessage = strlen(outBuff);
      outBuff[lengthOutMessage++] = '^';
      const char* inMessageBot = msg.text.c_str();
      for (int i = 0; inMessageBot[i] != '\0' ; i++)    //пробегаем по сообщению
      {	    
        if (inMessageBot[i] >= '0' && inMessageBot[i] <= '9') //выписываем все цифры в строку, 
          outBuff[lengthOutMessage++] = inMessageBot[i];      //обработка и разделение на стороне приёмника
      }
      flagExpectMessage = false;
    }
  //записываем в буффер сообщение
    if(msg.text[0] == '/')      
    {      
      outBuff[0] = '\0';                     //обнуляем буффер
    //ищем совпадения и записываем в буфер команду
      //------temperature--------
        if(msg.text.lastIndexOf("/temperature_day") != -1)
        {
          strcat(outBuff, "/^a^a");
          startExpectMessage();
        }
        if(msg.text.lastIndexOf("/temperature_night") != -1)
        {
          strcat(outBuff, "/^a^b");
          startExpectMessage();
        }
        if(msg.text.lastIndexOf("/temperature_day_off") != -1)
        {
          strcat(outBuff, "/^a^c");
          startExpectMessage();
        }
        if(msg.text.lastIndexOf("/temperature_sunrise") != -1)
        {
          strcat(outBuff, "/^a^d");
          startExpectMessage();
        }
        if(msg.text.lastIndexOf("/temperature_our_house") != -1)
        {
          strcat(outBuff, "/^a^e");
          startExpectMessage();
        }
        if(msg.text.lastIndexOf("/temperature_raise") != -1)
        {
          strcat(outBuff, "/^a^f");
        }
        if(msg.text.lastIndexOf("/temperature_reduce") != -1)
        {
          strcat(outBuff, "/^a^g");
        }
      //------mode house---------
        if(msg.text.lastIndexOf("/normal") != -1)
        { 
          strcat(outBuff, "/^c^a"); 
        }
        if(msg.text.lastIndexOf("/sleep") != -1)
        { 
          strcat(outBuff, "/^c^b"); 
        }
        if(msg.text.lastIndexOf("/leave_home") != -1)
        { 
          strcat(outBuff, "/^c^c"); 
        }
        if(msg.text.lastIndexOf("/vieving_film") != -1)
        { 
          strcat(outBuff, "/^c^d"); 
        }
        if(msg.text.lastIndexOf("/return_home") != -1)
        { 
          strcat(outBuff, "/^c^f"); 
        }
      //------time---------------
        //updateTime()  /^b^a  при перезагрузке и в полночь
        if(msg.text.lastIndexOf("/update_time") != -1)
        { 
          strcat(outBuff, "/^b^a"); 
          writeIntInBuff(timeClient.getHours());
          writeIntInBuff(timeClient.getMinutes());
          writeIntInBuff(timeClient.getSeconds());
        }
        if(msg.text.lastIndexOf("/time_alarm") != -1)
        { 
          strcat(outBuff, "/^b^b"); 
          startExpectMessage();
        }
        if(msg.text.lastIndexOf("/off_alarm") != -1)
        { 
          strcat(outBuff, "/^b^с"); 
        }
    }
  //отправка данных в порт
    if(!flagExpectMessage) 
    {
      sendMessageMega();
    } 
  //если на дождались, закрываем возможность для приёма сообщений без '/'    
    if(flagExpectMessage && millis() - timeRestartExpect > 5000)    
    {
      flagExpectMessage = false;       
      outBuff[0] = '\0';                //обнуляем буффер чтобы не отправлять неполное сообщение
    } 
  }
//отправка сообщения на мегу
  void sendMessageMega (){
    lengthOutMessage = strlen(outBuff);
    outBuff[lengthOutMessage++] = '^';
    byte crc = crc8_bytes((byte*)&outBuff,lengthOutMessage);
    outBuff[lengthOutMessage++] = crc;
    outBuff[lengthOutMessage++] = ';';
    Serial.write(outBuff, lengthOutMessage);     //send messege      
  }
//чтение данных от Mega
  void SerialRead() {
   if (Mega_serial.available()) 
   {
     char command = Mega_serial.buf[0];
     switch (command) {
      //повтораная отправка
      case('*'):
        Serial.write(outBuff, lengthOutMessage); 
        break;       
      case('#'):
        bot.sendMessage ("fail");
        break;
      default:
        bot.sendMessage(Mega_serial.buf);
        break;
     }
   }   
  } 
//функция добавления числа в собщение на отправку
  void writeIntInBuff ( byte value){
    strcat(outBuff, "^");  //дописываем разделитель в буфер
    char temp[4];
    itoa(value,temp, DEC);  //преобразовываем число в char
    strcat(outBuff ,temp); //дописываем число в буфер
  }
  void writeIntFloatBuff ( float value){
    strcat(outBuff, "^");  //дописываем разделитель в буфер
    char temp[4];
    //dtostrf(value,temp, DEC);  //преобразовываем число в char
    strcat(outBuff ,temp); //дописываем число в буфер
  }

//синхронизация времени с мегой в полночь
  void updateTimeMidnight(){
  //static bool updateTime = false;
  if (timeClient.getHours() == 0 && timeClient.getMinutes() == 0 &&
      timeClient.getSeconds() == 0 ) 
      {
        outBuff[0] = '\0';
        strcat(outBuff, "/^b^a"); 
        writeIntInBuff(timeClient.getHours());
        writeIntInBuff(timeClient.getMinutes());
        writeIntInBuff(timeClient.getSeconds());
        lengthOutMessage = strlen(outBuff);
        sendMessageMega();
        delay(1000);   //задержка, что бы не было кучи обновлений времени в течении этой секунды при 00.00.00
      }  
  }
//открываем возможность для приёма сообщений из чата бота без '/'  
  void startExpectMessage() {
    flagExpectMessage = true;            //откладываем отправление, ждём данных
    timeRestartExpect = millis();        //запускаем счётчик ожидания сообщения без '/'
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