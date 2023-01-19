#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Arduino.h>

#define ssid  "5G OBLUCHATEL"
#define passwordW "00000000"
//URL request
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);

//http time
#include <NTPClient.h>
const long utcOffsetInSeconds = 7200;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

char type ('r');

String buff;                //string буффер для отправки по serial
char charBuff[30];
#include <AsyncStream.h>
AsyncStream<30> serial(&Serial, ';'); 
#include <GParser.h>

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, passwordW);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  timeClient.begin();  
 
  buff.reserve(30);     //string буффер для отправки по serial
  GParser parser(charBuff);

  server.onNotFound(handle_NotFound);  //404
  server.begin();

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
}

void loop() {
  ArduinoOTA.handle();
  server.handleClient();
  timeClient.update();    //getSeconds() getMinutes() getHours()
  SerialWrite();
  SerialRead();

  if (serial.available()) 
  {
    type = Serial.read();
  }  
}
void updateTime() 
{
    buff += "t,";
    buff += timeClient.getSeconds();
    buff += ',';
    buff += timeClient.getMinutes();
    buff += ',';
    buff += timeClient.getHours();
    buff += ';';
}
void SerialWrite() {
  switch (type){

    case 't':
    updateTime();
    Serial.println(buff);
    type = 'a';
    break;

    default:
    break;
  }
}

void SerialRead() 
{
  if (type == 't')
  {
    buff.toCharArray(charBuff,30);
  }
}
//URL 404 
void handle_NotFound()
{
  server.send(404, "text/plain", "Not found");
}
