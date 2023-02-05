/*
 enum {
    MAIN_LCD , MENU , BRIGHTNESS, MODE_LIGHTING, TEMPERATURE  , 
  } mode_menu;
  
*/

void control_joustick(){
  joystick.j_click ();                       //опрос джостика
//возврат на главный экран при бездействии
 if (mode_menu != MAIN_LCD && (millis() - time_remove_mainLcd >= TIME_REMOVE_MAINLCD * 1000)) 
  mode_menu = MAIN_LCD;         
//выкключение подсветки при бездействии 
 if(flag_backlight_lcd && millis() - time_backlight_lcd >= TIME_BACKLIGTH_LCD *1000) {
  LCD.noBacklight();
  flag_backlight_lcd = false;
 }
 if (joystick.act_click()) {
  if(!flag_backlight_lcd) {
    LCD.backlight();
    joystick.reset_state_click();   //сброс состояний кнопок
    flag_backlight_lcd = true;
  }
  time_remove_mainLcd = millis();
  time_backlight_lcd = millis();
  LCD.clear();  
 }
 
 switch(mode_menu) {
  case (MAIN_LCD) :
  if (joystick.click_btn())  
  { 
    mode_menu = MENU; 
    line_menu = 0;
  }
  if (joystick.click_down() || joystick.click_up())  mode_menu = BRIGHTNESS;
  if (joystick.click_right() || joystick.click_left()) mode_menu = LED_PATTERN;
  break;
  
  case (MENU) :
  if(joystick.click_up())  line_menu--; 
  if(joystick.click_down())  line_menu++;
  if (joystick.click_btn()) menu_click = true;
  break;

  case (MODE_SCRIPT_HOUSE) :
  if (joystick.click_up()) {
    script_house = script_house.value +1;
    if ( script_house.value == LAST_SCRIP) {
      script_house = FIRST_SCRIP + 1;
      break;
    }
  }
  if (joystick.click_down()) {
    script_house = script_house.value - 1;
    if ( script_house.value == FIRST_SCRIP) {
      script_house = LAST_SCRIP - 1;
      break;
    }
  }
  break;

  case(TEMPERATURE) :
  if (joystick.click_up()) temperature_day += 1; 
  if (joystick.click_down()) temperature_day -= 1;
  
  case (BRIGHTNESS) :
  if(joystick.click_up() && Brightness < 254 )
  Brightness ++;
  if(joystick.click_down() && Brightness > 0 )
  Brightness --;
  break;

  case (MODE_LIGHTING) :
  break;

  case (LED_PATTERN) :
  if (joystick.click_right()) led_pattern_number++;
  if (joystick.click_left()) led_pattern_number--;
  if (joystick.click_up() && sped_led_show <= 200) sped_led_show += 5;
  if(joystick.click_down() && sped_led_show >= 10) sped_led_show -= 5;
  if (led_pattern_number >= quantity_led_pattern ) led_pattern_number = 0;
  if (led_pattern_number < 0 ) led_pattern_number = quantity_led_pattern - 1;
  break;

  default :
  break;
 }
 
}
