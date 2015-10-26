/*
 * DoseMenuFuncs.h
 *
 *  Created on: Oct 25, 2015
 *      Author: David
 */

#ifndef DOSEMENUFUNCS_H_
#define DOSEMENUFUNCS_H_

#include "Arduino.h"
#include "REBL_UI.h"
#include "DoseSchedule.h"
#include "DoseHead.h"
#include "DoseMenu.h"

void initMenu();
boolean exitMenu();
boolean gotoMainMenu();
boolean gotoScheduleMenu();
boolean gotoPumpMenu();
boolean gotoContainerMenu();


boolean inputTimeOfDay(TimeOfDay& var);


boolean setTheTime();
boolean singleDoseMenuItem();
boolean setBoosterDoseMenuItem();


boolean setScheduleMenuItem();
boolean setScheduleNameMenuItem();
boolean adjustVolumeMenuItem();
boolean adjustMaxVolumeMenuItem();
boolean showScheduleMenuItem();
boolean showScheduleStateMenuItem();
boolean matchScheduleMenuItem();
boolean enableScheduleMenuItem();
boolean clearSavedScheduleMenuItem();

boolean resetContainerMenuItem();
boolean addToContainerMenuItem();
boolean setContainerVolumeMenuItem();
boolean setContainerSizeMenuItem();

boolean primePumpMenuItem();
boolean calibratePumpMenuItem();
boolean calibratePwmMenuItem();
boolean showCalibrationMenuItem();
boolean pwmSelectMenuItem();


#endif /* DOSEMENUFUNCS_H_ */
