

// ***************************** НАСТРОЙКИ *****************************
 // ----- настройки параметров
  #define DEBUGING  1
  #define KEEP_SETTINGS 1     // хранить ВСЕ настройки в энергонезависимой памяти
  #define KEEP_STATE 1		    // сохранять в памяти состояние вкл/выкл системы (с пульта)
  #define RESET_SETTINGS 0    // сброс настроек в EEPROM памяти (поставить 1, прошиться, поставить обратно 0, прошиться. Всё)
  #define SETTINGS_LOG 0      // вывод всех настроек из EEPROM в порт при запуске
  #define REMOTE_KEY 1

 // ----- настройки ленты
  #define FASTLED_ALLOW_INTERRUPTS 1
  #include "FastLED.h"
  //-----leds_contur---
  #define LED_NUM 115        // количество светодиодов (данная версия поддерживает до 410 штук)
  CRGB leds[LED_NUM];
  //----leds_centr----
  #define LED_NUM_CENTR 40
  CRGB leds_centr[LED_NUM_CENTR];
  //----для будильника------
  #define COOLING  70    //длина пламени  
  #define SPARKING 150   //искры
  bool gReverseDirection = false;
  #define FRAMES_PER_SECOND 60
  #define TIME_SUNRISE 40              //время включения рассвета до будильника
  int time_sunrise = TIME_SUNRISE * 60000 / 256; // вычесление периода добавления яркости рассвета

  byte Brightness = 10;      // яркость по умолчанию (0 - 255)
  uint8_t random_color;
  uint8_t random_saturation;
  uint8_t red_color_now = 30;
  uint8_t green_color_now = 180;
  uint8_t blue_color_now = 255;
  
  uint8_t gHue = 0; // rotating "base color" used by many of the patterns
  uint8_t sped_led_show = 50;
 //--color temperature pattern
  #define TEMPERATURE_1 Tungsten100W
  #define TEMPERATURE_2 OvercastSky
  // How many seconds to show each temperature before switching
  #define DISPLAYTIME 20
  // How many seconds to show black between switches
  #define BLACKTIME   3

 //---------UART---------------  
  #include <SoftwareSerial.h>
  SoftwareSerial SerialMega(7, 8);   // RX, TX    
  #include "AsyncStream.h"
  AsyncStream<40> Mega_serial(&SerialMega,';',100); 
  #include "GParser.h"
  GParser Mega_parser (Mega_serial.buf, '^');
  char outBuff[20];  
 
 // ----- пины подключения
  #define SOUND_R A1         // аналоговый пин вход аудио, правый канал
  #define SOUND_L A2         // аналоговый пин вход аудио, левый канал
  #define SOUND_R_FREQ A3    // аналоговый пин вход аудио для режима с частотами (через кондер)
  #define MLED_PIN A7             // пин светодиода режимов
  #define MLED_ON HIGH
  #define LED_PIN 4              // пин DI светодиодной ленты
  #define IR_PIN 3                // пин ИК приёмника
  #define LED_PIN_CENTR 9    //лента центр


 // ----- настройки радуги 
  float RAINBOW_STEP = 5.00;         // шаг изменения цвета радуги

 // ----- отрисовка
  #define MODE 4                    // режим при запуске
  #define MAIN_LOOP 5               // период основного цикла отрисовки (по умолчанию 5)

 // ----- сигнал
  #define MONO 0                    // 1 - только один канал (ПРАВЫЙ!!!!! SOUND_R!!!!!), 0 - два канала
  #define EXP 1.4                   // степень усиления сигнала (для более "резкой" работы) (по умолчанию 1.4)
  #define POTENT 1                  // 1 - используем потенциометр, 0 - используется внутренний источник опорного напряжения 1.1 В
  byte EMPTY_BRIGHT = 15;           // яркость "не горящих" светодиодов (0 - 255)
  #define EMPTY_COLOR HUE_ORANGE    // цвет "не горящих" светодиодов. Будет чёрный, если яркость 0

 // ----- нижний порог шумов
  uint16_t LOW_PASS = 100;          // нижний порог шумов режим VU, ручная настройка
  uint16_t SPEKTR_LOW_PASS = 40;    // нижний порог шумов режим спектра, ручная настройка
  #define AUTO_LOW_PASS 0           // разрешить настройку нижнего порога шумов при запуске (по умолч. 0)
  #define EEPROM_LOW_PASS 1         // порог шумов хранится в энергонезависимой памяти (по умолч. 1)
  #define LOW_PASS_ADD 13           // "добавочная" величина к нижнему порогу, для надёжности (режим VU)
  #define LOW_PASS_FREQ_ADD 3       // "добавочная" величина к нижнему порогу, для надёжности (режим частот)

 // ----- режим шкала громкости
  float SMOOTH = 0.3;               // коэффициент плавности анимации VU (по умолчанию 0.5)
  #define MAX_COEF 1.8              // коэффициент громкости (максимальное равно срднему * этот коэф) (по умолчанию 1.8)

 // ----- режим цветомузыки
  float SMOOTH_FREQ = 0.8;          // коэффициент плавности анимации частот (по умолчанию 0.8)
  float MAX_COEF_FREQ = 1.2;        // коэффициент порога для "вспышки" цветомузыки (по умолчанию 1.5)
  #define SMOOTH_STEP 20            // шаг уменьшения яркости в режиме цветомузыки (чем больше, тем быстрее гаснет)
  #define LOW_COLOR HUE_RED         // цвет низких частот
  #define MID_COLOR HUE_BLUE       // цвет средних
  #define HIGH_COLOR HUE_YELLOW     // цвет высоких

 // ----- режим стробоскопа
  uint16_t STROBE_PERIOD = 140;     // период вспышек, миллисекунды
  #define STROBE_DUTY 20            // скважность вспышек (1 - 99) - отношение времени вспышки ко времени темноты
  #define STROBE_COLOR HUE_YELLOW   // цвет стробоскопа
  #define STROBE_SAT 0              // насыщенность. Если 0 - цвет будет БЕЛЫЙ при любом цвете (0 - 255)
  byte STROBE_SMOOTH = 200;         // скорость нарастания/угасания вспышки (0 - 255)

 // ----- режим подсветки
  byte LIGHT_COLOR = 0;             // начальный цвет подсветки
  byte LIGHT_SAT = 255;             // начальная насыщенность подсветки
  byte COLOR_SPEED = 100;
  int RAINBOW_PERIOD = 1;
  float RAINBOW_STEP_2 = 0.5;

 // ----- режим бегущих частот
  byte RUNNING_SPEED = 11;

 // ----- режим анализатора спектра
  byte HUE_START = 0;
  byte HUE_STEP = 5;
  #define LIGHT_SMOOTH 2

 /*Цвета для HSV    
    HUE_RED
    HUE_ORANGE
    HUE_YELLOW
    HUE_GREEN
    HUE_AQUA
    HUE_BLUE
    HUE_PURPLE
    HUE_PINK
  */

 // ----- КНОПКИ ПУЛЬТА -----
  #if  (REMOTE_KEY == 1)
    #define BUTT_STAR   0x1B92DDAD   // 
    #define BUTT_HASH   0x187E18AD   //start/stop
    #define BUTT_UP     0x65CF91AD    //queck
    #define BUTT_DOWN   0x2FF206AD    //slow
    #define BUTT_LEFT   0x72EB89AD    //auto  
    #define BUTT_RIGHT  0xA8C914AD    //flash 
    #define BUTT_OK     0x57997BAD    //flade3
    #define BUTT_1      0x18319BAD    //diy1   
    #define BUTT_2      0x151CD6AD    //diy2
    #define BUTT_3      0xABDDD9AD    //diy3
    #define BUTT_4      0xE25410AD    //diy4
    #define BUTT_8      0xDF3F4BAD    //diy5
    #define BUTT_9      0x76004EAD     //diy6
    #define BUTT_0      0x5484B6AD    ///flade7
    #define BUTT_5      0xE207E1AD    //мой режим
    #define BUTT_7      0xAF3F1BAD    //не юзаю
    #define BUTT_6      0x14CE54AD      //не юзаю
    #define BUTT_JUMPUP 0xC3ED3DAD    //jump 3
    #define BUTT_JUMPDW 0xC0D878AD    //jump 7
    #define BUTT_HIGHT_UP   0xB6FFA1AD
    #define BUTT_HIGHT_DOWN 0xB3EADCAD
    #define BUTT_RED    0x812216AD
    #define BUTT_RED1   0x62BB43AD
    #define BUTT_RED2   0x2CDDB8AD
    #define BUTT_RED3   0x297C76AD
    #define BUTT_RED4   0xF39EEBAD
    #define BUTT_GREEN  0x7E0D51AD
    #define BUTT_GREEN1 0x5FA67EAD
    #define BUTT_GREEN2 0x29C8F3AD
    #define BUTT_GREEN3 0x2667B1AD
    #define BUTT_GREEN4 0xF08A26AD
    #define BUTT_BLUE   0x4E5BA3AD
    #define BUTT_BLUE1  0x6CC276AD
    #define BUTT_BLUE2  0xA2A001AD
    #define BUTT_BLUE3  0xBD28B4AD
    #define BUTT_BLUE4  0x874B29AD
    #define BUTT_WHITE  0x517068AD
    #define BUTT_WHITE1 0x6FD73BAD
    #define BUTT_WHITE2 0xA5B4C6AD
    #define BUTT_WHITE3 0xBA13EFAD
    #define BUTT_WHITE4 0x843664AD
    #define BUTT_RED_UP 0xD53818AD
    #define BUTT_RED_DOWN 0x9F5A8DAD
    #define BUTT_GREEN_UP 0xD22353AD
    #define BUTT_GREEN_DOWN 0x9C45C8AD
    #define BUTT_BLUE_UP 0x68E456AD
    #define BUTT_BLUE_DOWN 0x3306CBAD

    #endif
  #if  (REMOTE_KEY == 0)   //малый пульт
    #define BUTT_UP     0xE207E1AD    //ch+
    #define BUTT_DOWN   0x4E5BA3AD    //ch-
    #define BUTT_LEFT   0x517068AD    //prew
    #define BUTT_RIGHT  0x1B92DDAD    //next
    #define BUTT_OK     0xE51CA6AD    //ch
    #define BUTT_PLUS   0xD22353AD
    #define BUTT_MINUS  0x5484B6AD
    #define BUTT_0      
    #define BUTT_100    0xF08A26AD
    #define BUTT_200    0x151CD6AD
    #define BUTT_1      0x18319BAD    
    #define BUTT_2      0xF39EEBAD
    #define BUTT_3      0x4AABDFAD
    #define BUTT_4      0xE25410AD
    #define BUTT_5      0x297C76AD
    #define BUTT_6      0x14CE54AD
    #define BUTT_7      0xAF3F1BAD
    #define BUTT_8      0xC089F6AD
    #define BUTT_9      0x38379AD
    #define BUTT_0      0x68E456AD
    #define BUTT_STAR   0xAC2A56AD  // play
    #define BUTT_HASH   0xDF3F4BAD   // eq
    #endif

 // ------------------------------ ДЛЯ РАЗРАБОТЧИКОВ --------------------------------
  #define MODE_AMOUNT 9      // количество режимов

  #define STRIPE LED_NUM / 5
  float freq_to_stripe = LED_NUM / 40; // /2 так как симметрия, и /20 так как 20 частот

  #define FHT_N 64         // ширина спектра х2
  #define LOG_OUT 1
  #include <FHT.h>         // преобразование Хартли

  #include <EEPROMex.h> 

  #include "IRLremote.h"
  CHashIR IRLremote;
  uint32_t IRdata;

  // градиент-палитра от зелёного к красному
  DEFINE_GRADIENT_PALETTE(soundlevel_gp) {
    0,    0,    255,  0,  // green
    100,  255,  255,  0,  // yellow
    150,  255,  100,  0,  // orange
    200,  255,  50,   0,  // red
    255,  255,  0,    0   // red
  };
  CRGBPalette32 myPal = soundlevel_gp;

  int Rlenght, Llenght;
  float RsoundLevel, RsoundLevel_f;
  float LsoundLevel, LsoundLevel_f;

  float averageLevel = 50;
  int maxLevel = 100;
  int MAX_CH = LED_NUM / 2;
  int hue;
  unsigned long main_timer, hue_timer, strobe_timer, running_timer, color_timer, rainbow_timer, eeprom_timer;
  float averK = 0.006;
  byte count;
  float index = (float)255 / MAX_CH;   // коэффициент перевода для палитры
  boolean lowFlag;
  byte low_pass;
  int RcurrentLevel, LcurrentLevel;
  int colorMusic[3];
  float colorMusic_f[3], colorMusic_aver[3];
  boolean colorMusicFlash[3], strobeUp_flag, strobeDwn_flag;
  byte this_mode = MODE;
  int thisBright[3];
  unsigned int light_time = STROBE_PERIOD * STROBE_DUTY / 100;
  volatile boolean ir_flag;
  boolean settings_mode, ONstate = true;
  int8_t freq_strobe_mode, light_mode;
  int freq_max;
  float freq_max_f, rainbow_steps;
  int freq_f[32];
  int this_color;
  boolean running_flag[3], eeprom_flag;

  #define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
  #define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
 // ------------------------------ ДЛЯ РАЗРАБОТЧИКОВ --------------------------------
 //-------макросы--------- 
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
 //enum 
  enum MODE_LIGHT_BEDROM {      //нулевой не отправляет по сериал, NULL_LIGHT как заглушка
    NULL_LIGHT, OFF_LIGHT,  START_LIGHT,  NORMAL_LIGHT,  BLACKOUT_LIGHT,
    NIGHT_LIGHT,  SUNRISE_LIGHT, COLOR_MUSIC, SUNSET_LIGHT, 
  } mode_light_bedroom(SUNRISE_LIGHT);

