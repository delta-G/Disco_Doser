/*
 * MenuStrings.cpp
 *
 *  Created on: Aug 21, 2015
 *      Author: David
 */

#include "MenuClass.h"
#include "DoseMenu.h"

#define arrSize(x) (sizeof(x) / sizeof(x[0]))


const char setTime_MT[] PROGMEM = "Set Time";
MenuItem setTime_MI(setTime_MT, setTheTime);

const char M1_S2[] PROGMEM = "Schedule";

const char singleDose_MT[] PROGMEM = "Single Dose";
MenuItem singleDose_MI(singleDose_MT, singleDoseMenuItem)

const char M1_S4[] PROGMEM = "Booster";
const char M1_S5[] PROGMEM = "Pump";
const char M1_S6[] PROGMEM = "Container";
const char M1_S7[] PROGMEM = "Run";




