#ifndef MACROS_H
#define MACROS_H

//-------макросы---------
#define FULL_STRING 1
#define LIGHT_STRING 0
#define PERIOD(x) \
  static uint32_t tmr; \
  bool flag = millis() - tmr >= (x); \
  if (flag) tmr += (x); \
  if (flag)

#if (DEBUGING == 1)
#define PRINT(title, y) \
  Serial.print(title); \
  Serial.print(": "); \
  Serial.println(y);
#define LCDdebug "DEBUG"
#else
#define PRINT(titly, y)
#define LCDdebug ""
#endif

#endif