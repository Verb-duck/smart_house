#ifndef mqtt_constant_h
#define mqtt_constant_h

  #define mqttServer  "192.168.3.44"
  #define mqttPort 1883
  #define mqttUser "esp"
  #define mqttPass "8266"
  #define mqttHeadTopic "device/"
  #define mqttDeviceStatusOn "online"
  #define mqttDeviceStatusOff "offline"
  #define mqttRetained_0 false
  #define mqttRetained_1 true
  #define mqttQos_0 0
  #define mqttQos_1 1

#if (DEBUGING == 1)
  #define PRINT(title, y) \
    Serial.print(title); \
    Serial.print(": "); \
    Serial.println(y);
  #else 
  #define PRINT(title, y) 
  #endif

#endif