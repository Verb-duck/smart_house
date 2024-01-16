#ifndef Joostick_h
#define Joostick_h
#include <Arduino.h>

class Joostik {
    public:
    Joostik (uint8_t pinx , uint8_t piny, uint8_t pinBtn) ;
    void j_click ();                                   //считывание состояния кнопок
    void debounce (uint16_t debounce = 300 );
    void processing_click( bool *_flag_click);          //отработка клика
    bool act_click ();                                  //срабатывание любой кнопки
    bool click_up();
    bool click_down();
    bool click_left();
    bool click_right() ;
    bool click_btn();

    private:
      uint8_t _pinx;
      uint8_t _piny;
      uint8_t _pinBtn;
      bool _flag_btn;
      uint16_t _x_position;
      uint16_t _y_position;
      bool _flag_debounce = false;
      bool _flag_act_click = false;
      bool _flag_up_click = false;
      bool _flag_down_click = false;
      bool _flag_left_click = false;
      bool _flag_right_click = false;
      bool _flag_btn_click = false;
      uint16_t _debounce = 300;
      uint32_t _timer;
  };

#endif