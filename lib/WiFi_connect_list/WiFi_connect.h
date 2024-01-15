/*

*/

#ifndef WiFi_connect_h
#define WiFi_connect_h

#define DEBUGING 1

#include <Arduino.h>
#include <ESP8266WiFi.h>        //wifi
#include <ESP8266mDNS.h>        //wifi
#include <ArduinoOTA.h>         //ota
#include <ESP8266httpUpdate.h>  //ota http
#include <PubSubClient.h>       //mqtt
#include "mqtt_constant.h"      //топики, команды 
#define ssid "5G OBLUCHATEL"
#define passwordW "00000000"

//функции обратного вызова при подключении, при разрыве соединения
//WiFiEventHandler gotIpEventHandler, disconnectedEventHandler; 


  
class mqttObject {
protected:
	static WiFiClient wificlient;
	static PubSubClient mqttClient;
  static char* mqttClientId;            //уникальный ID устройства
  static char* mqttTopicDeviceStatus;   //топик для отображения состояния устройства
  static char* mqttTopicOta;            //топик для скачивания прошивки
  static char* mqttLocationDevice;      //расположения устройства в доме
	static bool mqtt_connected();
  mqttObject(const mqttObject&);                //запрещаем копирование
  mqttObject& operator = (const mqttObject&);   //запрещаем копирование
  mqttObject(){}                                 //запрещаем создание
  static void set_name (const char* name_device,const char* location_device); 

public:
  static mqttObject& generate(const char* name_device,const char* location_device);      //создаёт всего одну копию объекта
  static void mqtt_loop();
};
//void mqtt_callback(char *topic, byte *payload, unsigned int length);

class mqttModule : public mqttObject {
  
};
bool WiFi_connected();				//подключение к ВиФи
void begin_OTA_WiFi_to_IP();  //обновление по IP
void WiFi_scan();							//поиск сетей
bool WiFi_loop();             //должна крутиться в loop, происходит проверка/переподключение WiFi and mqtt


#endif