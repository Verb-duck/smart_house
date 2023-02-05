
void accomplishment_script_house () {  
  static uint32_t timer_waiting;        
      
/* script_house:
  MOVE,
  NO_MOVE,
  SLEEP, 
  SUNRISE, 
  OUTSIDE_THE_HOME,*/

//----------DAY----------------  
    switch (script_house.value) {
      case( MOVE) :           
      if ( last_script_house != MOVE ) {            //если первый запуск
        heater_room_1.set_temperature (temperature_day.value);
        heater_room_2.set_temperature (temperature_day.value);
        mode_light_bedroom = START_LIGHT;
        randomSeed( millis());  random_color = random(256);                         //рандом цвета заполнения центр ленты
        randomSeed( micros());  random_saturation = random(170 , 256);              //рандом насыщенности заполнения центр ленты
        randomSeed( millis());  led_pattern_number = random(quantity_led_pattern);  //рандомный выбор режима подсветки
        last_script_house = MOVE;
        fun.cleaner(off);
      }
      if ( pir_sensor ()) {            
        timer_waiting = millis();       //сбрасываем таймер выключения
      }
      if ( millis() - timer_waiting >= TIME_OFF_NO_MOVE * 60000) {  //выключение при простое
        script_house = NO_MOVE;
      }
     
      break;

      case( NO_MOVE) :
      if ( last_script_house != NO_MOVE ) {            //если первый запуск
        heater_room_1.set_temperature (temperature_day_off.value);
        heater_room_2.set_temperature (temperature_day_off.value);
        mode_light_bedroom = OFF_LIGHT;
        last_script_house = NO_MOVE;
        fun.cleaner(on); 
      }
      if ( pir_sensor ()){
        timer_waiting = millis();
        script_house = MOVE;
      }
      
      
      break;     
//**********DAY**********

//----------NIGHT-------------    
        case( SLEEP) :
        if ( last_script_house != SLEEP ) {            //если первый запуск
          mode_light_bedroom = SUNSET_LIGHT; 
          last_script_house = SLEEP;
          heater_room_1.set_temperature (temperature_night.value);
          heater_room_2.set_temperature (temperature_night.value);
          fun.cleaner(off); 
        }
       //включение будильника  
        if (work_alarm_clock.value && sunriseStartTime == timeNow) {
          script_house = SUNRISE;
        }
 /*      //включение ночной подсветки
        static uint8_t count_start_nigh_light = 0;
        if( pir_sensor () && (millis() - timer_waiting > 3000)) {
          count_start_nigh_light ++;
          timer_waiting = millis();
          if( count_start_nigh_light = 2 ) {
            mode_light_bedroom = NIGHT_LIGHT;
          }
        }
        if ( count_start_nigh_light && (millis() - timer_waiting > 10000))
          count_start_nigh_light = 0;
          mode_light_bedroom = BLACKOUT_LIGHT; */
        break;
        

       case(SUNRISE) :
        if ( last_script_house != SUNRISE ) {            //если первый запуск
          heater_room_1.set_temperature (temperature_sunrise.value);
          heater_room_2.set_temperature (temperature_sunrise.value);
          mode_light_bedroom = SUNRISE_LIGHT;
          last_script_house = SUNRISE;
        }
      //выключение будильника при движении    
        static uint8_t count_start_day = 0;
        if ( pir_sensor () && (millis() - timer_waiting > 8000) )  {
          timer_waiting = millis();
          count_start_day ++;          
          if ( count_start_day == 3) {
            script_house = MOVE;
            count_start_day = 0;
          } 
        }
        if ( count_start_day && millis() - timer_waiting > 30000)
          count_start_day = 0;
        break;
      
//**********NIGHT**********  

//----------OUTSIDE_THE_HOME------
      case(OUTSIDE_THE_HOME):
      if ( last_script_house != OUTSIDE_THE_HOME) {
        last_script_house =  OUTSIDE_THE_HOME;
        heater_room_1.set_temperature (temperature_our_house.value);
        heater_room_2.set_temperature (temperature_our_house.value);
        mode_light_bedroom = OFF_LIGHT;
      }
      break;
//**********OUTSIDE_THE_HOME 

      default :
      break;
  }
}  



      
