
#ifndef TimeOfDay_h
#define TimeOfDay_h

#include "Arduino.h"
#include "Time.h"
#define MIDNIGHT 1440



class TimeOfDay {
  
  public:
    
    int theTime;  // Time kept in the minutes since midnight format  uses 11 bits
    
    TimeOfDay(int tod);
    TimeOfDay(time_t tim);
    TimeOfDay();
    TimeOfDay(int, int);
    int getTime();
    int getHour();
    int getMinute();
    void setTime(int);
    void setTime(time_t);
    void setTime(int, int);
    void printMe(char* buf);
    static int lengthOfTime(TimeOfDay, TimeOfDay);
    
    
};

void timePrint(time_t, char*, char*);


    
#endif   
    
    
