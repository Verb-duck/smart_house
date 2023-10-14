#pragma once
#include <EEPROM.h>
template<class Type>
struct pair {
    Type value;
    int addres;
    char* name;
    pair(Type value = Type(), int addres = 255, char* enter_name = "free") : value(value), addres(addres) {
        size_t size = 0;
        while (enter_name[size]) {
            size++;
        }
        size++;
        name = new char[size];
        size = 0;
        while (enter_name[size]) {
            name[size] = enter_name[size];
            size++;
        }
        name[size] = '\0';
    }
    void operator =(Type value)
    {
        this->value = value;
        writeEEPROM(*this);
    }
    void operator +=(Type value)
    {
        this->value += value;
        writeEEPROM(*this);
    }
    void operator -=(Type value)
    {
        this->value -= value;
        writeEEPROM(*this);
    }

};
enum typeValue {
    BOOL = 1,  //1 byte 
    INT = 2,   //2 byte
    ENUM = 2,
    FLOAT = 4, // 4 byte 
};
//создание переменной с присвоением адреса в EEPROM
int next_addr = 2;       //0 занят для key_EEPROM
template <class Type>
pair<Type> create(const Type value, const int addres, const char* name) {
    return pair<Type>(value, addres, name);

}


template <class Type>
bool writeEEPROM(pair<Type>& pp) {
    EEPROM.update(pp.addres, pp.value);
    Serial.print(pp.name);
    Serial.print(" ");
    Serial.println(pp.value);
    return 0;
}

template <class Type>
bool readEEPROM(pair<Type>& pp) {
    EEPROM.get(pp.addres, pp.value);
    Serial.print(pp.name);
    Serial.print(" ");
    Serial.println(pp.value);
    return 0;
}
