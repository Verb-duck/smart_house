#ifndef UartSerial_h
#define UartSerial_h
#include "AsyncStream.h"
#include "GParser.h"

//#define DEBUGING 1 
//#if (DEBUGING == 1)
//#define PRINT(title, y) \
//    Serial.print(title); \
//    Serial.print(": "); \
//    Serial.println(y);
//#else
//#define PRINT(titly, y)
//#endif

//�������� ������� �������� �������/���������� ��������

template<int Size>
class UartSerial : public GParser {
private:
    Stream* serial;
    AsyncStream<Size>* iStream;
    char outBuff[Size];             //buff save and send message
    byte lengthOut = 0;
    uint8_t attempt{ 0 }, max_attempt{ 50 };
public:
    UartSerial(Stream* serial) : GParser(iStream->buf, '^') {
        this->serial = serial;
        iStream = new AsyncStream<Size>(serial, ';', 50);   //���� ���������
        GParser::buf = iStream->buf;                      //��� ���,��� ��� ��� ��������� �������, ����� ������ ��� �� ����������
    }
    bool read() {
        if (iStream->available())
        {
            PRINT("incoming message: ", iStream->buf);
            if (iStream->buf[0] == 33 && iStream->buf[1] == 33)         //������ ������� �� ��������� �������� ���������, ������ 33? �� ������ ���
            {
                if (attempt <= max_attempt)
                {
                    attempt++;
                    serial->write(outBuff, lengthOut);     //������ ���������� 
                    PRINT("replay send message", "");
                    return false;
                }
                //�������� ������� ��������� � �������
                else          //��������� �� �����������                                   
                {
                    attempt = 0;
                    PRINT("message didnt send", "");
                    return false;
                }
            }
            if (iStream->buf[0] == 33 && iStream->buf[0] == 32)      //�������� ������ ��������� �� �����
            {
                PRINT("its ok, message recieve", "");
                return false;
            }
            byte length = strlen(iStream->buf);
            byte crc = crc8_bytes((byte*)iStream, length);
            if (crc != 0)                                     //��������� crc
            {
                if (attempt <= max_attempt)
                {
                    attempt++;
                    //�������� ������� �� ��������� �������� ���������
                    serial->write(33);  serial->write(33);  serial->write(';');
                    PRINT("problemm, attempt �", attempt);
                    return false;
                }
                //�������� ������� ��������� � �������
                else    //��������� �� ���������
                {
                    attempt = 0;
                    return false;
                }

            }
            split();
            //������������, ��� ��������� �������
            serial->write(33);  serial->write(32);  serial->write(';');
            attempt = 0;              //���������� ������� ������� ��������� ��������/�����
            PRINT("message ok", "");
            return true;
        }
        return false;
    }
    byte category() {
        return (byte)iStream->buf[0];
    }
    byte variable() {
        return (byte)iStream->buf[2];
    }
    bool getBool(int num) {
        return atol(str[num]);
    }
    void setMaxAttempt(uint8_t set) {        //������������ ����������� ������� ��������� �������� ���������
        max_attempt = set;
    }
    //�������� ���������       
    template<byte category = 255, byte variable = 255, class type1>
    void send(type1 value1) {
        lengthOut = 0;
        if (category != 255)
        {
            outBuff[lengthOut++] = category;  outBuff[lengthOut++] = '^';
        }
        if (variable != 255)
        {
            outBuff[lengthOut++] = variable;  outBuff[lengthOut++] = '^';
        }
        write_value_buff(value1);
        byte crc = crc8_bytes((byte*)&outBuff, lengthOut);
        outBuff[lengthOut++] = crc;
        outBuff[lengthOut++] = ';';
        serial->write(outBuff, lengthOut);         //send messege  
        PRINT("message send: ", outBuff);
    }
    template<byte category = 255, byte variable = 255, class type1, class type2>
    void send(type1 value1, type2 value2) {
        lengthOut = 0;
        if (category != 255)
        {
            outBuff[lengthOut++] = category;  outBuff[lengthOut++] = '^';
        }
        if (variable != 255)
        {
            outBuff[lengthOut++] = variable;  outBuff[lengthOut++] = '^';
        }
        write_value_buff(value1);
        write_value_buff(value2);
        byte crc = crc8_bytes((byte*)&outBuff, lengthOut);
        outBuff[lengthOut++] = crc;
        outBuff[lengthOut++] = ';';
        serial->write(outBuff, lengthOut);         //send messege  
        PRINT("message send: ", outBuff);
    }
    template<byte category = 255, byte variable = 255, class type1, class type2, class type3>
    void send(type1 value1, type2 value2, type3 value3) {
        lengthOut = 0;
        if (category != 255)
        {
            outBuff[lengthOut++] = category;  outBuff[lengthOut++] = '^';
        }
        if (variable != 255)
        {
            outBuff[lengthOut++] = variable;  outBuff[lengthOut++] = '^';
        }
        write_value_buff(value1);
        write_value_buff(value2);
        write_value_buff(value3);
        byte crc = crc8_bytes((byte*)&outBuff, lengthOut);
        outBuff[lengthOut++] = crc;
        outBuff[lengthOut++] = ';';
        serial->write(outBuff, lengthOut);         //send messege  
        PRINT("message send: ", outBuff);
    }
private:
    //������� ������ �������� � �����
    void write_value_buff(int value) {
        char temp[5];
        itoa(value, temp, DEC);  //��������������� ����� � char
        for (char* ptr(temp); *ptr != '\0'; ptr++)
            outBuff[lengthOut++] = *ptr;
        outBuff[lengthOut++] = '^';
    }
    void write_value_buff(double value) {
        char temp[20];
        dtostrf(value, 19, 3, temp);  //��������������� ����� � char
        for (char* ptr(temp); *ptr != '\0'; ptr++)
        {
            if (*ptr == ' ')
                continue;
            outBuff[lengthOut++] = *ptr;
        }
        outBuff[lengthOut++] = '^';
    }

    // ������� ��� ������� crc
    byte crc8_bytes(byte* buffer, byte size) {
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
};

#endif

