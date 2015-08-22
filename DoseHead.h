#ifndef DoseHead_h
#define DoseHead_h


// uncomment to use Serial for debugging instead of LCD.

//#define USING_SERIAL
#define NEW_BOARD   // uses alternate pin assignments for home-brew board.


/********  PIN ASSIGNMENTS   ********
 *
 * 0 -
 * 1 -
 * 2 - Encoder Interrupt Pin
 * 3 - Encoder B Pin  // Moved here because something is wrong with 4 on my SMD board
 * 4 -
 * 5 - Alk Pump
 * 6 - Cal Pump
 * 7 - Button Pin
 * 8 - LCD Enable Pin
 * 9 - SS for RTC
 * 10 - SS for LCD
 * 11 - MOSI
 * 12 - MISO
 * 13 - SCK
 *
 * A0 - Heart Beat
 * A1 -
 * A2 -
 * A3 - LCD Backlight (Blue)
 * A4 - LCD Backlight (Green)
 * A5 - LCD Backlight (Red)
 *
 */

#ifdef NEW_BOARD

#define ENCODER_INTERRUPT_PIN 2
#define ENCODER_B_PIN 4
#define ALK_PUMP_PIN 6
#define CAL_PUMP_PIN 9
#define BUTTON_PIN 8
#define LCD_ENABLE_PIN A3
#define RTC_SS_PIN 10
#define LCD_SS_PIN A1

#define HEART_BEAT_PIN A5
#define LCD_BLUE A0
#define LCD_GREEN A2
#define LCD_RED A4

#else

#define ENCODER_INTERRUPT_PIN 2
#define ENCODER_B_PIN 3
#define ALK_PUMP_PIN 5
#define CAL_PUMP_PIN 6
#define BUTTON_PIN 7
#define LCD_ENABLE_PIN 8
#define RTC_SS_PIN 9
#define LCD_SS_PIN 10

#define HEART_BEAT_PIN A0
#define LCD_BLUE A3
#define LCD_GREEN A4
#define LCD_RED A5

#endif


/********  EEPROM ADDRESSES  ********/

#define EA_CALIBRATION_FLAGS 16
#define EA_ALK_PUMP 18
#define EA_CA_PUMP 28
#define EA_ALK_SCHEDULE 50  // also serving as SRAM address for state variables
#define EA_CA_SCHEDULE 80   // also serving as SRAM address for state variables


/***********  CONSTANTS  ***********/
#define NUMBER_OF_PUMPS 2

//#define CALIBRATION_TIME 30000ul   //Math requires hard coded value for now (see code)

#define BUTTON_DEBOUNCE 50
#define BUTTON_LONG_PRESS_TIME 2000
#define DEFAULT_VOLUME 20.0
#define DEFAULT_RATE 20.0
#define MIDNIGHT 1440

#define MAX_SINGLE_DOSE 100
#define SINGLE_DOSE_INCREMENT 10

#define MAXIMUM_VOLUME 999

#define DEFAULT_MAXIMUM_DAILY_VOLUME 120

#define DEFAULT_CONTAINER_SIZE 4000
#define MAXIMUM_CONTAINER_SIZE 9900
#define CONTAINER_SIZE_STEP 100

#define MAXIMUM_BOOSTER_DOSE 250
#define MAXIMUM_BOOSTER_DAYS 14

#define MESSAGE_DISPLAY_TIME 4000

//#define DOSE_DELAY 30000ul

#define MAX_ALERTS 10
#define ALERT_DELAY 5000ul

#include "DoseInterface.h"
#include "TimeOfDay.h"
#include "DoseSchedule.h"
//#include "DoseMenu.h"
#include "DoseDisplay.h"
#include "DoseAlert.h"


typedef void (*S_Function)();

enum StateVar {
  RUN_STATE, MENU_STATE, DOSE_STATE, ALERT_STATE};

void alertHandler(DoseAlert*);
void removeAlert(int);

void recordDose(char, unsigned long, byte);

DoseSchedule* getSchedule(int);
void doRunStateUI();
void doDoseStateUI();
void doMenuStateUI();
void doAlertStateUI();
void doUI();
int setState(StateVar);
void figureTheColor();

void buildRunStateDisplay(char*, char*);

void runDosers();

int freeRam();


#endif