void setup() {
  Serial.begin(9600);
  SerialMega.begin(9600);
 //---------лента 2811------- 
  FastLED.addLeds<WS2811, LED_PIN, BRG>(leds, LED_NUM).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<WS2811, LED_PIN_CENTR, BRG>(leds_centr, LED_NUM_CENTR).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(Brightness);
  FastLED.clear();
  FastLED.show();   

  pinMode(MLED_PIN, OUTPUT);        //Режим пина для светодиода режима на выход
  digitalWrite(MLED_PIN, !MLED_ON); //Выключение светодиода режима

  IRLremote.begin(IR_PIN);

  if (POTENT) analogReference(EXTERNAL);
  else
 #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    analogReference(INTERNAL1V1);
 #else
    analogReference(INTERNAL);
 #endif

  // жуткая магия, меняем частоту оцифровки до 18 кГц
  // команды на ебучем ассемблере, даже не спрашивайте, как это работает
  // поднимаем частоту опроса аналогового порта до 38.4 кГц, по теореме
  // Котельникова (Найквиста) частота дискретизации будет 19.2 кГц
  // http://yaab-arduino.blogspot.ru/2015/02/fast-sampling-from-analog-input.html
  sbi(ADCSRA, ADPS2);
  cbi(ADCSRA, ADPS1);
  sbi(ADCSRA, ADPS0);

  if (RESET_SETTINGS) EEPROM.write(100, 0);        // сброс флага настроек

  if (AUTO_LOW_PASS && !EEPROM_LOW_PASS) {         // если разрешена автонастройка нижнего порога шумов
    autoLowPass();
  }
  if (EEPROM_LOW_PASS) {                // восстановить значения шумов из памяти
    LOW_PASS = EEPROM.readInt(70);
    SPEKTR_LOW_PASS = EEPROM.readInt(72);
  }

  // в 100 ячейке хранится число 100. Если нет - значит это первый запуск системы
  if (KEEP_SETTINGS) {
    if (EEPROM.read(100) != 100) {
      //Serial.println(F("First start"));
      EEPROM.write(100, 100);
      updateEEPROM();
    } else {
      readEEPROM();
    }
  }

 #if (SETTINGS_LOG == 1)
  Serial.print(F("this_mode = ")); Serial.println(this_mode);
  Serial.print(F("freq_strobe_mode = ")); Serial.println(freq_strobe_mode);
  Serial.print(F("light_mode = ")); Serial.println(light_mode);
  Serial.print(F("RAINBOW_STEP = ")); Serial.println(RAINBOW_STEP);
  Serial.print(F("MAX_COEF_FREQ = ")); Serial.println(MAX_COEF_FREQ);
  Serial.print(F("STROBE_PERIOD = ")); Serial.println(STROBE_PERIOD);
  Serial.print(F("LIGHT_SAT = ")); Serial.println(LIGHT_SAT);
  Serial.print(F("RAINBOW_STEP_2 = ")); Serial.println(RAINBOW_STEP_2);
  Serial.print(F("HUE_START = ")); Serial.println(HUE_START);
  Serial.print(F("SMOOTH = ")); Serial.println(SMOOTH);
  Serial.print(F("SMOOTH_FREQ = ")); Serial.println(SMOOTH_FREQ);
  Serial.print(F("STROBE_SMOOTH = ")); Serial.println(STROBE_SMOOTH);
  Serial.print(F("LIGHT_COLOR = ")); Serial.println(LIGHT_COLOR);
  Serial.print(F("COLOR_SPEED = ")); Serial.println(COLOR_SPEED);
  Serial.print(F("RAINBOW_PERIOD = ")); Serial.println(RAINBOW_PERIOD);
  Serial.print(F("RUNNING_SPEED = ")); Serial.println(RUNNING_SPEED);
  Serial.print(F("HUE_STEP = ")); Serial.println(HUE_STEP);
  Serial.print(F("EMPTY_BRIGHT = ")); Serial.println(EMPTY_BRIGHT);
  Serial.print(F("ONstate = ")); Serial.println(ONstate);
  Serial.print(F("mode_light_bedroom = ")); Serial.println(mode_light_bedroom);
  Serial.print(F("Brightness = ")); Serial.println(Brightness);
 #endif
 }

