  #include "AsyncStream.h"
  #include "GParser.h"
  #include <SoftwareSerial.h>
  SoftwareSerial mySerial(10 , 9);

  template<int Size> 
  class UartSerial {
    public:
    UartSerial( Stream* serial) {
      this -> serial = serial;
      iStream = new AsyncStream<Size>(serial,';',50); 
      parser = new GParser(iStream->buf, '^');
    }
    void read() {
      if (iStream -> available()) 
      {
        byte length = strlen(iStream->buf);        
        byte crc = crc8_bytes((byte*)iStream,length);
        if(crc != 0)
        {
          write(0 , 0);
          return;
        } 
        Serial.print((byte)iStream->buf[0]);
        Serial.print(" "); 
        Serial.println((byte)iStream->buf[2]);
      }
    }
    void write(byte command, int value) {
      lengthOut = 0;     
      outBuff[lengthOut++] = command;
      outBuff[lengthOut++] = '^';
      outBuff[lengthOut++] = value;
      outBuff[lengthOut++] = '^';
      byte crc = crc8_bytes((byte*)&outBuff,lengthOut);
      outBuff[lengthOut++] = crc;
      outBuff[lengthOut++] = ';';
      serial->write(outBuff, lengthOut);        //send messege  
      Serial.print("message send = "); 
      Serial.print(outBuff[0]);
      Serial.print(" "); 
      Serial.println((byte)outBuff[2]);
    }
    private:
    Stream* serial;
    AsyncStream<Size> *iStream;
    GParser *parser;
    char outBuff[Size];             //buff save and send message
    byte lengthOut = 0;
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

void setup() {
  mySerial.begin(115200);
  Serial.begin(115200);
}

void loop() {
  serial.read();
}
