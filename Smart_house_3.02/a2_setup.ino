//********************SETUP******************

void setup() {
  #if (DEBUGING == 1) 
  Serial.begin(9600);
  #endif
  Serial2.begin(9600);        //связь с нано
  Serial3.begin(115200);      //связь с есп
//---------пины--------
  pinMode(PHOTO_SENSOR, INPUT); 
  pinMode(PIR_SENSOR, INPUT);
  pinMode(MIC_SENSOR, INPUT); 
  pinMode(LED_PIN_STATION, OUTPUT);   
//---------дисплей---------
  LCD.init();            // инициализация LCD дисплея
  LCD.clear();
  LCD.backlight();       // включение подсветки дисплея
  LCD.setCursor(0,0);
  LCD.print( "version "); LCD.print( ver); LCD.print(LCDdebug);
//---------ик приёмник------
  IRLremote.begin(IR_PIN);
//---------dht11----------
  dht.begin();
//---------btn------
  joystick.debounce(400);        // настройка антидребезга (по умолчанию 80 мс) 
//---------модуль времени------
  sunriseStartSet();  
  #if ( TIME_SETUP == 1)   //запись времени в rtc модуль
    rtc.setTime(BUILD_SEC, BUILD_MIN, BUILD_HOUR, BUILD_DAY, BUILD_MONTH, BUILD_YEAR);
  #elif ( TIME_SETUP == 2)
    now.second = setTime_second; now.minute = setTime_minute; now.hour = setTime_hour;
    rtc.setTime(now);  
  #endif
//---------Watchdog-----------
  wdt_disable();   
  wdt_enable(WDTO_4S);          //4s WDTO_4S, 8s WDTO_8S 
//---------чтение/запиись настроек в еепром
  if(key_EEPROM != EEPROM.read(0)){     //запись в еепром
    PRINT("update EEPROM settings", " " );
    EEPROM.put(0, key_EEPROM) ;
    writeEEPROM(temperature_day) ; 
    writeEEPROM(temperature_night);  
    writeEEPROM(temperature_day_off);  
    writeEEPROM(temperature_sunrise);
    writeEEPROM(temperature_our_house);       
    writeEEPROM(alarmClockH);
    writeEEPROM(alarmClockM);
    writeEEPROM(work_alarm_clock);      
    writeEEPROM(script_house);
  }
  else {
    PRINT("read EEPROM settings", " " );
    readEEPROM(temperature_day) ; 
    readEEPROM(temperature_night);  
    readEEPROM(temperature_day_off);  
    readEEPROM(temperature_sunrise);
    readEEPROM(temperature_our_house);   
    readEEPROM(alarmClockH);    
    readEEPROM(alarmClockM);
    readEEPROM(work_alarm_clock);      
    readEEPROM(script_house);   
  }
//  
  delay(2000);
  LCD.clear();
}
//_______________________*SETUP*_____________________