void loop() {  
  remoteTick();     // опрос ИК пульта
  mainLoop();       // главный цикл обработки и отрисовки
  eepromTick();     // проверка не пора ли сохранить настройки
  serialRead();     // связь с мегой
 }

void mainLoop() {
  // главный цикл отрисовки
  if(ONstate) {
    if (millis() - main_timer > MAIN_LOOP) {
      // сбрасываем значения
      RsoundLevel = 0;
      LsoundLevel = 0;

      // перваые два режима - громкость (VU meter)
      if (this_mode == 0 || this_mode == 1) {
        for (byte i = 0; i < 100; i ++) {                                 // делаем 100 измерений
          RcurrentLevel = analogRead(SOUND_R);                            // с правого
          if (!MONO) LcurrentLevel = analogRead(SOUND_L);                 // и левого каналов

          if (RsoundLevel < RcurrentLevel) RsoundLevel = RcurrentLevel;   // ищем максимальное
          if (!MONO) if (LsoundLevel < LcurrentLevel) LsoundLevel = LcurrentLevel;   // ищем максимальное
        }

        // фильтруем по нижнему порогу шумов
        RsoundLevel = map(RsoundLevel, LOW_PASS, 1023, 0, 500);
        if (!MONO)LsoundLevel = map(LsoundLevel, LOW_PASS, 1023, 0, 500);

        // ограничиваем диапазон
        RsoundLevel = constrain(RsoundLevel, 0, 500);
        if (!MONO)LsoundLevel = constrain(LsoundLevel, 0, 500);

        // возводим в степень (для большей чёткости работы)
        RsoundLevel = pow(RsoundLevel, EXP);
        if (!MONO)LsoundLevel = pow(LsoundLevel, EXP);

        // фильтр
        RsoundLevel_f = RsoundLevel * SMOOTH + RsoundLevel_f * (1 - SMOOTH);
        if (!MONO)LsoundLevel_f = LsoundLevel * SMOOTH + LsoundLevel_f * (1 - SMOOTH);

        if (MONO) LsoundLevel_f = RsoundLevel_f;  // если моно, то левый = правому

        // заливаем "подложку", если яркость достаточная
        if (EMPTY_BRIGHT > 5) {
          for (int i = 0; i < LED_NUM; i++)
            leds[i] = CHSV(EMPTY_COLOR, 255, EMPTY_BRIGHT);
        }

        // если значение выше порога - начинаем самое интересное
        if (RsoundLevel_f > 15 && LsoundLevel_f > 15) {

          // расчёт общей средней громкости с обоих каналов, фильтрация.
          // Фильтр очень медленный, сделано специально для автогромкости
          averageLevel = (float)(RsoundLevel_f + LsoundLevel_f) / 2 * averK + averageLevel * (1 - averK);

          // принимаем максимальную громкость шкалы как среднюю, умноженную на некоторый коэффициент MAX_COEF
          maxLevel = (float)averageLevel * MAX_COEF;

          // преобразуем сигнал в длину ленты (где MAX_CH это половина количества светодиодов)
          Rlenght = map(RsoundLevel_f, 0, maxLevel, 0, MAX_CH);
          Llenght = map(LsoundLevel_f, 0, maxLevel, 0, MAX_CH);

          // ограничиваем до макс. числа светодиодов
          Rlenght = constrain(Rlenght, 0, MAX_CH);
          Llenght = constrain(Llenght, 0, MAX_CH);

          animation();       // отрисовать
        }
      }

      // 3-5 режим - цветомузыка
      if (this_mode == 2 || this_mode == 3 || this_mode == 7 || this_mode == 8) {
        analyzeAudio();
        colorMusic[0] = 0;
        colorMusic[1] = 0;
        colorMusic[2] = 0;
        for (int i = 0 ; i < 32 ; i++) {
          if (fht_log_out[i] < SPEKTR_LOW_PASS) fht_log_out[i] = 0;
        }
        // низкие частоты, выборка со 2 по 5 тон (0 и 1 зашумленные!)
        for (byte i = 2; i < 6; i++) {
          if (fht_log_out[i] > colorMusic[0]) colorMusic[0] = fht_log_out[i];
        }
        // средние частоты, выборка с 6 по 10 тон
        for (byte i = 6; i < 11; i++) {
          if (fht_log_out[i] > colorMusic[1]) colorMusic[1] = fht_log_out[i];
        }
        // высокие частоты, выборка с 11 по 31 тон
        for (byte i = 11; i < 32; i++) {
          if (fht_log_out[i] > colorMusic[2]) colorMusic[2] = fht_log_out[i];
        }
        freq_max = 0;
        for (byte i = 0; i < 30; i++) {
          if (fht_log_out[i + 2] > freq_max) freq_max = fht_log_out[i + 2];
          if (freq_max < 5) freq_max = 5;

          if (freq_f[i] < fht_log_out[i + 2]) freq_f[i] = fht_log_out[i + 2];
          if (freq_f[i] > 0) freq_f[i] -= LIGHT_SMOOTH;
          else freq_f[i] = 0;
        }
        freq_max_f = freq_max * averK + freq_max_f * (1 - averK);
        for (byte i = 0; i < 3; i++) {
          colorMusic_aver[i] = colorMusic[i] * averK + colorMusic_aver[i] * (1 - averK);  // общая фильтрация
          colorMusic_f[i] = colorMusic[i] * SMOOTH_FREQ + colorMusic_f[i] * (1 - SMOOTH_FREQ);      // локальная
          if (colorMusic_f[i] > ((float)colorMusic_aver[i] * MAX_COEF_FREQ)) {
            thisBright[i] = 255;
            colorMusicFlash[i] = true;
            running_flag[i] = true;
          } else colorMusicFlash[i] = false;
          if (thisBright[i] >= 0) thisBright[i] -= SMOOTH_STEP;
          if (thisBright[i] < EMPTY_BRIGHT) {
            thisBright[i] = EMPTY_BRIGHT;
            running_flag[i] = false;
          }
        }
        animation();
      }
      //4 заливка цветом
      if (this_mode == 4 || this_mode == 5 || this_mode == 6)   animation();
      if (!IRLremote.receiving())    // если на ИК приёмник не приходит сигнал (без этого НЕ РАБОТАЕТ!)
        FastLED.show();         // отправить значения на ленту

      if (this_mode != 7 && this_mode != 4 && this_mode != 5)       // 7 режиму не нужна очистка!!!
        FastLED.clear();          // очистить массив пикселей
      main_timer = millis();    // сбросить таймер
    }
  }
 }
