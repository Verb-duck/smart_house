

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
  AsyncStream<40> ESP_serial(&Serial3,';',100); 
  GParser ESP_parser (ESP_serial.buf, '^');
  char outBuffEsp[20]; 
  bool repeatOutMessageEsp = false;
  byte lengthOutMessageEsp = 0; 
  //----nano-----
  AsyncStream<40> Nano_serial(&Serial2,';',100); 
  GParser Nano_parser (Nano_serial.buf, '^');
  char outBuffNano[20];  
  bool repeatOutMessageNano = false;
  byte lengthOutMessageNano = 0;
