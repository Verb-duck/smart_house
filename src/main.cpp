#define ver "3.05 "
#define DEBUGING 1
#define key_EEPROM 33  // запись/сброс настроек в EEPROM при прошивке, сменить число
                       // свободная ячейка 19, незабудь добавить в setup в чтение памяти
//--------настройка--------
#define PERIOD_SENSOR_TEMPERATURE 1  //время опроса датчика температуры сек
#define TIME_SUNRISE 20              //время включения рассвета до будильника
#define TIME_OFF_NO_MOVE 5           //время ухода в спящий режим мин
#define TIME_REMOVE_MAINLCD 5        //время перехода на главный экран при бездействии сек
#define TIME_BACKLIGTH_LCD 20        //время выключения подсветки экрана при бездействии сек
#define TIME_SETUP 0                 // 0 , 1 - установить время == времени компиляции, 2 - установка вручную
  #define setTime_second 1
  #define setTime_minute 2
  #define setTime_hour 3
#define TEMPERATURE_DAY 22
#define TEMPERATURE_NIGHT 19

//---------пины--------
#define IR_PIN 18           //ИК приёмник
#define HEATER_PIN_2 8     //обогреватель 2, половина возле окна
#define HEATER_PIN_1 9     //обогреватель 1, половина возле двери
#define DC18B20_PIN 10      //пин для термометров термистров обогревателя
#define LED_PIN_CENTR 28    //лента центр
#define LED_PIN_STATION 45  // пин светодиода событий.
#define PIR_SENSOR 47       //датчик движения ик
#define STIK_KEY_PIN 42     //btn joostic
#define FREE_RELE_PIN 35    //low signal
#define CLEANER_FUN_PIN 36  //low signal
//pwm pin
#define LCD_BRIGHTNESS 4    //яркость экрана, шим.
//analog pin
#define DHT_PIN A0          //датчик dht11
#define STIK_Y_PIN A10      //joostic
#define STIK_X_PIN A11
#define PHOTO_SENSOR A12  //датчик освещённости

//#include <microDS18B20.h>
#include <Arduino.h>
#include "macros.h"
#include "heater.h"
#include "fun_cleaner.h"
//#include "EEPROM_memory.h"
#include "eeprom_pair.h"      //временно заменить на еепром мемори


//---------переменные--------
//----temperature
auto temperature_day  = create(TEMPERATURE_DAY * 10,2,"temp day");             //температура дня, *10 -> уйти от float
auto temperature_night = create (TEMPERATURE_NIGHT * 10,4,"temp night");           //температура ночи
auto temperature_day_off = create(200,6,"temp day off");         //температура простоя
auto temperature_sunrise = create(210,8,"temp sunrise");         //температура для подъёма
auto temperature_our_house = create(20,10,"temp our house");       //температура если уехал
float temperature_now;                             //температура измерения
//----lighting----
uint8_t random_color;
uint8_t random_saturation;
uint8_t light_color_now = 30;
uint8_t light_saturation_now = 180;
uint8_t Brightness;
//----clock----
auto work_alarm_clock(create(false,12,"work alarm clock"));  // вкл/выкл будильник
//-----menu------
static uint32_t time_remove_mainLcd;  //переменная времени для возврата на главный экран
static uint32_t time_backlight_lcd;   //переменная выключения подсветки экрана
bool flag_backlight_lcd = true;
bool menu_click;
uint8_t last_mode_menu;  //для сохранения предыдущего режима
uint8_t line_menu;

void setup() {
  Serial.begin(115200);
  Serial.println();
}

void loop() {


}
