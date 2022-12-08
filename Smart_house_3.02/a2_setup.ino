//********************SETUP******************

void setup() {
  #if (DEBUGING == 1) 
  Serial.begin(9600);
  #endif
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
//-----------диммер-----------------  
 
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
  #if ( TIME_SETUP == 1)   
    rtc.setTime(BUILD_SEC, BUILD_MIN, BUILD_HOUR, BUILD_DAY, BUILD_MONTH, BUILD_YEAR);
  #elif ( TIME_SETUP == 2)
    now.second = setTime_second; now.minute = setTime_minute; now.hour = setTime_hour;
    rtc.setTime(now);  
  #endif  
  
  delay(2000);
  LCD.clear();
}
//_______________________*SETUP*_____________________
