#ifndef DoseMenu_h
#define DoseMenu_h


#include "Arduino.h"
#include "DoseHead.h"
#include "DoseDisplay.h"
#include "DoseInterface.h"
#include "DoseSchedule.h"
#include "DS3234RTC.h"

#define LEAP_YEAR(Y)     ( ((1970+Y)>0) && !((1970+Y)%4) && ( ((1970+Y)%100) || !((1970+Y)%400) ) )





#include <avr/pgmspace.h>

//typedef prog_char menu_t;



enum { 
  SET_TIME, SCHEDULE, SINGLE_DOSE, BOOSTER, PUMP, CONTAINER, RUN};




enum { 
  SET_SCHEDULE, SET_SCHEDULE_NAME, ADJUST_VOLUME , ADJUST_MAX , SHOW_SCHEDULE ,  MATCH_SCHEDULE , ENABLE_SCHEDULE , SAVE_SCHEDULE, GET_SAVED, CLEAR_SAVED, EXIT_SCHED_MENU};



enum { 
  PRIME_PUMP , CALIBRATE_PUMP , CALIBRATE_PWM , EXIT_PUMP_MENU };



enum { 
  RESET_CONTAINER , ADD_TO_CONTAINER , SET_CONTAINER_VOLUME, SET_CONTAINER_SIZE, EXIT_CONT_MENU};





void initMenu();
void callMenu();
void doMenu();
void displayMenu();
const char * const* getMenuText(int, int);
int getMenuSize(int);

void notImplementedError();
void branch_Base();
void branch_Schedule();
void branch_Pump();
void branch_Container();

typedef void (*Branch_Function)();

enum {
  BASE_MENU, SCHEDULE_MENU, PUMP_MENU, CONTAINER_MENU};




boolean inputTime(time_t& var);
boolean inputTimeOfDay(TimeOfDay& var);


boolean setTheTime();
boolean singleDoseMenuItem();
boolean setBoosterDoseMenuItem();

boolean setScheduleMenuItem();
boolean setScheduleNameMenuItem();
boolean adjustVolumeMenuItem();
boolean adjustMaxVolumeMenuItem();
boolean showScheduleMenuItem();
boolean matchScheduleMenuItem();
boolean enableScheduleMenuItem();

boolean resetContainerMenuItem();
boolean addToContainerMenuItem();
boolean setContainerVolumeMenuItem();
boolean setContainerSizeMenuItem();

boolean primePumpMenuItem();
boolean calibratePumpMenuItem();
boolean calibratePwmMenuItem();
boolean pwmSelectMenuItem();



#endif
