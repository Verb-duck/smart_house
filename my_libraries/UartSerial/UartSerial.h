#ifndef UartSerial_h
#define UartSerial_h
#include "AsyncStream.h"
#include "GParser.h"

#define DEBUGING 1 
#if (DEBUGING == 1)
#define PRINT(title, y) \
    Serial.print(title); \
    Serial.print(": "); \
    Serial.println(y);
#else
#define PRINT(titly, y)
#endif

//�������� ������� �������� �������/���������� ��������

template<int Size>
class UartSerial : public GParser {
public:
    UartSerial(Stream* serial); 
    bool read();  //������ ��������� ���������, ���������� true ��� ����� ���������
    byte category() {return (byte)iStream->buf[0];}
    byte variable() {return (byte)iStream->buf[2];}
    bool getBool(int num) {return atol(str[num]);}
        //������������ ����������� ������� ��������� �������� ���������
    void setMaxAttempt(uint8_t set) {max_attempt = set;}
        //�������� ���������       
    template<byte category = 255, byte variable = 255, class type1>
        void send(type1 value1);
    template<byte category = 255, byte variable = 255, class type1, class type2>
        void send(type1 value1, type2 value2);
    template<byte category = 255, byte variable = 255, class type1, class type2, class type3>
        void send(type1 value1, type2 value2, type3 value3);
private:
    Stream* serial;
    AsyncStream<Size>* iStream;
    char outBuff[Size];                         //buff save and send message
    byte lengthOut = 0;                         //����� ��������� �� ��������
    uint8_t attempt{ 0 }, max_attempt{ 50 };
    void write_value_buff(int value);           //������� ������ �������� � �����
    void write_value_buff(double value);
    byte crc8_bytes(byte* buffer, byte size);    // ������� ��� ������� crc
};

#endif

