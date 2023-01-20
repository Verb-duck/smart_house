#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX
#include <AsyncStream.h>
AsyncStream<100> serial(&mySerial); // указали Stream-объект и символ конца
#include <GParser.h>
GParser parser(serial.buf, ',');

void setup() {
  mySerial.begin(4000);
  Serial.begin(9600);

}

void loop() {

  strlen(serial.buf);
  if (serial.available()) {     // если данные получены
  // int am = parser.split();    // разделяем, получаем количество данных
   //Serial.print(parser[0]);
   //Serial.print(" ");
   Serial.println(serial.buf);
  }

}
