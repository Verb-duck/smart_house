#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX
#include <AsyncStream.h>
AsyncStream<40> serial(&mySerial); // указали Stream-объект и символ конца
#include <GParser.h>
GParser parser(serial.buf, ',');
char typeCommand ('z'); 

void setup() {
  mySerial.begin(4000);
  Serial.begin(9600);
  

}

void loop() {
 SerialRead() ;
 if (serial.available()) {     // если данные получены
    int count = parser.split();  
    for (byte i = 0; i < count; i++) 
    {
      Serial.print(parser[i]);
      Serial.print(":");
    }
    Serial.println();
  }  

}

void SerialRead() {
  if (Serial.available()) 
  {
    char t = Serial.read();
    mySerial.write('t');
  }  
}
