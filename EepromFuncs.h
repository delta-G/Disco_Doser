#ifndef EepromFuncs_h
#define EepromFuncs_h


#include "EEPROM.h"


template<class T>
int writeToEEPROM(int address, T& value)
{
  union {
    T type;
    byte b[sizeof(T)];
  } 
  temp;

  temp.type = value;
  for (unsigned int i = 0; i < sizeof(T); i++)
  {
    EEPROM.write(address + i, temp.b[i]);
  }
  return sizeof(T);
}


template<class T>
int readFromEEPROM(int address, T& value)
{
  union {
    T type;
    byte b[sizeof(T)];
  } 
  temp;

  for (unsigned int i = 0; i < sizeof(T); i++)
  {
    temp.b[i] = EEPROM.read(address + i);
  }
  value = temp.type;
  return sizeof(T);
}




#endif
