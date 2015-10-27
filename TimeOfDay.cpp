
#include "TimeOfDay.h"

void TimeOfDay::printMe(char* buf){
  sprintf(buf, "%02d:%02d%n", theTime / 60, theTime % 60);
}

char* dayNames[] = {"Inv", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

void timePrint(time_t time, char* buf1, char* buf2){
  sprintf(buf1, "%02d:%02d:%02d%n", hour(time), minute(time), second(time));
  sprintf(buf2, "%s %2d-%2d-%4d%n", dayNames[weekday(time)], month(time), day(time), year(time));
}


TimeOfDay::TimeOfDay(){
  theTime = 0;
}

TimeOfDay::TimeOfDay(int tod){
  theTime = tod;
}

TimeOfDay::TimeOfDay(time_t tim){
  
  theTime = (((int)minute(tim)) + ((int)hour(tim)*60));
}

TimeOfDay::TimeOfDay(int aHour, int aMinute){
  theTime = (aHour * 60) + aMinute;
  theTime = (theTime + MIDNIGHT) % MIDNIGHT;
}




int TimeOfDay::getTime(){
    
  return theTime;
}

int TimeOfDay::getHour(){
  return theTime / 60;
}

int TimeOfDay::getMinute(){
  return theTime % 60;
}



void TimeOfDay::setTime(int tod){
  theTime = tod;
}

void TimeOfDay::setTime(time_t tim){
  theTime = (((int)minute(tim)) + ((int)hour(tim)*60));
}

void TimeOfDay::setTime(int aHour, int aMinute){
  theTime = aMinute + (aHour*60);
}


int TimeOfDay::lengthOfTime(TimeOfDay _start, TimeOfDay _end)
{
    //  Calculates the length of time between two numbers in tod_t format (minutes since midnight)
  
    if (_start.getTime() < _end.getTime())
    {
      return (_end.getTime() - _start.getTime());   // Simple case
    }
    else if (_start.getTime() > _end.getTime())
    {
      return ((MIDNIGHT - _start.getTime()) + _end.getTime());  //  Handle the rollover
    }
    else
    {
      return (MIDNIGHT);  //  If start = end, return length of day
    }
}
