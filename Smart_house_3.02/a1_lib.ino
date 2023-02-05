

//-------- iremote-----------
 #include "IRLremote.h"
  CHashIR IRLremote;
  uint32_t IRdata;

//---------лента 2811--------
  #include <FastLED.h>
  //-----leds_contur---
  #define LED_NUM 115   // количество светодиодов
  CRGB leds[LED_NUM];
  //----leds_centr----
  #define LED_NUM_CENTR 40
  CRGB leds_centr[LED_NUM_CENTR];
  
  #define COOLING  70    //длина пламени
  #define SPARKING 150   //искры
  bool gReverseDirection = false;
  #define FRAMES_PER_SECOND 60
 
  using ledShov = void(*[])();
  ledShov led_pattern = {Color, Cylon, rainbow, rainbow_With_Glitter, confetti, sinelon, juggle, bpm, color_Temperature };
  const char *led_pattern_name[] ={"color", "cylon", "rainbow", "rainbowWithGlitter", "confetti", "sinelon", "juggle", "bpm", "color temperature"};
  int8_t led_pattern_number = 0;
  uint8_t quantity_led_pattern = sizeof(led_pattern) / sizeof(led_pattern[0]);
  uint8_t gHue = 0; // rotating "base color" used by many of the patterns
  uint8_t sped_led_show = 50;
 //--color temperature pattern
  #define TEMPERATURE_1 Tungsten100W
  #define TEMPERATURE_2 OvercastSky
  // How many seconds to show each temperature before switching
  #define DISPLAYTIME 20
  // How many seconds to show black between switches
  #define BLACKTIME   3
  
//---------дисплей-----------
  #include <Wire.h>                       // библиотека для протокола I2C 
  #include <LiquidCrystal_I2C.h>          // подключаем библиотеку для LCD 1602
  LiquidCrystal_I2C LCD(0x27, 20, 4);     // присваиваем имя LCD для дисплея

//---------модуль времени----
  #include <microDS3231.h>
  MicroDS3231 rtc;                        // SDA A4 SCL A5  
  DateTime timeNow;
  auto alarmClockH(create(9, INT));
  auto alarmClockM(create(10, INT));
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
  
//---------ESP---------------  
  #include "AsyncStream.h"
  #include "GParser.h"
  AsyncStream<40> ESP_serial(&Serial3,';',100); 
  GParser ESP_parser (ESP_serial.buf, ',');


