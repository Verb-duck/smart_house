
enum MODE_MENU {
  MAIN_LCD , MENU , BRIGHTNESS, MODE_LIGHTING, TEMPERATURE_VIEW  , 
  MODE_SCRIPT_HOUSE, ALARM_CLOCK, LED_PATTERN
  } mode_menu;

//пронумерованны первые, что бы не пересеrалась нумераця при отправке по uart 
enum SCRIPT_HOUSE { 
  FIRST_SCRIP = 0, SCRIPT_HOUSE_NOW, DAY,  SLEEP, VIEWING_FILM, LEFT_HOME, NIGHT_MOVE , DAY_CAME_OUT, SUNRISE, SUNSET, LAST_SCRIP, 
} ;
auto script_house (create(DAY_CAME_OUT, ENUM));       //режим работы,создание пары (значение,адрес)
SCRIPT_HOUSE last_script_house  = FIRST_SCRIP; 

enum MODE_LIGHT_BEDROM {   //нулевой не отправляет по сериал, FIRST_LIGHT как заглушка
  FIRST_LIGHT = 20, MODE_LIGHT_BEDROM_NOW, OFF_LIGHT,  START_LIGHT,  NORMAL_LIGHT,  BLACKOUT_LIGHT,
  NIGHT_LIGHT,  SUNRISE_LIGHT, COLOR_MUSIC, SUNSET_LIGHT,  LAST_LIGHT
} mode_light_bedroom;

enum {
   off, on,
} ON_OFF ;

enum SERIAL_CODE_BYTE {
  FIRST = 40,
  TEMPERATURE, TEMPERATURE_RAISE, TEMPERATURE_REDUCE,
  TIME, SINCHRONIZATION, ALARM, ALARM_OFF , HOUR, MINUTE, SECOND,
};