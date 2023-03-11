
void accomplishment_script_house () {  
  static uint32_t timer_waiting;  

//----------DAY----------------  
    switch (script_house.value) {
      case( MOVE) :           
      if ( last_script_house != MOVE ) {            //если первый запуск
        heater_room_1.set_temperature (&temperature_day.value);
        heater_room_2.set_temperature (&temperature_day.value);
        mode_light_bedroom = START_LIGHT; serialWriteNano(mode_light_bedroom, 'a');
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
        heater_room_1.set_temperature (&temperature_day_off.value);
        heater_room_2.set_temperature (&temperature_day_off.value);
        mode_light_bedroom = OFF_LIGHT;serialWriteNano(mode_light_bedroom, 'a');
        last_script_house = NO_MOVE;
        fun.cleaner(on); 
      }
      if ( pir_sensor ()){
        timer_waiting = millis();
        script_house = MOVE;
      }

      case( VIEWING_FILM) :
      if ( last_script_house != VIEWING_FILM ) {            //если первый запуск
        heater_room_1.set_temperature (&temperature_day.value);
        heater_room_2.set_temperature (&temperature_day.value);
        mode_light_bedroom = OFF_LIGHT;serialWriteNano(mode_light_bedroom, 'a');
        last_script_house = VIEWING_FILM;
        fun.cleaner(off); 
      }      
      
      break;     
//**********DAY**********

//----------NIGHT-------------    
        case( SLEEP) :
        if ( last_script_house != SLEEP ) {            //если первый запуск
          mode_light_bedroom = SUNSET_LIGHT; serialWriteNano(mode_light_bedroom, 'a');
          last_script_house = SLEEP;
          heater_room_1.set_temperature (&temperature_night.value);
          heater_room_2.set_temperature (&temperature_night.value);
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
          heater_room_1.set_temperature (&temperature_sunrise.value);
          heater_room_2.set_temperature (&temperature_sunrise.value);
          mode_light_bedroom = SUNRISE_LIGHT;serialWriteNano(mode_light_bedroom, 'a');
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
        heater_room_1.set_temperature (&temperature_our_house.value);
        heater_room_2.set_temperature (&temperature_our_house.value);
        mode_light_bedroom = OFF_LIGHT;serialWriteNano(mode_light_bedroom, 'a');
      }
      break;
//**********OUTSIDE_THE_HOME 

      default :
      break;
  }
}  



      
