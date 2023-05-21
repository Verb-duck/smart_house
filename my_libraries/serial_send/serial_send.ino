  #include "AsyncStream.h"
  #include "GParser.h"
  #include <SoftwareSerial.h>
  SoftwareSerial mySerial(7 , 8);   // RX, TX  

  template<int Size> 
  class UartSerial : public GParser {
    private:
    Stream* serial;
    AsyncStream<Size> *iStream;
    GParser *parser;
    char outBuff[Size];             //buff save and send message
    byte lengthOut = 0;
    public:
    UartSerial( Stream* serial) : GParser(iStream->buf, '^') {
      this -> serial = serial;
      iStream = new AsyncStream<Size>(serial,';',50);   //приём сообщения
    }
    void read() {
      if (iStream -> available()) 
      {
        byte length = strlen(iStream->buf);        
        byte crc = crc8_bytes((byte*)iStream,length);
        if(crc != 0)
        {
          //send(0 , 0);
          return;
        } 
        split();
      }
    }
    bool getBool(int num) {
        return atol(str[num]);
    }
    //отправка сообщения
     template<class type1>
     void send(byte category,  type1 value) {
      lengthOut = 0;     
      outBuff[lengthOut++] = category;
      outBuff[lengthOut++] = '^';
      write_value_buff(value);
      byte crc = crc8_bytes((byte*)&outBuff,lengthOut);
      outBuff[lengthOut++] = crc;
      outBuff[lengthOut++] = ';';
      serial->write(outBuff, lengthOut);        //send messege  
     }
     template<class type1>
     void send(byte category, byte variable, type1 value) {
      lengthOut = 0;     
      outBuff[lengthOut++] = category;
      outBuff[lengthOut++] = '^';
      outBuff[lengthOut++] = variable;
      outBuff[lengthOut++] = '^';
      write_value_buff(value);
      byte crc = crc8_bytes((byte*)&outBuff,lengthOut);
      outBuff[lengthOut++] = crc;
      outBuff[lengthOut++] = ';';
      serial->write(outBuff, lengthOut);        //send messege  
      Serial.print("message send = "); 
      Serial.println(outBuff);
     }
     template<class type1, class type2>
     void send(byte category, byte variable, type1 value,  type2 value2) {
      lengthOut = 0;     
      outBuff[lengthOut++] = category;
      outBuff[lengthOut++] = '^';
      outBuff[lengthOut++] = variable;
      outBuff[lengthOut++] = '^';
      write_value_buff(value);
      write_value_buff(value2);
      byte crc = crc8_bytes((byte*)&outBuff,lengthOut);
      outBuff[lengthOut++] = crc;
      outBuff[lengthOut++] = ';';
      serial->write(outBuff, lengthOut);        //send messege  
      Serial.print("message send = "); 
      Serial.println(outBuff);
     }
     template<class type1, class type2, class type3>
     void send(byte category, byte variable, type1 value, type2 value2, type3 value3) {
      lengthOut = 0;     
      outBuff[lengthOut++] = category;
      outBuff[lengthOut++] = '^';
      outBuff[lengthOut++] = variable;
      outBuff[lengthOut++] = '^';
      write_value_buff(value);
      write_value_buff(value2);
      write_value_buff(value3);
      byte crc = crc8_bytes((byte*)&outBuff,lengthOut);
      outBuff[lengthOut++] = crc;
      outBuff[lengthOut++] = ';';
      serial->write(outBuff, lengthOut);        //send messege  
      Serial.print("message send = "); 
      Serial.print(outBuff);
     }
    private:    
    //функции записи значения в буфер
      void write_value_buff (int value) {
        char temp[5];
        itoa(value,temp, DEC);  //преобразовываем число в char
        for(char* ptr(temp); *ptr != '\0'; ptr++)  
          outBuff[lengthOut++] = *ptr;
        outBuff[lengthOut++] = '^';
      }
      void write_value_buff (double value) {
        char temp[20];
        dtostrf(value,19,3,temp);  //преобразовываем число в char
        for(char* ptr(temp); *ptr != '\0'; ptr++)
        {
          if(*ptr == ' ') 
            continue;
          outBuff[lengthOut++] = *ptr;
        }       
        outBuff[lengthOut++] = '^';
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
  };
  UartSerial<50> serial(&mySerial);

  enum category {
    FIRST, temperature, now, day, nigth,
  };
void setup() {
  mySerial.begin(115200);
  Serial.begin(9600);
  
  
}

void loop() {
  serial.send(1,2, true, 2.2,true);
  delay(2000);
}
