

void loop() {
 timeNow= rtc.getTime();               //запись времени в структуру
 accomplishment_script_house ();      //режимы работы  
 lcd_imagin ();                        //вывод на экран
 control_joustick();  
 dimmer_temp ();
 light_bedroom();  
}

//---------подстройка температуры----------
void dimmer_temp () {
  PERIOD (PERIOD_SENSOR_TEMPERATURE * 1000) { 
  temperature_now = dht.readTemperature()-2;    //опрос датчика температуры   
  heater_room_1.work (temperature_now);
  heater_room_2.work (temperature_now); 
  }
}
