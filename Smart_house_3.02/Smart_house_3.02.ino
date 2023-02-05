#define ver "3.03 "
#define DEBUGING 1

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
#define key_EEPROM 2  // запись/сброс настроек в EEPROM при прошивке, сменить число

//---------пины--------
#define ZERO_CROSS 1        //детектор ноля в 220в,  не используется
#define IR_PIN 18           //ИК приёмник
#define HEATER_PIN_1 22     //обогреватель 1
#define HEATER_PIN_2 23     //обогреватель 2
#define DC18B20_PIN 24      //пин для термометров
#define LED_PIN 27          //лента контур
#define LED_PIN_CENTR 28    //лента центр
//#define MLED_PIN 34         // пин светодиода режимов цветомузыки. событий.
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

//-------pair struct
  #include <EEPROM.h>
  template<class Type>  
     struct pair {
	  Type value;
	  int addr;
	  pair(Type f = Type(), int s = 254) : value(f), addr(s) {}	
    void operator =(Type value) 
    {
      this->value = value; 
      writeEEPROM( *this);
      Serial.print( "new value:\t");Serial.println( this->addr);
    }
    void operator +=(Type value) 
    {
      this->value += value; 
      writeEEPROM( *this);
      Serial.print( "new value:\t");Serial.println( this->addr);
    }
    void operator -=(Type value) 
    {
      this->value -= value; 
      writeEEPROM( *this);
      Serial.print( "new value:\t");Serial.println(this-> addr);
    }
    
  };
  enum typeValue {
  	BOOL = 1,  //1 byte 
  	INT = 2,   //2 byte
    ENUM = 2,
  	FLOAT = 4, // 4 byte 
  };
  //построение пары, присвоение адреса в EEPROM
  	int next_addr = 2;       //0 занят для key_EEPROM
    template <class Type>
    pair<Type> create (const Type value, typeValue type) {
  	int count_addr = next_addr;
  	next_addr += (int)type;
  	return pair<Type> (value, count_addr);	
  }

  template <class Type>
  bool writeEEPROM (pair<Type> &pp ) {
    EEPROM.put(pp.addr,pp.value) ;
  return 0;
  } 

  template <class Type>
  bool readEEPROM (pair<Type> &pp ) {
    EEPROM.get(pp.addr,pp.value) ;
  return 0;
  } 
 
//---------переменные--------
//----temperature
auto temperature_day  = create(200,INT);             //температура дня, *10 -> уйти от float
auto temperature_night = create (190,INT);           //температура ночи
auto temperature_day_off = create(200,INT);         //температура простоя
auto temperature_sunrise = create(200,INT);         //температура для подъёма
auto temperature_our_house = create(30,INT);       //температура если уехал
float temperature_now;                             //температура измерения
//----lighting----
uint8_t random_color;
uint8_t random_saturation;
uint8_t light_color_now = 30;
uint8_t light_saturation_now = 180;
uint8_t Brightness;
//----clock----
auto work_alarm_clock(create(false,BOOL));  // вкл/выкл будильник
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