void animation() {
  // согласно режиму
  switch (this_mode) {
    case 0:
      count = 0;
      for (int i = (MAX_CH - 1); i > ((MAX_CH - 1) - Rlenght); i--) {
        leds[i] = ColorFromPalette(myPal, (count * index));   // заливка по палитре " от зелёного к красному"
        count++;
      }
      count = 0;
      for (int i = (MAX_CH); i < (MAX_CH + Llenght); i++ ) {
        leds[i] = ColorFromPalette(myPal, (count * index));   // заливка по палитре " от зелёного к красному"
        count++;
      }
      if (EMPTY_BRIGHT > 0) {
        CHSV this_dark = CHSV(EMPTY_COLOR, 255, EMPTY_BRIGHT);
        for (int i = ((MAX_CH - 1) - Rlenght); i > 0; i--)
          leds[i] = this_dark;
        for (int i = MAX_CH + Llenght; i < LED_NUM; i++)
          leds[i] = this_dark;
      }
      break;
    case 1:
      if (millis() - rainbow_timer > 30) {
        rainbow_timer = millis();
        hue = floor((float)hue + RAINBOW_STEP);
      }
      count = 0;
      for (int i = (MAX_CH - 1); i > ((MAX_CH - 1) - Rlenght); i--) {
        leds[i] = ColorFromPalette(RainbowColors_p, (count * index) / 2 - hue);  // заливка по палитре радуга
        count++;
      }
      count = 0;
      for (int i = (MAX_CH); i < (MAX_CH + Llenght); i++ ) {
        leds[i] = ColorFromPalette(RainbowColors_p, (count * index) / 2 - hue); // заливка по палитре радуга
        count++;
      }
      if (EMPTY_BRIGHT > 0) {
        CHSV this_dark = CHSV(EMPTY_COLOR, 255, EMPTY_BRIGHT);
        for (int i = ((MAX_CH - 1) - Rlenght); i > 0; i--)
          leds[i] = this_dark;
        for (int i = MAX_CH + Llenght; i < LED_NUM; i++)
          leds[i] = this_dark;
      }
      break;
    case 2:
      for (int i = 0; i < LED_NUM; i++) {
        if (i < STRIPE)          leds[i] = CHSV(HIGH_COLOR, 255, thisBright[2]);
        else if (i < STRIPE * 2) leds[i] = CHSV(MID_COLOR, 255, thisBright[1]);
        else if (i < STRIPE * 3) leds[i] = CHSV(LOW_COLOR, 255, thisBright[0]);
        else if (i < STRIPE * 4) leds[i] = CHSV(MID_COLOR, 255, thisBright[1]);
        else if (i < STRIPE * 5) leds[i] = CHSV(HIGH_COLOR, 255, thisBright[2]);
      }
      break;
    case 3:
      for (int i = 0; i < LED_NUM; i++) {
        if (i < LED_NUM / 3)          leds[i] = CHSV(HIGH_COLOR, 255, thisBright[2]);
        else if (i < LED_NUM * 2 / 3) leds[i] = CHSV(MID_COLOR, 255, thisBright[1]);
        else if (i < LED_NUM)         leds[i] = CHSV(LOW_COLOR, 255, thisBright[0]);
      }
      break;
    case 4: 
      static uint32_t timer_waiting;
      static bool flag_one_start = true; 
      switch (mode_light_bedroom) {         
      case (NORMAL_LIGHT) :
        full_paint (red_color_now, green_color_now, blue_color_now);
        break;
      case (START_LIGHT) :
        if (flag_one_start)  {          //задержка на включение света
          flag_one_start = false;
          timer_waiting = millis(); 
          Brightness = 255;
          FastLED.setBrightness(Brightness);
        }
        if (millis() - timer_waiting >= 1000) 
          start_paint (red_color_now, green_color_now, 255, NORMAL_LIGHT);
        break;
      
      case (SUNRISE_LIGHT) :
        if (Brightness < 255) 
          {PERIOD(time_sunrise) {      //плавный рассвет
            Brightness++; 
            FastLED.setBrightness(Brightness);
        }} 
        {PERIOD (20) {
          static byte heat[LED_NUM];
        // Step 1.  Cool down every cell a little
          for( int i = 0; i < LED_NUM; i++) {
            heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / LED_NUM) + 2));
          }        
          // Step 2.  Heat from each cell drifts 'up' and diffuses a little
          for( int k= LED_NUM - 1; k >= 2; k--) {
            heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
          }          
          // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
          if( random8() < SPARKING ) {
            int y = random8(7);
            heat[y] = qadd8( heat[y], random8(160,255) );
          }
          // Step 4.  Map from heat cells to LED colors
          for( int j = 0; j < LED_NUM; j++) {
            CRGB color = HeatColor( heat[j]);
            int pixelnumber;
            if( gReverseDirection ) {
              pixelnumber = (LED_NUM-1) - j;
            } else {
              pixelnumber = j;
            }
            leds[pixelnumber] = color;
          }
        }}
        break;
        
      case (SUNSET_LIGHT) :
        start_paint (10, 200, 200, BLACKOUT_LIGHT); 
        break;
        
      case (BLACKOUT_LIGHT) :   //плавное затемнение
        {PERIOD(300) {
          Brightness--;
          FastLED.setBrightness(Brightness);  
          if (Brightness == 0)  
          mode_light_bedroom = OFF_LIGHT; 
        }}  
        break;      

      case (NIGHT_LIGHT) :      //ночная подсветка
        {PERIOD(200){ 
          if (Brightness < 50) { 
            Brightness++;
            for( int i = 0; i < LED_NUM; i++) 
            leds[i] = CHSV(0, 200, 255);
            FastLED.setBrightness(Brightness);     
          }
        }}
        break;

      case (OFF_LIGHT) :        //полное выключение света
        ONstate = false;
        flag_one_start = true;
        FastLED.clear();    
        EEPROM.updateByte(65, mode_light_bedroom); 
        break; 
      default :
        break;
      }
      break;
    case 5:
     break;
    case 6:
      switch (light_mode) {
        case 0: for (int i = 0; i < LED_NUM; i++) leds[i] = CHSV(LIGHT_COLOR, LIGHT_SAT, 255);
          break;
        case 1:
          if (millis() - color_timer > COLOR_SPEED) {
            color_timer = millis();
            if (++this_color > 255) this_color = 0;
          }
          for (int i = 0; i < LED_NUM; i++) leds[i] = CHSV(this_color, LIGHT_SAT, 255);
          break;
        case 2:
          if (millis() - rainbow_timer > 30) {
            rainbow_timer = millis();
            this_color += RAINBOW_PERIOD;
            if (this_color > 255) this_color = 0;
            if (this_color < 0) this_color = 255;
          }
          rainbow_steps = this_color;
          for (int i = 0; i < LED_NUM; i++) {
            leds[i] = CHSV((int)floor(rainbow_steps), 255, 255);
            rainbow_steps += RAINBOW_STEP_2;
            if (rainbow_steps > 255) rainbow_steps = 0;
            if (rainbow_steps < 0) rainbow_steps = 255;
          }
          break;
      }
      break;
    case 7:
      switch (freq_strobe_mode) {
        case 0:
          if (running_flag[2]) leds[LED_NUM / 2] = CHSV(HIGH_COLOR, 255, thisBright[2]);
          else if (running_flag[1]) leds[LED_NUM / 2] = CHSV(MID_COLOR, 255, thisBright[1]);
          else if (running_flag[0]) leds[LED_NUM / 2] = CHSV(LOW_COLOR, 255, thisBright[0]);
          else leds[LED_NUM / 2] = CHSV(EMPTY_COLOR, 255, EMPTY_BRIGHT);
          break;
        case 1:
          if (running_flag[2]) leds[LED_NUM / 2] = CHSV(HIGH_COLOR, 255, thisBright[2]);
          else leds[LED_NUM / 2] = CHSV(EMPTY_COLOR, 255, EMPTY_BRIGHT);
          break;
        case 2:
          if (running_flag[1]) leds[LED_NUM / 2] = CHSV(MID_COLOR, 255, thisBright[1]);
          else leds[LED_NUM / 2] = CHSV(EMPTY_COLOR, 255, EMPTY_BRIGHT);
          break;
        case 3:
          if (running_flag[0]) leds[LED_NUM / 2] = CHSV(LOW_COLOR, 255, thisBright[0]);
          else leds[LED_NUM / 2] = CHSV(EMPTY_COLOR, 255, EMPTY_BRIGHT);
          break;
      }
      leds[(LED_NUM / 2) - 1] = leds[LED_NUM / 2];
      if (millis() - running_timer > RUNNING_SPEED) {
        running_timer = millis();
        for (int i = 0; i < LED_NUM / 2 - 1; i++) {
          leds[i] = leds[i + 1];
          leds[LED_NUM - i - 1] = leds[i];
        }
      }
      break;
    case 8:
      byte HUEindex = HUE_START;
      for (int i = 0; i < LED_NUM / 2; i++) {
        byte this_bright = map(freq_f[(int)floor((LED_NUM / 2 - i) / freq_to_stripe)], 0, freq_max_f, 0, 255);
        this_bright = constrain(this_bright, 0, 255);
        leds[i] = CHSV(HUEindex, 255, this_bright);
        leds[LED_NUM - i - 1] = leds[i];
        HUEindex += HUE_STEP;
        if (HUEindex > 255) HUEindex = 0;
      }
      break;
  }
 }
