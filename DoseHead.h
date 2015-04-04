#ifndef DoseHead_h
#define DoseHead_h


// uncomment to use Serial for debugging instead of LCD.

//#define USING_SERIAL

/********  PIN ASSIGNMENTS   ********
 *
 * 0 -
 * 1 -
 * 2 - Encoder Interrupt Pin
 * 3 -
 * 4 - Encoder B Pin
 * 5 - Alk Pump
 * 6 - Cal Pump
 * 7 -
 * 8 - LCD Enable Pin
 * 9 - SS for RTC
 * 10 - SS for LCD
 * 11 - MOSI
 * 12 - MISO
 * 13 - SCK
 *
 *
 */


/********  EEPROM ADDRESSES  ********/

#define EA_CALIBRATION_FLAGS 16
#define EA_ALK_PUMP 18
#define EA_CA_PUMP 28
#define EA_ALK_SCHEDULE 50
#define EA_CA_SCHEDULE 80


/***********  CONSTANTS  ***********/
#define NUMBER_OF_PUMPS 2

//#define DEBOUNCE 50
#define DEFAULT_VOLUME 20.0
#define DEFAULT_RATE 20.0
#define MIDNIGHT 1440

#define DEFAULT_MAXIMUM_DAILY_VOLUME 120

#define MAXIMUM_CONTAINER_SIZE 5000

#define MAXIMUM_BOOSTER_DOSE 250
#define MAXIMUM_BOOSTER_DAYS 14

//#define DOSE_DELAY 30000ul

#define MAX_ALERTS 10
#define ALERT_DELAY 5000ul

#include "DoseInterface.h"
#include "TimeOfDay.h"
#include "DoseSchedule.h"
#include "DoseMenu.h"
#include "DoseInterface.h"
#include "DoseDisplay.h"
#include "DoseAlert.h"


typedef void (*S_Function)();

enum StateVar {
  RUN_STATE, MENU_STATE, DOSE_STATE, ALERT_STATE};

void alertHandler(DoseAlert*);
void removeAlert(int);


DoseSchedule* getSchedule(int);
void doRunStateUI();
void doDoseStateUI();
void doMenuStateUI();
void doAlertStateUI();
void doUI();
int setState(StateVar);
void figureTheColor();

void runDosers();

int freeRam();


#endif












