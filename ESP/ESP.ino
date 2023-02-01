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

//-------serial-----
  char charBuff[40] { ""};     //буффер для отправки по serial
  #include <AsyncStream.h>
  AsyncStream<40> serial(&Serial,';'); 
  #include <GParser.h>
  GParser parser(serial.buf, ',');


void setup() { 
  bot.setChatID(284342215);  
  bot.attach(newMsg);
  bot.sendMessage("hello");

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
  delay(30000);
  Serial.println("Ready");
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
  
}
//отправка данных в порт
void SerialWrite(void(*action)()){
  action();                      //выбрать какие данные
  strcat(charBuff, ";");         //добавляем термиатор
  Serial.write(charBuff, strlen(charBuff)); //отправляем  
}
//чтение данных
void SerialRead() {
  if (serial.available()) 
  {
    parser.split();   
    if(serial.buf[0] == 't')
      SerialWrite(updateTime);       
  }  
}
//запись времени в буфер
void updateTime(){
  writeCharInBuff('t');
  writeIntInBuff(timeClient.getHours());
  writeIntInBuff(timeClient.getMinutes());
  writeIntInBuff(timeClient.getSeconds());
}
//запись начального символа в буффер
void writeCharInBuff(char value){
  charBuff[0] = value;
  charBuff[1] = '\0';
}
//функция добавления числа в буфер
void writeIntInBuff ( int value){
  strcat(charBuff, ",");
  char temp[4];
  itoa(value,temp, DEC);
  strcat(charBuff ,temp);
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