//Serial 
 void serialRead() {
  if (Mega_serial.available()) {    // если данные получены
  //  /^a^a^234^...^crc;   формат принимаемой строки 
  //обработка crc
    byte length = strlen(Mega_serial.buf);
    byte crc_mega = crc8_bytes((byte*)&Mega_serial, length-1);  //расчёт crc входящего сообщения без последнего байта
    byte crc_esp = (byte)(Mega_serial.buf[length-1]);   
    if(crc_mega != crc_esp) 
    {
      serialWrite ("*");   
      return;
    }  

  //обработка сообщения
    char command1 = Mega_serial.buf[0]; 
      Serial.print("message send nano = "); 
      Serial.print(Mega_serial.buf[0]);
      Serial.print(" "); 
      Serial.println((byte)Mega_serial.buf[2]);
    byte am = Mega_parser.split();           //разделяeт строку на подстроки, заменяя разделители на NULL
    switch (command1)
    {
      case('a'):          //mode_light_bedroom
        mode_light_bedroom = (byte)Mega_serial.buf[2];
        this_mode = 4;
        ONstate = true;
        break;
      default:
        break;  
    }
    updateEEPROM();
  }
 }

 void serialWrite (const char* mes) {
   
    SerialMega.write(mes, strlen(mes));
 }
