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
bool flagExpectMessage = false;
int timeRestartExpect;
char charBuff[40] {""};     //буффер для приёма/правки по serial

//-------serial-----
  #include <AsyncStream.h>
  AsyncStream<40> serial(&Serial,';'); 
  #include <GParser.h>
  GParser parser(serial.buf, ',');


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
  delay(1000);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  timeClient.begin();  
  timeClient.update();  
  SerialWrite(updateTime);  
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
    if(msg.text[0] == '/' || flagExpectMessage)       //если пришла управляющая команда
    {
      charBuff[0] = '/'; charBuff[2] = '\0';    //начинаем сообщение
      //открываем возможность для приёма сообщений без '/'
        if(!flagExpectMessage)
        {
          flagExpectMessage = true;       
          timeRestartExpect = millis();   //счётчик ожижания сообщения без '/'
        }

      //ищем совпадения
        if(msg.text.lastIndexOf("/tempDay") != -1)
        {
          writeCharInBuff('a');
          writeCharInBuff('a');
        }
    }
   Serial.write(msg.text.c_str(), msg.text.length()); //отправляем 
    if(flagExpectMessage && millis() - timeRestartExpect > 2000) 
    {
      flagExpectMessage = false;
    } 
  }

//bot.sendMessage("hello");

//отправка данных в порт
  void SerialWrite(void(*action)()){
    action();                      //выбрать какие данные
    strcat(charBuff, ";");         //добавляем термиатор
    Serial.write(charBuff, strlen(charBuff)); //отправляем  
  }
//чтение данных из Serial
  void SerialRead() {
   if (serial.available()) 
   {
    parser.split();   
    if(serial.buf[0] == 't')
      SerialWrite(updateTime);       
    }  
  } 
//добавление символа в буффер на отправку
  void writeCharInBuff(char value){
    int index = 0;
    while(charBuff[index++] != "\0")  //ищем конец буфера
    charBuff[index++] = value;
    charBuff[index] = '\0';
  }
//функция добавления числа в собщение на отправку
  void writeIntInBuff ( int value){
    strcat(charBuff, ",");  //дописываем разделитель в буфер
    char temp[4];
    itoa(value,temp, DEC);  //преобразовываем число в char
    strcat(charBuff ,temp); //дописываем число в буфер
  }
//запись времени в буфер
  void updateTime(){
    writeCharInBuff('t');
    writeIntInBuff(timeClient.getHours());
    writeIntInBuff(timeClient.getMinutes());
    writeIntInBuff(timeClient.getSeconds());
  }
//синхронизация времени с мегой в полночь
  void updateTimeMidnight(){
  //static bool updateTime = false;
  if (timeClient.getHours() == 0 && timeClient.getMinutes() == 0 &&
      timeClient.getSeconds() == 0 ) 
      {
        SerialWrite(updateTime);
        delay(1000);
      }  
  }

