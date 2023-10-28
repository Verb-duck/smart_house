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
#define ZERO_CROSS 1        //детектор ноля в 220в,  не используется
#define IR_PIN 18           //ИК приёмник
#define HEATER_PIN_2 8     //обогреватель 2, половина возле окна
#define HEATER_PIN_1 9     //обогреватель 1, половина возле двери
#define DC18B20_PIN 10      //пин для термометров термистров обогревателя
#define LED_PIN 27          //лента контур
#define LED_PIN_CENTR 28    //лента центр
#define LED_PIN_STATION 45  // пин светодиода событий.
#define PIR_SENSOR 47       //датчик движения ик
#define STIK_KEY_PIN 42     //btn joostic
#define MIC_SENSOR 15       //датчик звука
#define FREE_RELE_PIN 35    //low signal
#define CLEANER_FUN_PIN 36  //low signal
//pwm pin
#define LCD_BRIGHTNESS 4    //яркость экрана, шим.
//analog pin
#define DHT_PIN A0          //датчик dht11
#define SOUND_R A2          //аналоговый пин вход аудио, правый канал
#define SOUND_L A3          //аналоговый пин вход аудио, левый канал
#define SOUND_R_FREQ A1     // аналоговый пин вход аудио для режима с частотами (через кондер)
#define STIK_Y_PIN A10      //joostic
#define STIK_X_PIN A11
#define PHOTO_SENSOR A12  //датчик освещённости
#include <EEPROM.h>
template<class Type>
struct pair {
    Type value;
    int addres;
    char* name;
    pair(Type value = Type(), int addres = 255, char* enter_name = "free") : value(value), addres(addres) {
        size_t size = 0;
        while (enter_name[size]) {
            size++;
        }
        size++;
        name = new char[size];
        size = 0;
        while (enter_name[size]) {
            name[size] = enter_name[size];
            size++;
        }
        name[size] = '\0';
    }
    void operator =(Type value)
    {
        this->value = value;
        writeEEPROM(*this);
    }
    void operator +=(Type value)
    {
        this->value += value;
        writeEEPROM(*this);
    }
    void operator -=(Type value)
    {
        this->value -= value;
        writeEEPROM(*this);
    }
};
enum typeValue {
    BOOL = 1,  //1 byte 
    INT = 2,   //2 byte
    ENUM = 2,
    FLOAT = 4, // 4 byte 
};
//создание переменной с присвоением адреса в EEPROM
int next_addr = 2;       //0 занят для key_EEPROM
template <class Type>
pair<Type> create(const Type value, const int addres, const char* name) {
    return pair<Type>(value, addres, name);

}

template <class Type>
bool writeEEPROM(pair<Type>& pp) {
    EEPROM.put(pp.addres, pp.value);
    Serial.print(pp.name);
    Serial.print(" ");
    Serial.println(pp.value);
    return 0;
}

template <class Type>
bool readEEPROM(pair<Type>& pp) {
    EEPROM.get(pp.addres, pp.value);
    Serial.print(pp.name);
    Serial.print(" ");
    Serial.println(pp.value);
    return 0;
}
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




//-------макросы---------
#define FULL_STRING 1
#define LIGHT_STRING 0
#define PERIOD(x) \
  static uint32_t tmr; \
  bool flag = millis() - tmr >= (x); \
  if (flag) tmr += (x); \
  if (flag)

#if (DEBUGING == 1)
#define PRINT(title, y) \
  Serial.print(title); \
  Serial.print(": "); \
  Serial.println(y);
#define LCDdebug "DEBUG"
#else
#define PRINT(titly, y)
#define LCDdebug ""
#endif