// функция для расчёта crc
 byte crc8_bytes(byte *buffer, byte size) {
  byte crc = 0;
  for (byte i = 0; i < size; i++) {
    byte data = buffer[i];
    for (int j = 8; j > 0; j--) {
      crc = ((crc ^ data) & 1) ? (crc >> 1) ^ 0x8C : (crc >> 1);
      data >>= 1;
    }
  }
  return crc;
 }
//EEPROM
 void updateEEPROM() {
  EEPROM.updateByte(1, this_mode);
  EEPROM.updateByte(2, freq_strobe_mode);
  EEPROM.updateByte(3, light_mode);
  EEPROM.updateInt(4, RAINBOW_STEP);
  EEPROM.updateFloat(8, MAX_COEF_FREQ);
  EEPROM.updateInt(12, STROBE_PERIOD);
  EEPROM.updateInt(16, LIGHT_SAT);
  EEPROM.updateFloat(20, RAINBOW_STEP_2);
  EEPROM.updateInt(24, HUE_START);
  EEPROM.updateFloat(28, SMOOTH);
  EEPROM.updateFloat(32, SMOOTH_FREQ);
  EEPROM.updateInt(36, STROBE_SMOOTH);
  EEPROM.updateInt(40, LIGHT_COLOR);
  EEPROM.updateInt(44, COLOR_SPEED);
  EEPROM.updateInt(48, RAINBOW_PERIOD);
  EEPROM.updateInt(52, RUNNING_SPEED);
  EEPROM.updateInt(56, HUE_STEP);
  EEPROM.updateInt(60, EMPTY_BRIGHT);
  if (KEEP_STATE) EEPROM.updateByte(64, ONstate);
  EEPROM.updateByte(65, mode_light_bedroom); 
  EEPROM.updateByte(66, Brightness);
 }
 void readEEPROM() {
  this_mode = EEPROM.readByte(1);
  freq_strobe_mode = EEPROM.readByte(2);
  light_mode = EEPROM.readByte(3);
  RAINBOW_STEP = EEPROM.readInt(4);
  MAX_COEF_FREQ = EEPROM.readFloat(8);
  STROBE_PERIOD = EEPROM.readInt(12);
  LIGHT_SAT = EEPROM.readInt(16);
  RAINBOW_STEP_2 = EEPROM.readFloat(20);
  HUE_START = EEPROM.readInt(24);
  SMOOTH = EEPROM.readFloat(28);
  SMOOTH_FREQ = EEPROM.readFloat(32);
  STROBE_SMOOTH = EEPROM.readInt(36);
  LIGHT_COLOR = EEPROM.readInt(40);
  COLOR_SPEED = EEPROM.readInt(44);
  RAINBOW_PERIOD = EEPROM.readInt(48);
  RUNNING_SPEED = EEPROM.readInt(52);
  HUE_STEP = EEPROM.readInt(56);
  EMPTY_BRIGHT = EEPROM.readInt(60);
  if (KEEP_STATE) ONstate = EEPROM.readByte(64);
  mode_light_bedroom = EEPROM.readByte(65);
  Brightness = EEPROM.readByte(66);
 }
 void eepromTick() {
  if (eeprom_flag)
    if (millis() - eeprom_timer > 5000) {  // 30 секунд после последнего нажатия с пульта
      eeprom_flag = false;
      eeprom_timer = millis();
      updateEEPROM();
    }
 }
//заполнение цветом  
 void full_paint (const int color , const int saturation, const int hue ) {
  for (int i = 0; i < LED_NUM; i++)         leds[i].setRGB(color, saturation, hue);
  for (int i = 0; i < LED_NUM_CENTR; i++)   leds_centr[i].setRGB(color, saturation, hue);

 }
 void start_paint (const int color, const int saturation, const int hue, uint8_t next ) {
   static uint8_t LED_LEFT =  LED_NUM / 2; 
   static uint8_t LED_RIGHT =  LED_NUM / 2;
   static uint8_t LED_CENTR;
   {PERIOD (300) { 
    if( LED_RIGHT < LED_NUM) {
      leds[LED_RIGHT++].setRGB( color, saturation ,hue); 
      leds[LED_LEFT--].setRGB( color, saturation ,hue); 
    }
    if (LED_RIGHT >= 79) {
      leds_centr[LED_CENTR++].setRGB( color, saturation ,hue);   
    }
   }}
   if (LED_CENTR == LED_NUM_CENTR)  {
    mode_light_bedroom = next;
    LED_LEFT =  LED_NUM / 2;
    LED_RIGHT =  LED_NUM / 2;
    LED_CENTR = 0;
   }
  } 
// вспомогательная функция, изменяет величину value на шаг incr в пределах minimum.. maximum
 int smartIncr(int value, int incr_step, int mininmum, int maximum) {
  int val_buf = value + incr_step;
  val_buf = constrain(val_buf, mininmum, maximum);
  return val_buf;
  }

 float smartIncrFloat(float value, float incr_step, float mininmum, float maximum) {
  float val_buf = value + incr_step;
  val_buf = constrain(val_buf, mininmum, maximum);
  return val_buf;
  }
