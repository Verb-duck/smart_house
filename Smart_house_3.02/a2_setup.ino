//********************SETUP******************

void setup() {
  #if (DEBUGING == 1) 
  Serial.begin(9600);
  #endif
  Serial3.begin(115200);      //связь с есп
//---------пины--------
  pinMode(PHOTO_SENSOR, INPUT); 
  pinMode(PIR_SENSOR, INPUT);
  pinMode(MIC_SENSOR, INPUT);  
//---------лента 2811-------
  FastLED.addLeds< WS2811, LED_PIN, BRG>(leds, LED_NUM).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<WS2811, LED_PIN_CENTR, BRG>(leds_centr, LED_NUM_CENTR).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(0);
  FastLED.clear();
  FastLED.show();   
//---------дисплей---------
  LCD.init();            // инициализация LCD дисплея
  LCD.clear();
  LCD.backlight();       // включение подсветки дисплея
  LCD.setCursor(0,0);
  LCD.print( "version "); LCD.print( ver); LCD.print(LCDdebug);
//------ик приёмник------
  IRLremote.begin(IR_PIN);
//---------dht11----------
  dht.begin();
//---------btn------
  joystick.debounce(400);        // настройка антидребезга (по умолчанию 80 мс) 
//------модуль времени------
  sunriseStartSet();  
  #if ( TIME_SETUP == 1)   //запись времени в rtc модуль
    rtc.setTime(BUILD_SEC, BUILD_MIN, BUILD_HOUR, BUILD_DAY, BUILD_MONTH, BUILD_YEAR);
  #elif ( TIME_SETUP == 2)
    now.second = setTime_second; now.minute = setTime_minute; now.hour = setTime_hour;
    rtc.setTime(now);  
  #endif   
 

//-------чтение/запиись настроек в еепром

std::cout << "temperature_day " << temperature_day.value << " \t adddr " << temperature_day.addr << std::endl; 
std::cout << "temperature_night " << temperature_night.value << " \t adddr " << temperature_night.addr << std::endl; 
std::cout << "temperature_day_off " << temperature_day_off.value << " \t adddr " << temperature_day_off.addr << std::endl; 
std::cout << "last_script_house " << last_script_house.value << " \t adddr " << last_script_house.addr << std::endl; 

  if(key_EEPROM != EEPROM.read(0)){     //запись в еепром
    EEPROM.put(0, key_EEPROM) ;
    writeEEPROM(temperature_day) ; 
    writeEEPROM(temperature_night);  
    writeEEPROM(temperature_day_off);  
    writeEEPROM(temperature_sunrise);
    writeEEPROM(temperature_our_house);       
    writeEEPROM(script_house);         
    writeEEPROM(last_script_house);   
    writeEEPROM(work_alarm_clock);      
    PRINT("update EEPROM settings", " " );
  }
  else {
    EEPROM.get(2, temperature_day.value) ;
    EEPROM.get(6, temperature_night.value);
    EEPROM.get(10, temperature_day_off.value);
    EEPROM.get(14, temperature_sunrise.value);
    EEPROM.get(18, temperature_our_house.value);       
    EEPROM.get(22, script_house);
    EEPROM.get(24, last_script_house);
    EEPROM.get(26, work_alarm_clock);
    PRINT("read EEPROM settings", " " );
  }
  
  
  delay(2000);
  LCD.clear();
}
//_______________________*SETUP*_____________________
