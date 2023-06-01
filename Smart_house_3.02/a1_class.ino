
#include <microDS18B20.h>
template <uint8_t HEATER_PIN , uint8_t DS_PIN = 255, uint8_t *DS_ADDR = (uint8_t*)nullptr>
class Heater  {
  private:
  int _pin_heater = HEATER_PIN;
  MicroDS18B20< DS_PIN, DS_ADDR>_sensor_dimmer;
  uint8_t _max_temp_DS = 90;                      //максимальная температура на датчике
  float temperature_now;
  int* temperature_set;
  bool flag_retenion_temp;
  int16_t timer_off = 6000;
  int16_t timer_ratio = 0;       //коэффициент удержания температуры
  uint32_t timer_now ;
  boolean flag_heater ;         
  
  bool request_sensor () {
    //-------DS18B20------- опрос датчиков температуры на диммере
    if (_sensor_dimmer.readTemp()) {            //читаем значение
      if(_sensor_dimmer.getTemp() > _max_temp_DS) {       //получил данные
        _sensor_dimmer.requestTemp();           // Запрашиваем преобразование температуры
        return false;
      }
      else {
        _sensor_dimmer.requestTemp();           // Запрашиваем преобразование температуры
        return true;
      }
    }
    else {                                      //датчик пропал
      _sensor_dimmer.requestTemp();             // Запрашиваем преобразование температуры
      return false;                 
    }
  }
  
  public:
  Heater ()  {
    pinMode(_pin_heater, OUTPUT);
    digitalWrite (_pin_heater, LOW);
  }
  void set_temperature(const int* temperature) {      //получаем указатель на заданную температуру в текущем режиме
    temperature_set = temperature;
    flag_retenion_temp = false; 
  }
  void work(float temperature_now) {
    if(request_sensor ()) {
       if ( !flag_retenion_temp) {             //и подгоняем температуру
         if ( *temperature_set > temperature_now * 10)  flag_heater = true;
         else   flag_heater = false;
         if (  *temperature_set - 1 == (int)temperature_now * 10 ) {
          flag_retenion_temp = true; //когда встала нужная температура
          timer_ratio = 0;
         }
       }                                            //включаем режим удержания    
       else {                                //режим удержания заданной температуры
        if (temperature_now * 10 > *temperature_set && timer_ratio > -5000) timer_ratio -= 1000;                  //корректировка коэффициента
        else if (temperature_now * 10 < *temperature_set && timer_ratio < 5000) timer_ratio += 1000 ; 
        if ( flag_heater && (millis() - timer_now > ( timer_off + timer_ratio))) {       //время включенного подогрева
          flag_heater = false;
          timer_now = millis();      
        }  
        if ( !flag_heater && (millis() - timer_now > ( timer_off - timer_ratio ))) {       //время выключенного подогрева
          flag_heater = true;  
          timer_now = millis();
        }        
      } 
    }
    else  flag_heater = false;

    digitalWrite(_pin_heater, flag_heater);
  }
};

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






