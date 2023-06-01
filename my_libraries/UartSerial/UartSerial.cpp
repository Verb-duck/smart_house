template<int Size>
 UartSerial<Size>::UartSerial(Stream* serial) : GParser(iStream->buf, '^') {
    this->serial = serial;
    iStream = new AsyncStream<Size>(serial, ';', 50);   //приём сообщения
    GParser::buf = iStream->buf;                      //ещё раз,так как при созданнии объекта, этого буфера ещё не существует
}

template<int Size>
 bool UartSerial<Size>::read()
{
    if (iStream->available())
    {
        PRINT("incoming message: ", iStream->buf);
        if (iStream->buf[0] == 33 && iStream->buf[1] == 33)         //пришла команда на повторную отправку сообщения, почему 33? да просто так
        {
            if (attempt <= max_attempt)
            {
                attempt++;
                serial->write(outBuff, lengthOut);     //заново отправляем 
                PRINT("replay send message", "");
                return false;
            }
            //добавить возврат сообщения о неудаче
            else          //сообщение не отправленно                                   
            {
                attempt = 0;
                PRINT("message didnt send", "");
                return false;
            }
        }
        if (iStream->buf[0] == 33 && iStream->buf[0] == 32)      //добавить выдачу сообщения на вверх
        {
            PRINT("its ok, message recieve", "");
            return false;
        }
        uint8_t length = strlen(iStream->buf);
        uint8_t crc = crc8_uint8_ts((uint8_t*)iStream, length);
        if (crc != 0)                                     //проверяем crc
        {
            if (attempt <= max_attempt)
            {
                attempt++;
                //отсылаем команду на повторную отправку сообщения
                serial->write(33);  serial->write(33);  serial->write(';');
                PRINT("problemm, attempt №", attempt);
                return false;
            }
            //добавить возврат сообщения о неудаче
            else    //сообщение не полученно
            {
                attempt = 0;
                return false;
            }
        }
        split();
        //подтверждаем, что сообщение принято
        serial->write(33);  serial->write(32);  serial->write(';');
        attempt = 0;              //сбрасываем счётчик попыток повторной отправки/приёма
        PRINT("message ok", "");
        return true;
    }
    return false;
}

template<int Size>
 void UartSerial<Size>::write_value_buff(int value)
{
    char temp[5];
    itoa(value, temp, DEC);  //преобразовываем число в char
    for (char* ptr(temp); *ptr != '\0'; ptr++)
        outBuff[lengthOut++] = *ptr;
    outBuff[lengthOut++] = '^';
}

template<int Size>
 void UartSerial<Size>::write_value_buff(double value)
{
    char temp[20];
    dtostrf(value, 19, 3, temp);  //преобразовываем число в char
    for (char* ptr(temp); *ptr != '\0'; ptr++)
    {
        if (*ptr == ' ')
            continue;
        outBuff[lengthOut++] = *ptr;
    }
    outBuff[lengthOut++] = '^';
}

template<int Size>
 uint8_t UartSerial<Size>::crc8_uint8_ts(uint8_t* buffer, uint8_t size)
{
    uint8_t crc = 0;
    for (uint8_t i = 0; i < size; i++) {
        uint8_t data = buffer[i];
        for (int j = 8; j > 0; j--) {
            crc = ((crc ^ data) & 1) ? (crc >> 1) ^ 0x8C : (crc >> 1);
            data >>= 1;
        }
    }
    return crc;
}

template<int Size>
template<uint8_t category, uint8_t variable, class type1>
 void UartSerial<Size>::send(type1 value1)
{
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
    uint8_t crc = crc8_uint8_ts((uint8_t*)&outBuff, lengthOut);
    outBuff[lengthOut++] = crc;
    outBuff[lengthOut++] = ';';
    serial->write(outBuff, lengthOut);         //send messege  
    PRINT("message send: ", outBuff);
}

template<int Size>
template<uint8_t category, uint8_t variable, class type1, class type2>
 void UartSerial<Size>::send(type1 value1, type2 value2)
{
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
    uint8_t crc = crc8_uint8_ts((uint8_t*)&outBuff, lengthOut);
    outBuff[lengthOut++] = crc;
    outBuff[lengthOut++] = ';';
    serial->write(outBuff, lengthOut);         //send messege  
    PRINT("message send: ", outBuff);
}

template<int Size>
template<uint8_t category, uint8_t variable, class type1, class type2, class type3>
 void UartSerial<Size>::send(type1 value1, type2 value2, type3 value3)
{
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
    uint8_t crc = crc8_uint8_ts((uint8_t*)&outBuff, lengthOut);
    outBuff[lengthOut++] = crc;
    outBuff[lengthOut++] = ';';
    serial->write(outBuff, lengthOut);         //send messege  
    PRINT("message send: ", outBuff);
}