//Ir remote
 //доп функция 
  void ColorLightingNow (byte red, byte green, byte blue)
   {
      red_color_now = red; 
      green_color_now = green;
      blue_color_now = blue;
      ONstate = true;
      this_mode = 4;
      mode_light_bedroom = NORMAL_LIGHT;
   }
 void remoteTick() {
  if (IRLremote.available())  {
    auto data = IRLremote.read();
    IRdata = data.command;
    ir_flag = true;
  }
  if (ir_flag) { // если данные пришли
    eeprom_timer = millis();
    eeprom_flag = true;
    switch (IRdata) {
    //Brightness
      case BUTT_HIGHT_UP : 
        if(Brightness == 0) 
          Brightness = 5;
        if(Brightness <= 245)
          Brightness +=10;
        FastLED.setBrightness(Brightness);
        break;
      case BUTT_HIGHT_DOWN : 
        if(Brightness >= 10)
          Brightness -=10;
          if(Brightness == 5)
            Brightness = 0;
        FastLED.setBrightness(Brightness);
        break;  
    //цвета
      case BUTT_RED_UP :
        red_color_now++;
        Serial.print(red_color_now);Serial.print(" ");Serial.print(green_color_now);Serial.print(" ");Serial.println(blue_color_now);
        break;
      case BUTT_RED_DOWN :
        red_color_now--;
        Serial.print(red_color_now);Serial.print(" ");Serial.print(green_color_now);Serial.print(" ");Serial.println(blue_color_now);
        break;
      case BUTT_GREEN_UP :
        green_color_now++;
        Serial.print(red_color_now);Serial.print(" ");Serial.print(green_color_now);Serial.print(" ");Serial.println(blue_color_now);
        break;
      case BUTT_GREEN_DOWN :
        green_color_now--;
        Serial.print(red_color_now);Serial.print(" ");Serial.print(green_color_now);Serial.print(" ");Serial.println(blue_color_now);
        break;
      case BUTT_BLUE_UP :
        blue_color_now++;
        Serial.print(red_color_now);Serial.print(" ");Serial.print(green_color_now);Serial.print(" ");Serial.println(blue_color_now);
        break;
      case BUTT_BLUE_DOWN :
        blue_color_now--;
        Serial.print(red_color_now);Serial.print(" ");Serial.print(green_color_now);Serial.print(" ");Serial.println(blue_color_now);
        break;
     //1 столбец
      case BUTT_RED : 
        ColorLightingNow(255,0,0);        
        break;  
      case BUTT_RED1 : 
        ColorLightingNow(255, 10,0);
        break;  
      case BUTT_RED2 : 
        ColorLightingNow(245,25,0);
        break;
      case BUTT_RED3 : 
        ColorLightingNow(240,39,2);
        break;  
      case BUTT_RED4 : 
        ColorLightingNow(255,255,0);
        break;  
     //2 столбец
      case BUTT_GREEN : 
        ColorLightingNow(0,100,0);
        break;
      case BUTT_GREEN1 : 
        ColorLightingNow(24, 105 , 15);
        break;  
      case BUTT_GREEN2 : 
        ColorLightingNow(255, 255 , 15);
        break;  
      case BUTT_GREEN3 : 
        ColorLightingNow(34, 98 , 225);
        break;  
      case BUTT_GREEN4 : 
        ColorLightingNow(40, 110 , 200);
        break;  
     //3 столбец      
      case BUTT_BLUE : 
        ColorLightingNow(0,0,128);
        break;    
      case BUTT_BLUE1 : 
        ColorLightingNow(12,120,255);
        break; 
      case BUTT_BLUE2 : 
        ColorLightingNow(255,0,10);
        break;    
      case BUTT_BLUE3 : 
        ColorLightingNow(70,24,122);
        break; 
      case BUTT_BLUE4 : 
        ColorLightingNow(255,100,255);
        break; 
     //4 столбец      
      case BUTT_WHITE : 
        ColorLightingNow(255,255,255);
        break; 
      case BUTT_WHITE1 : 
        ColorLightingNow(216,30,24);
        break; 
      case BUTT_WHITE2 : 
        ColorLightingNow(174,38,32);
        break; 
      case BUTT_WHITE3 : 
        ColorLightingNow(40,103,39);
        break; 
      case BUTT_WHITE4 : 
        ColorLightingNow(45,110,89);
        break; 
    // режимы
      case BUTT_1: this_mode = 0;
        break;
      case BUTT_2: this_mode = 1;
        break;
      case BUTT_3: this_mode = 2;
        break;
      case BUTT_4: this_mode = 3;
        break;
      case BUTT_5: this_mode = 4;
        break;
      case BUTT_6: this_mode = 5;
        break;
      case BUTT_7: this_mode = 6;
        break;
      case BUTT_8: this_mode = 7;
        break;
      case BUTT_9: this_mode = 8;
        break;
      case BUTT_0: fullLowPass();
        break;
      case BUTT_STAR: ONstate = !ONstate; FastLED.clear(); FastLED.show(); updateEEPROM();
        break;
      case BUTT_HASH:
        switch (this_mode) {
          case 4:
          case 7: if (++freq_strobe_mode > 3) freq_strobe_mode = 0;
            break;
          case 6: if (++light_mode > 2) light_mode = 0;
            break;
        }
        break;
      case BUTT_OK: digitalWrite(MLED_PIN, settings_mode ^ MLED_ON); settings_mode = !settings_mode;
        break;
      case BUTT_UP:
        if (settings_mode) {
          // ВВЕРХ общие настройки
          EMPTY_BRIGHT = smartIncr(EMPTY_BRIGHT, 5, 0, 255);
        } else {
          switch (this_mode) {
            case 0:
              break;
            case 1: RAINBOW_STEP = smartIncrFloat(RAINBOW_STEP, 0.5, 0.5, 20);
              break;
            case 2:
            case 3:
            case 4: MAX_COEF_FREQ = smartIncrFloat(MAX_COEF_FREQ, 0.1, 0, 5);
              break;
            case 5: STROBE_PERIOD = smartIncr(STROBE_PERIOD, 20, 1, 1000);
              break;
            case 6:
              switch (light_mode) {
                case 0: LIGHT_SAT = smartIncr(LIGHT_SAT, 20, 0, 255);
                  break;
                case 1: LIGHT_SAT = smartIncr(LIGHT_SAT, 20, 0, 255);
                  break;
                case 2: RAINBOW_STEP_2 = smartIncrFloat(RAINBOW_STEP_2, 0.5, 0.5, 10);
                  break;
              }
              break;
            case 7: MAX_COEF_FREQ = smartIncrFloat(MAX_COEF_FREQ, 0.1, 0.0, 10);
              break;
            case 8: HUE_START = smartIncr(HUE_START, 10, 0, 255);
              break;
          }
        }
        break;
      case BUTT_DOWN:
        if (settings_mode) {
          // ВНИЗ общие настройки
          EMPTY_BRIGHT = smartIncr(EMPTY_BRIGHT, -5, 0, 255);
        } else {
          switch (this_mode) {
            case 0:
              break;
            case 1: RAINBOW_STEP = smartIncrFloat(RAINBOW_STEP, -0.5, 0.5, 20);
              break;
            case 2:
            case 3:
            case 4: MAX_COEF_FREQ = smartIncrFloat(MAX_COEF_FREQ, -0.1, 0, 5);
              break;
            case 5: STROBE_PERIOD = smartIncr(STROBE_PERIOD, -20, 1, 1000);
              break;
            case 6:
              switch (light_mode) {
                case 0: LIGHT_SAT = smartIncr(LIGHT_SAT, -20, 0, 255);
                  break;
                case 1: LIGHT_SAT = smartIncr(LIGHT_SAT, -20, 0, 255);
                  break;
                case 2: RAINBOW_STEP_2 = smartIncrFloat(RAINBOW_STEP_2, -0.5, 0.5, 10);
                  break;
              }
              break;
            case 7: MAX_COEF_FREQ = smartIncrFloat(MAX_COEF_FREQ, -0.1, 0.0, 10);
              break;
            case 8: HUE_START = smartIncr(HUE_START, -10, 0, 255);
              break;
          }
        }
        break;
      case BUTT_LEFT:
        if (settings_mode) {
          // ВЛЕВО общие настройки
          Brightness = smartIncr(Brightness, -20, 0, 255);
          FastLED.setBrightness(Brightness);
        } else {
          switch (this_mode) {
            case 0:
            case 1: SMOOTH = smartIncrFloat(SMOOTH, -0.05, 0.05, 1);
              break;
            case 2:
            case 3:
            case 4: SMOOTH_FREQ = smartIncrFloat(SMOOTH_FREQ, -0.05, 0.05, 1);
              break;
            case 5: STROBE_SMOOTH = smartIncr(STROBE_SMOOTH, -20, 0, 255);
              break;
            case 6:
              switch (light_mode) {
                case 0: LIGHT_COLOR = smartIncr(LIGHT_COLOR, -10, 0, 255);
                  break;
                case 1: COLOR_SPEED = smartIncr(COLOR_SPEED, -10, 0, 255);
                  break;
                case 2: RAINBOW_PERIOD = smartIncr(RAINBOW_PERIOD, -1, -20, 20);
                  break;
              }
              break;
            case 7: RUNNING_SPEED = smartIncr(RUNNING_SPEED, -10, 1, 255);
              break;
            case 8: HUE_STEP = smartIncr(HUE_STEP, -1, 1, 255);
              break;
          }
        }
        break;
      case BUTT_RIGHT:
        if (settings_mode) {
          // ВПРАВО общие настройки
          Brightness = smartIncr(Brightness, 20, 0, 255);
          FastLED.setBrightness(Brightness);
        } else {
          switch (this_mode) {
            case 0:
            case 1: SMOOTH = smartIncrFloat(SMOOTH, 0.05, 0.05, 1);
              break;
            case 2:
            case 3:
            case 4: SMOOTH_FREQ = smartIncrFloat(SMOOTH_FREQ, 0.05, 0.05, 1);
              break;
            case 5: STROBE_SMOOTH = smartIncr(STROBE_SMOOTH, 20, 0, 255);
              break;
            case 6:
              switch (light_mode) {
                case 0: LIGHT_COLOR = smartIncr(LIGHT_COLOR, 10, 0, 255);
                  break;
                case 1: COLOR_SPEED = smartIncr(COLOR_SPEED, 10, 0, 255);
                  break;
                case 2: RAINBOW_PERIOD = smartIncr(RAINBOW_PERIOD, 1, -20, 20);
                  break;
              }
              break;
            case 7: RUNNING_SPEED = smartIncr(RUNNING_SPEED, 10, 1, 255);
              break;
            case 8: HUE_STEP = smartIncr(HUE_STEP, 1, 1, 255);
              break;
          }
        }
        break;
      default: eeprom_flag = false;   // если не распознали кнопку, не обновляем настройки!
        break;
    }
    ir_flag = false;
  }
 }


