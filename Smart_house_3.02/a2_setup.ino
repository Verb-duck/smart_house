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
  
  float temperature_day = 22;               //температура дня
  float temperature_night = 19;             //температура ночи
  float temperature_day_off = 20;             //температура простоя
  float temperature_sunrise = 20;             //температура для подъёма
  float temperature_our_house = 16;           //температура если уехал  
//-------чтение/запиись настроек в еепром
  if(key_EEPROM != EEPROM.read(0)){     //запись в еепром
    EEPROM.put(0, key_EEPROM) ;
    EEPROM.put(2, temperature_day) ;
    EEPROM.put(6, temperature_night);
    EEPROM.put(10, temperature_day_off);
    EEPROM.put(14, temperature_sunrise);
    EEPROM.put(18, temperature_our_house);       
    EEPROM.put(22, script_house);    // 2bit
    EEPROM.put(24, last_script_house);
    PRINT("update EEPROM settings", " " );
  }
  else {
    EEPROM.get(2, temperature_day) ;
    EEPROM.get(6, temperature_night);
    EEPROM.get(10, temperature_day_off);
    EEPROM.get(14, temperature_sunrise);
    EEPROM.get(18, temperature_our_house);       
    EEPROM.get(22, script_house);
    EEPROM.get(24, last_script_house);
    PRINT("read EEPROM settings", " " );
  }
  
  
  delay(2000);
  LCD.clear();
}
//_______________________*SETUP*_____________________
