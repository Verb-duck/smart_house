#define DHT_PIN D4          //датчик dht11
//---------dht11-------------
  #include "DHT.h"
  DHT dht(DHT_PIN, DHT11);
  int32_t timer;

  #include <ESP8266WiFi.h>
  #include <PubSubClient.h>
  const char *ssid = "5G OBLUCHATEL"; // Имя вайфай точки доступа
const char *pass = "00000000"; // Пароль от точки доступа

const char *mqtt_server = "server"; // Имя сервера MQTT
const int mqtt_port = 11140; // Порт для подключения к серверу MQTT
const char *mqtt_user = "Login"; // Логи от сервер
const char *mqtt_pass = "Pass"; // Пароль от сервера

#define BUFFER_SIZE 100

bool LedState = false;
int tm=300;
float temp=0;

// Функция получения данных от сервера

void callback(const MQTT::Publish& pub)
{
Serial.print(pub.topic()); // выводим в сериал порт название топика
Serial.print(" => ");
Serial.print(pub.payload_string()); // выводим в сериал порт значение полученных данных

String payload = pub.payload_string();

if(String(pub.topic()) == "test/led") // проверяем из нужного ли нам топика пришли данные
{
int stled = payload.toInt(); // преобразуем полученные данные в тип integer
digitalWrite(5,stled); // включаем или выключаем светодиод в зависимоти от полученных значений данных
}
}

WiFiClient wclient;
PubSubClient client(wclient, mqtt_server, mqtt_port);

void setup() {
  Serial.begin(115200);
  //---------dht11----------
  pinMode(DHT_PIN, INPUT); 
  dht.begin();
  timer = millis();

}

void loop() {
  if(millis() - timer >= 10000)
  {
    timer += 10000;
    float temperature_now = dht.readTemperature()-2;
    Serial.print("temperature ");
    Serial.println(temperature_now);
  }
}
