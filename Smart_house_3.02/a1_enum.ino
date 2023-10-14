
enum MODE_MENU {
  MAIN_LCD , MENU , BRIGHTNESS, MODE_LIGHTING, TEMPERATURE  , 
  MODE_SCRIPT_HOUSE, ALARM_CLOCK, LED_PATTERN
  } mode_menu;
 
enum SCRIPT_HOUSE { 
  FIRST_SCRIP, MOVE,  SLEEP, VIEWING_FILM, OUTSIDE_THE_HOME, NIGHT_MOVE , NO_MOVE, SUNRISE, LAST_SCRIP,
} ;
auto script_house (create(NO_MOVE, 13,"script house"));       //режим работы,создание пары (значение,адрес)
SCRIPT_HOUSE last_script_house  = FIRST_SCRIP; 

enum MODE_LIGHT_BEDROM {   //нулевой не отправляет по сериал, NULL_LIGHT как заглушка
  NULL_LIGHT, OFF_LIGHT,  START_LIGHT,  NORMAL_LIGHT,  BLACKOUT_LIGHT,
  NIGHT_LIGHT,  SUNRISE_LIGHT, COLOR_MUSIC, SUNSET_LIGHT, 
} mode_light_bedroom;


enum {
   off, on,
} ON_OFF ;
