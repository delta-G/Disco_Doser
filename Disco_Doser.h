// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef Disco_Doser_H_
#define Disco_Doser_H_
#include "Arduino.h"
//add your includes for the project Disco_Doser here
#include "REBL_UI.h"
#include "EEPROM.h"
#include "DosingPump.h"
#include "Time.h"
#include "TimeOfDay.h"
#include "LiquidCrystal_SPI_8Bit.h"
#include "githash.h"

#include "DoseSchedule.h"
#include "DoseHead.h"

#include "DoseAlert.h"
#include "DS3234RTC.h"
#include <avr/wdt.h>

//end of add your includes here


#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project Disco_Doser here


void heartbeat();
void splashScreen();


//Do not add code below this line
#endif /* Disco_Doser_H_ */
