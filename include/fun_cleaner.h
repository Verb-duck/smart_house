#ifndef FUN_CLEANER_H
#define FUN_CLEANER_H
#include <arduino.h>

class Fun_cleaner {  
    public:
    Fun_cleaner( int pin) {
      _pin = pin;
      pinMode(_pin, OUTPUT);
    }
    void cleaner (bool on_off){
      _flag_work = on_off;
      digitalWrite (_pin, !_flag_work);  //стоит реле низкого сигнала, инвертируем     
    }   
    private:   
    int _pin;
    bool _flag_work ;
    
};
#endif