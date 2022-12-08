//CHSV  оттенок насыщенность яркость 
//CHSV(30, 255, 255);  //желтый
//
void light_bedroom() { 
  static uint32_t timer_waiting;
  static bool flag_one_start = true;
  
  switch (mode_light_bedroom) {

    case (NO_LIGHT) :
      break; 
      
    case (NORMAL_LIGHT) :
      normal_light();
      break; 
      
    
    case (START_LIGHT) :
      if (flag_one_start)  {          //задержка на включение света
        flag_one_start = false;
        timer_waiting = millis(); 
      }
      if (millis() - timer_waiting >= 1000) 
        paint_light (random_color, random_saturation, 255, NORMAL_LIGHT);
      break;
      
    case(SUNRISE_LIGHT) :
      sunrise_light();
      break;
      
    case(SUNSET_LIGHT) :
      paint_light (10, 229, 200, BLACKOUT_LIGHT); 
      break;
      
    case (BLACKOUT_LIGHT) :
      blackout_light();     
      break;      

    case(NIGHT_LIGHT) :
      night_light();
      break;

    case(OFF_LIGHT) :
      off_light();
      flag_one_start = true;
      break;
     
    default :
      break;
  }
  FastLED.setBrightness(Brightness);
  FastLED.show();
}

void paint_light (const int &color, const int &saturation, const int &hue, uint8_t next ) {
   Brightness = 255;
   static uint8_t LED_LEFT =  LED_NUM / 2; 
   static uint8_t LED_RIGHT =  LED_NUM / 2;
   static uint8_t LED_CENTR;
   {PERIOD (150) { 
    if( LED_RIGHT < LED_NUM) {
      leds[LED_RIGHT++]= CHSV( color, saturation ,hue); 
      leds[LED_LEFT--] = CHSV( color, saturation ,hue); 
    }
    if (LED_RIGHT >= 79) {
      leds_centr[LED_CENTR++] = CHSV( color, saturation ,hue);   
    }
    }}
   if (LED_CENTR == LED_NUM_CENTR)  {
    mode_light_bedroom = next;
    LED_LEFT =  LED_NUM / 2;
    LED_RIGHT =  LED_NUM / 2;
    LED_CENTR = 0;
   }
}

void normal_light() {
  {PERIOD (100) {
    if ( random_color != light_color_now || random_saturation != light_saturation_now) {
      if ( random_color != light_color_now)random_color ++;
      if ( random_saturation < light_saturation_now) random_saturation++;
      else if ( random_saturation > light_saturation_now) random_saturation--;
      for( int i = 0; i < LED_NUM_CENTR; i++) 
      leds_centr[i] = CHSV(random_color, random_saturation, 255); 
    }
  }}
   
    led_pattern[ led_pattern_number]();
    FastLED.delay(sped_led_show);
  }

void blackout_light() {
  PERIOD(2000) {
    Brightness--;  
    if (Brightness == 0)  
     mode_light_bedroom = OFF_LIGHT; 
  }  
}

void night_light() {
  PERIOD(100){ 
    if (Brightness < 100) { 
    Brightness++;
    for( int i = 0; i < LED_NUM; i++) 
      leds[i] = CHSV(0, 150, 255);    
    }
  }
}

void sunrise_light() {
  if (Brightness < 254) {
    PERIOD(time_sunrise) {      //плавный рассвет
      Brightness++; 
  }} 
  {PERIOD (90) {
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
}

void off_light() {
  Brightness = 0;
  FastLED.clear();
  mode_light_bedroom = NO_LIGHT;
}

// --------разные режимы подсветки-------------


void Cylon() {  
    fadeToBlackBy( leds, LED_NUM, 20);
    byte dothue = 0;
    for( int i = 0; i < 8; i++) {
      leds[beatsin16( i+7, 0, LED_NUM-1 )] |= CHSV(dothue, 200, 255);
      dothue += 32; }
}
void rainbow() 
{
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