// --------разные режимы подсветки-------------
 void Color() {}

 void Cylon() {  
    fadeToBlackBy( leds, LED_NUM, 20);
    byte dothue = 0;
    for( int i = 0; i < 8; i++) {
      leds[beatsin16( i+7, 0, LED_NUM-1 )] |= CHSV(dothue, 200, 255);
      dothue += 32; }
 }
 void rainbow() {
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, LED_NUM, gHue, 7);
 }
 void addGlitter( int chanceOfGlitter) 
  {
  if( random8() < chanceOfGlitter) {
    leds[ random16(LED_NUM) ] += CRGB::White;
  } 
 }
 void rainbow_With_Glitter() 
  {
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
 }
 void confetti() 
  {
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, LED_NUM, 10);
  int pos = random16(LED_NUM);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
 }

 void sinelon()
  {
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, LED_NUM, 20);
  int pos = beatsin16( 13, 0, LED_NUM-1 );
  leds[pos] += CHSV( gHue, 255, 192);
 }

 void bpm()
  {
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < LED_NUM; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
 }

 void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, LED_NUM, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, LED_NUM-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
 }
 void color_Temperature () {
  // draw a generic, no-name rainbow
  static uint8_t starthue = 0;
  fill_rainbow( leds + 5, LED_NUM - 5, --starthue, 20);

  // Choose which 'color temperature' profile to enable.
  uint8_t secs = (millis() / 1000) % (DISPLAYTIME * 2);
  if( secs < DISPLAYTIME) {
    FastLED.setTemperature( TEMPERATURE_1 ); // first temperature
    leds[0] = TEMPERATURE_1; // show indicator pixel
  } else {
    FastLED.setTemperature( TEMPERATURE_2 ); // second temperature
    leds[0] = TEMPERATURE_2; // show indicator pixel
  }

  // Black out the LEDs for a few secnds between color changes
  // to let the eyes and brains adjust
  //if( (secs % DISPLAYTIME) < BLACKTIME) {
  //  memset8( leds, 0, LED_NUM * sizeof(CRGB));
  //}
 }
//
 void autoLowPass() {
  // для режима VU
  delay(10);                                // ждём инициализации АЦП
  int thisMax = 0;                          // максимум
  int thisLevel;
  for (byte i = 0; i < 200; i++) {
    thisLevel = analogRead(SOUND_R);        // делаем 200 измерений
    if (thisLevel > thisMax)                // ищем максимумы
      thisMax = thisLevel;                  // запоминаем
    delay(4);                               // ждём 4мс
  }
  LOW_PASS = thisMax + LOW_PASS_ADD;        // нижний порог как максимум тишины + некая величина

  // для режима спектра
  thisMax = 0;
  for (byte i = 0; i < 100; i++) {          // делаем 100 измерений
    analyzeAudio();                         // разбить в спектр
    for (byte j = 2; j < 32; j++) {         // первые 2 канала - хлам
      thisLevel = fht_log_out[j];
      if (thisLevel > thisMax)              // ищем максимумы
        thisMax = thisLevel;                // запоминаем
    }
    delay(4);                               // ждём 4мс
  }
  SPEKTR_LOW_PASS = thisMax + LOW_PASS_FREQ_ADD;  // нижний порог как максимум тишины
  if (EEPROM_LOW_PASS && !AUTO_LOW_PASS) {
    EEPROM.updateInt(70, LOW_PASS);
    EEPROM.updateInt(72, SPEKTR_LOW_PASS);
  }
 }

 void analyzeAudio() {
  for (int i = 0 ; i < FHT_N ; i++) {
    int sample = analogRead(SOUND_R_FREQ);
    fht_input[i] = sample; // put real data into bins
  }
  fht_window();  // window the data for better frequency response
  fht_reorder(); // reorder the data before doing the fht
  fht_run();     // process the data in the fht
  fht_mag_log(); // take the output of the fht
 }

 void fullLowPass() {
  digitalWrite(MLED_PIN, MLED_ON);   // включить светодиод
  FastLED.setBrightness(0); // погасить ленту
  FastLED.clear();          // очистить массив пикселей
  FastLED.show();           // отправить значения на ленту
  delay(500);               // подождать чутка
  autoLowPass();            // измерить шумы
  delay(500);               // подождать
  FastLED.setBrightness(Brightness);  // вернуть яркость
  digitalWrite(MLED_PIN, !MLED_ON);    // выключить светодиод
 }
