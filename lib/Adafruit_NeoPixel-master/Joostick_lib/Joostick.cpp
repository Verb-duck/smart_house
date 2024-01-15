#include "Joostick.h"

Joostik::Joostik (uint8_t pinx , uint8_t piny, uint8_t pinBtn) {
        _pinx = pinx; _piny = piny; _pinBtn = pinBtn;
        pinMode( _pinx, INPUT_PULLUP);
        pinMode( _piny, INPUT_PULLUP);
        pinMode( _pinBtn, INPUT_PULLUP);
      }
      void Joostik::debounce (uint16_t debounce ) {
        _debounce = debounce;
      }
   //считывание состояния кнопок
      void Joostik::j_click () {
        _flag_btn = !digitalRead (_pinBtn);
          if (_flag_btn) processing_click (&_flag_btn_click);;
        _x_position = analogRead( _pinx);
          if (_x_position > 700) processing_click (&_flag_down_click);
          else if (_x_position < 500)  processing_click (&_flag_up_click);
        _y_position = analogRead( _piny);
          if (_y_position > 700)processing_click (&_flag_right_click);
          else if (_y_position < 500) processing_click (&_flag_left_click);        
       }
      
  //отработка клика
      void Joostik::processing_click( bool *_flag_click) {
        if (!_flag_debounce) 
          {
            _flag_debounce = true;
            _timer = millis();
          }
          if (millis() - _timer >= _debounce) 
          {
             *_flag_click = true;
             _flag_act_click = true;
             _flag_debounce = false;
          }
      }
      
      
      bool Joostik::act_click () {
        if (_flag_act_click) 
        {
          _flag_act_click = false;
          return true;
        }
        else return false;
      }

      bool Joostik::click_up() {
        if ( _flag_up_click) {
          _flag_up_click = false;
          return true;
        }
        else return false;
      }
      bool Joostik::click_down() {
        if ( _flag_down_click) {
          _flag_down_click = false;
          return true;
        }
        else return false;
      }
      bool Joostik::click_left() {
        if ( _flag_left_click) {
          _flag_left_click = false;
          return true;
        }
        else return false;
      }
      bool Joostik::click_right() {
        if ( _flag_right_click) {
          _flag_right_click = false;
          return true;
        }
        else return false;
      }
      bool Joostik::click_btn() {
        if ( _flag_btn_click) {
          _flag_btn_click = false;
          return true;
        }
        else return false;
      }