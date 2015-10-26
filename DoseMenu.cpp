/*
 * DoseMenu.cpp
 *
 *  Created on: Oct 25, 2015
 *      Author: David
 */

#include "DoseMenu.h"

MenuItem PROGMEM mainMenuItems[] = {
		{"Set Time" , setTheTime },
		{"Schedule" , gotoScheduleMenu },
		{"Single Dose" , singleDoseMenuItem },
		{"Booster" , setBoosterDoseMenuItem },
		{"Pump" , gotoPumpMenu },
		{"Container" , gotoContainerMenu },
		{"Run" , exitMenu }
};

MenuItem PROGMEM scheduleMenuItems[] = {
		{"Set Schedule" , setScheduleMenuItem },
		{"Set Name" , setScheduleNameMenuItem },
		{"Adjust Volume" , adjustVolumeMenuItem },
		{"Set Max Volume" , adjustMaxVolumeMenuItem },
		{"Show Schedule" , showScheduleMenuItem },
		{"Show State" , showScheduleStateMenuItem },
		{"Match Schedule" , matchScheduleMenuItem },
		{"Enable Schedule" , enableScheduleMenuItem },
		{"Clear Saved" , clearSavedScheduleMenuItem },
		{"Exit" , gotoMainMenu }

};

MenuItem PROGMEM pumpMenuItems[] = {
		{"Prime Pump" , primePumpMenuItem },
		{"Cal Pump" , calibratePumpMenuItem },
		{"Cal PWM" , calibratePwmMenuItem },
		{"Show Cal" , showCalibrationMenuItem },
		{"Exit" , gotoMainMenu }
};

MenuItem PROGMEM containerMenuItems[] = {
		{"Reset Cont" , resetContainerMenuItem },
		{"Add to Cont" , addToContainerMenuItem },
		{"Set Cont Vol" , setContainerVolumeMenuItem },
		{"Set Cont Size" , setContainerSizeMenuItem },
		{"Exit" , gotoMainMenu }
};



MenuList mainMenu(mainMenuItems, menuListSize(mainMenuItems));
MenuList scheduleMenu(scheduleMenuItems, menuListSize(scheduleMenuItems));
MenuList pumpMenu(pumpMenuItems, menuListSize(pumpMenuItems));
MenuList containerMenu(containerMenuItems, menuListSize(containerMenuItems));
