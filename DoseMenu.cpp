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




void initMenu(){
	reblMenu.setCurrentMenu(&mainMenu);
}

boolean exitMenu() {
	setState(RUN_STATE);
	return true;
}

boolean gotoMainMenu() {
	reblMenu.setCurrentMenu(&mainMenu);
	return true;
}

boolean gotoScheduleMenu() {
	reblMenu.setCurrentMenu(&scheduleMenu);
	return true;
}

boolean gotoPumpMenu() {
	reblMenu.setCurrentMenu(&pumpMenu);
	return true;
}

boolean gotoContainerMenu() {
	reblMenu.setCurrentMenu(&containerMenu);
	return true;
}

boolean inputTimeOfDay(TimeOfDay& var) {

	static int state = 0;
	static int setHour;
	static int setMinute;

	switch (state) {

	case 0: {      // get set up
		encoderOn();
		buttonOn();
		setHour = var.getHour();
		setMinute = var.getMinute();
		state++;
		break;
	}
	case 1: {  // input the hours

		if (checkButton()) {

			state++;
			break;
		}
		useRotaryEncoder(setHour, 0, 23);
		break;
	}
	case 2: {  // input the minutes

		if (checkButton()) {

			state++;
			break;
		}
		useRotaryEncoder(setMinute, 0, 59);
		break;
	}
	case 3: { // wrap it up and let the caller know we're done with his variable. var should still be OK from the last time we touched it.
		state = 0;
		encoderOff();
		buttonOff();
		return true;
	}

	}  // end of switch (state)

	if (reblMenu.cancelFlag) {
		state = 0;
		encoderOff();
		buttonOff();
		return true;
	}

	var.setTime(setHour, setMinute);
	return false;
}

boolean setTheTime() {
	static time_t timeSetTime;
	static int state = 0;

	switch (state) {
	case 0: {
		timeSetTime = now();
		state++;
		//  No break, go ahead and run the next case too.
	}
		/* no break */
	case 1: {
		if (inputTime(timeSetTime)) {
			state++;
		}
		break;
	}
	case 2: {  // require one more button press to confirm.
		if (!isButtonOn()) {
			buttonOn();
		}
		if (checkButton()) {
			//setTime(timeSetTime);
			RTC.set(timeSetTime);
			setSyncProvider(RTC.get);  // Forces a sync at next check of time
			buttonOff();
			state = 0;
			// reinitialize schedules.
			for (int i = 0; i < NUMBER_OF_PUMPS; i++) {
				getSchedule(i)->initSchedule();
			}
			return true;
		}
		break;
	}
	}  // end swtich (state)

	if (reblMenu.cancelFlag) {
		state = 0;
		buttonOff();
		return true;
	}

	return false;
}

boolean singleDoseMenuItem() {

	static int scheduleChoice = 0;
	static int volumeChoice = 0;

	static int state = 0;

	switch (state) {

	case 0: {
		encoderOn();
		buttonOn();
		scheduleChoice = 0;
		volumeChoice = 0;
		state++;
		break;
	}
	case 1: {
		if (checkButton()) {

			state++;
			break;
		}
		displayLineLeft(0, F("Choose Pump:"));
		useRotaryEncoder(scheduleChoice, 0, NUMBER_OF_PUMPS - 1);
		displayLineLeft(1, getSchedule(scheduleChoice)->getName());
		break;
	}
	case 2: {   // Set by tens
		if (checkButton()) {

			state++;
			break;
		}
		displayLineLeft(0, F("Choose Volume:"));
		useRotaryEncoder(volumeChoice, 0, MAX_SINGLE_DOSE,
				SINGLE_DOSE_INCREMENT);
		char buf[6];
		sprintf_P(buf, PSTR("%03d%n"), volumeChoice);
		displayLineLeft(1, buf);
		setCursor(1, 1);
		break;

	}
	case 3: {    // Set by ones
		if (checkButton()) {

			state++;
			break;
		}
		displayLineLeft(0, F("Choose Volume:"));
		useRotaryEncoder(volumeChoice, 0, MAX_SINGLE_DOSE);
		char buf[6];
		sprintf_P(buf, PSTR("%03d%n"), volumeChoice);
		displayLineLeft(1, buf);
		setCursor(1, 2);
		break;

	}
	case 4: {
		getSchedule(scheduleChoice)->singleDose(volumeChoice);
		state = 0;
		encoderOff();
		buttonOff();
		showCursor(false);
		return true;
	}
	} // end switch (state)

	if (reblMenu.cancelFlag) {
		state = 0;
		encoderOff();
		buttonOff();
		showCursor(false);
		return true;
	}

	return false;

}

boolean setBoosterDoseMenuItem() {

	static int scheduleChoice = 0;
	static int volumeChoice = 0;
	static int daysChoice = 1;

	static int state = 0;

	switch (state) {

	case 0: {
		encoderOn();
		buttonOn();
		scheduleChoice = 0;
		volumeChoice = 0;
		daysChoice = 1;
		state++;
		break;
	}
	case 1: {
		if (checkButton()) {

			state++;
			break;
		}
		displayLineLeft(0, F("Choose Sched:"));
		useRotaryEncoder(scheduleChoice, 0, NUMBER_OF_PUMPS - 1);
		displayLineLeft(1, getSchedule(scheduleChoice)->getName());
		break;
	}
	case 2: {
		if (checkButton()) {

			state++;
			break;
		}
		displayLineLeft(0, F("Total Volume:"));

		int maxAllow = ((getSchedule(scheduleChoice)->getMaxVolume()
				- getSchedule(scheduleChoice)->getDailyVolume())
				* (MAXIMUM_BOOSTER_DAYS - 1));
		if (maxAllow > MAXIMUM_BOOSTER_DOSE) {
			maxAllow = MAXIMUM_BOOSTER_DOSE;
		}

		int minAllow = -(getSchedule(scheduleChoice)->getDailyVolume()
				* (MAXIMUM_BOOSTER_DAYS - 1));

		useRotaryEncoder(volumeChoice, minAllow, maxAllow);
		char buf[11];
		sprintf_P(buf, PSTR("Change %+03d%n"), volumeChoice);
		displayLineLeft(1, buf);
		break;

	}
	case 3: {
		if (checkButton()) {
			state++;
			break;
		}
		displayLineLeft(0, F("Spread Over"));
		int minDays;
		if (volumeChoice > 0) {
			minDays = (volumeChoice
					/ (getSchedule(scheduleChoice)->getMaxVolume()
							- getSchedule(scheduleChoice)->getDailyVolume()))
					+ 1;
		} else {
			minDays = (volumeChoice
					/ getSchedule(scheduleChoice)->getDailyVolume()) + 1;
		}

		useRotaryEncoder(daysChoice, minDays, MAXIMUM_BOOSTER_DAYS);
		char buf[8];
		sprintf_P(buf, PSTR("%d Days%n"), daysChoice);
		displayLineLeft(1, buf);
		break;
	}
	case 4: {
		getSchedule(scheduleChoice)->createBooster(volumeChoice, daysChoice);
		state = 0;
		buttonOff();
		encoderOff();
		return true;
	}

	} // end switch (state)

	if (reblMenu.cancelFlag) {
		state = 0;
		encoderOff();
		buttonOff();
		return true;
	}
	return false;
}

boolean setScheduleMenuItem() {
	static int scheduleChoice = 0;
	static TimeOfDay startTimeChoice(0);
	static TimeOfDay endTimeChoice(0);
	static TimeOfDay intervalChoice(0);
	static int volumeChoice = 0;

	static int state = 0;

	switch (state) {

	case 0: {
		encoderOn();
		buttonOn();
		scheduleChoice = 0;
		volumeChoice = 0;
		state++;
		break;
	}
	case 1:   // choose a schedule
	{
		if (checkButton()) {   // schedule chosen
			startTimeChoice = getSchedule(scheduleChoice)->getStartTime();
			endTimeChoice = getSchedule(scheduleChoice)->getEndTime();
			intervalChoice = getSchedule(scheduleChoice)->getInterval();
			volumeChoice = getSchedule(scheduleChoice)->getDailyVolume();
			state++;
			break;
		}
		displayLineLeft(0, F("Choose Sched:"));
		useRotaryEncoder(scheduleChoice, 0, NUMBER_OF_PUMPS - 1);
		displayLineLeft(1, getSchedule(scheduleChoice)->getName());
		break;
	}

	case 2: {
		if (inputTimeOfDay(startTimeChoice)) {
			state++;
			break;
		}
		displayLineLeft(0, F("Start Time:"));
		char buf[NUM_LCD_COLS + 1];
		startTimeChoice.printMe(buf);
		displayLineLeft(1, buf);
		break;
	}

	case 3: {
		if (inputTimeOfDay(endTimeChoice)) {
			state++;
			break;
		}
		displayLineLeft(0, F("End Time:"));
		char buf[NUM_LCD_COLS + 1];
		endTimeChoice.printMe(buf);
		displayLineLeft(1, buf);
		break;
	}

	case 4: {
		if (inputTimeOfDay(intervalChoice)) {
			encoderOn();  // since inputTimeOfDay turns them off.
			buttonOn();
			state++;
			break;
		}
		displayLineLeft(0, F("Interval:"));
		char buf[NUM_LCD_COLS + 1];
		intervalChoice.printMe(buf);
		displayLineLeft(1, buf);
		break;
	}
	case 5: {
		if (checkButton()) {

			state++;
			break;
		}
		displayLineLeft(0, F("Choose Volume:"));
		useRotaryEncoder(volumeChoice, 0,
				getSchedule(scheduleChoice)->getMaxVolume());
		char buf[6];
		sprintf_P(buf, PSTR("%03d%n"), volumeChoice);
		displayLineLeft(1, buf);
		break;

	}
	case 6: {
		getSchedule(scheduleChoice)->setSchedule(startTimeChoice, endTimeChoice,
				intervalChoice, volumeChoice);
		state = 0;
		buttonOff();
		encoderOff();
		return true;
	}

	} // end switch (state)

	if (reblMenu.cancelFlag) {
		state = 0;
		encoderOff();
		buttonOff();
		return true;
	}

	return false;
}

boolean setScheduleNameMenuItem() {

	static int scheduleChoice = 0;
	static char* namePtr;
	static int index;

	static int state = 0;

	switch (state) {

	case 0: {
		encoderOn();
		buttonOn();
		scheduleChoice = 0;
		index = 0;
		state++;
		break;
	}

	case 1: {
		if (checkButton()) {

			state++;
			namePtr = getSchedule(scheduleChoice)->getName();
			break;
		}
		displayLineLeft(0, F("Choose Sched:"));
		useRotaryEncoder(scheduleChoice, 0, NUMBER_OF_PUMPS - 1);
		displayLineLeft(1, getSchedule(scheduleChoice)->getName());
		break;
	}

	case 2: {
		if (checkButton()) {

			index++;
			if (index >= 3) {
				state = 0;
				namePtr[3] = 0;
				return true;
			}
		}

		displayLineLeft(0, F("Enter Name"));
		useRotaryEncoderASCII(namePtr[index]);
		displayLineLeft(1, namePtr);
		break;
	}

	} // end switch(state)

	return false;
}

boolean adjustVolumeMenuItem() {

	static int scheduleChoice = 0;
	static int volumeChoice = 0;

	static int state = 0;

	switch (state) {

	case 0: {
		encoderOn();
		buttonOn();
		scheduleChoice = 0;
		volumeChoice = 0;
		state++;
		break;
	}
	case 1: {
		if (checkButton()) {

			state++;
			volumeChoice = getSchedule(scheduleChoice)->getDailyVolume();
			break;
		}
		displayLineLeft(0, F("Choose Sched:"));
		useRotaryEncoder(scheduleChoice, 0, NUMBER_OF_PUMPS - 1);
		displayLineLeft(1, getSchedule(scheduleChoice)->getName());
		break;
	}
	case 2: {
		if (checkButton()) {

			state++;
			break;
		}
		displayLineLeft(0, F("Choose Volume:"));
		useRotaryEncoder(volumeChoice, 0,
				getSchedule(scheduleChoice)->getMaxVolume());
		char buf[6];
		sprintf_P(buf, PSTR("%03d%n"), volumeChoice);
		displayLineLeft(1, buf);
		break;

	}
	case 3: {
		getSchedule(scheduleChoice)->setDailyVolume(volumeChoice);
		state = 0;
		return true;
	}

	} // end switch (state)

	if (reblMenu.cancelFlag) {
		state = 0;
		encoderOff();
		buttonOff();
		return true;
	}

	return false;
}

boolean adjustMaxVolumeMenuItem() {

	static int scheduleChoice = 0;
	static int volumeChoice = 0;

	static int state = 0;

	switch (state) {

	case 0: {
		encoderOn();
		buttonOn();
		scheduleChoice = 0;
		volumeChoice = 0;
		state++;
		break;
	}
	case 1: {
		if (checkButton()) {

			state++;
			volumeChoice = getSchedule(scheduleChoice)->getMaxVolume();
			break;
		}
		displayLineLeft(0, F("Choose Sched:"));
		useRotaryEncoder(scheduleChoice, 0, NUMBER_OF_PUMPS - 1);
		displayLineLeft(1, getSchedule(scheduleChoice)->getName());
		break;
	}
	case 2: {
		if (checkButton()) {

			state++;
			break;
		}
		displayLineLeft(0, F("Choose Volume:"));
		useRotaryEncoder(volumeChoice, 0, MAXIMUM_VOLUME);
		char buf[6];
		sprintf_P(buf, PSTR("%03d%n"), volumeChoice);
		displayLineLeft(1, buf);
		break;

	}
	case 3: {
		getSchedule(scheduleChoice)->setMaxVolume(volumeChoice);
		state = 0;
		return true;
	}

	} // end switch (state)

	if (reblMenu.cancelFlag) {
		state = 0;
		encoderOff();
		buttonOff();
		return true;
	}

	return false;
}

boolean showScheduleMenuItem() {

	static int scheduleChoice = 0;

	static int state = 0;

	switch (state) {

	case 0: {
		encoderOn();
		buttonOn();
		scheduleChoice = 0;
		state++;
		break;
	}
	case 1: {
		if (checkButton()) {

			state++;
			break;
		}
		displayLineLeft(0, F("Choose Sched:"));
		useRotaryEncoder(scheduleChoice, 0, NUMBER_OF_PUMPS - 1);
		displayLineLeft(1, getSchedule(scheduleChoice)->getName());
		break;
	}
	case 2: {
		if (checkButton()) {
			buttonOff();
			encoderOff();
			state = 0;
			return true;
		}
		char bufs[2][NUM_LCD_COLS + 1];
		sprintf_P(bufs[0], PSTR("%02d:%02d to %02d:%02d%n"),
				getSchedule(scheduleChoice)->getStartTime().getHour(),
				getSchedule(scheduleChoice)->getStartTime().getMinute(),
				getSchedule(scheduleChoice)->getEndTime().getHour(),
				getSchedule(scheduleChoice)->getEndTime().getMinute());
		sprintf_P(bufs[1], PSTR("%03dmL I: %02d:%02d%n"),
				getSchedule(scheduleChoice)->getDailyVolume(),
				getSchedule(scheduleChoice)->getInterval().getHour(),
				getSchedule(scheduleChoice)->getInterval().getMinute());
		displayLineLeft(0, bufs[0]);
		displayLineLeft(1, bufs[1]);
		break;

	}

	} // end switch (state)

	return false;

}

boolean showScheduleStateMenuItem() {

	static int scheduleChoice = 0;

	static int state = 0;

	switch (state) {

	case 0: {
		encoderOn();
		buttonOn();
		scheduleChoice = 0;
		state++;
		break;
	}
	case 1: {
		if (checkButton()) {

			state++;
			break;
		}
		displayLineLeft(0, F("Choose Sched:"));
		useRotaryEncoder(scheduleChoice, 0, NUMBER_OF_PUMPS - 1);
		displayLineLeft(1, getSchedule(scheduleChoice)->getName());
		break;
	}
	case 2: {
		if (checkButton()) {
			buttonOff();
			encoderOff();
			state = 0;
			return true;
		}
		char bufs[2][NUM_LCD_COLS + 1];
		sprintf_P(bufs[0], PSTR("Last %02d:%02d"),
				getSchedule(scheduleChoice)->getLastTime().getHour(),
				getSchedule(scheduleChoice)->getLastTime().getMinute());
		sprintf_P(bufs[1], PSTR("Boost %03d in %02d%n"),
				getSchedule(scheduleChoice)->getBoosterVolume(),
				getSchedule(scheduleChoice)->getBoosterDays());
		displayLineLeft(0, bufs[0]);
		displayLineLeft(1, bufs[1]);
		break;

	}
	} // end switch (state)

	return false;

}

boolean matchScheduleMenuItem() {

	static int state = 0;
	static boolean choice = true;

	switch (state) {
	case 0: {
		choice = true;
		encoderOn();
		buttonOn();
		state++;
		// No break needed, go ahead and run next state as well.
	}
		/* no break */
	case 1: {
		if (checkButton()) {

			state++;
			break;
		}
		if (checkRotaryEncoder()) {
			choice = !choice;
		}
		displayLineLeft(0, F("Confirm Match"));
		displayLineLeft(1, choice ? F("->Yes"):F("->No"));
		break;
	}
	case 2: {
		if (choice) {
			int setStart = getSchedule(0)->getStartTime().getTime();
			int setEnd = getSchedule(0)->getEndTime().getTime();
			int setInterval = getSchedule(0)->getInterval().getTime();
			int setVol = getSchedule(0)->getDailyVolume();

			setStart = (setStart + (setInterval / 2)) % MIDNIGHT;
			setEnd = (setEnd + (setInterval / 2)) % MIDNIGHT;

			TimeOfDay sTime(setStart);
			TimeOfDay eTime(setEnd);
			TimeOfDay iTime(setInterval);

			getSchedule(1)->setSchedule(sTime, eTime, iTime, setVol);
		}
		state = 0;
		buttonOff();
		encoderOff();
		return true;
	}

	}  // end switch (state)
	return false;
}

boolean enableScheduleMenuItem() {

	static int scheduleChoice = 0;
	static boolean choice = true;

	static int state = 0;

	static int prevMillis;

	switch (state) {

	case 0: {
		encoderOn();
		buttonOn();
		scheduleChoice = 0;
		choice = true;
		state++;
		break;
	}
	case 1: {
		if (checkButton()) {
			if (!(getSchedule(scheduleChoice)->isCal())) {
				state = 3;
				break;
			}
			state++;
			break;
		}
		displayLineLeft(0, F("Choose Sched:"));
		useRotaryEncoder(scheduleChoice, 0, NUMBER_OF_PUMPS - 1);
		displayLineLeft(1, getSchedule(scheduleChoice)->getName());
		break;
	}
	case 2: {
		if (checkButton()) {
			buttonOff();
			encoderOff();

			getSchedule(scheduleChoice)->setEnabled(choice);

			state = 0;
			return true;
		}
		if (checkRotaryEncoder()) {
			choice = !choice;
		}
		char buf[NUM_LCD_COLS + 1];
		sprintf_P(buf, PSTR("Enable %s%n"),
				getSchedule(scheduleChoice)->getName());
		displayLineLeft(0, buf);
		displayLineLeft(1, choice ? F("->Enable"):F("->Disable"));
		break;
	}
		//  ERROR CASE
	case 3: {
		prevMillis = millis();
		state++;
		// No break, fall through and display
	}
		/* no break */
	case 4: {
		if (millis() - prevMillis >= MESSAGE_DISPLAY_TIME) {
			state = 0;
			buttonOff();
			encoderOff();
			return true;
		} else {
			displayLineLeft(0, F("Not Calibrated"));
			displayLineLeft(1, F("Can't Enable"));
		}
		break;
	}

}			//end switch (state)
	return false;
}

boolean clearSavedScheduleMenuItem() {
	for (int pump_count = 0; pump_count < NUMBER_OF_PUMPS; pump_count++) {
		getSchedule(pump_count)->saveSchedule(0);
	}
	return true;
}

boolean resetContainerMenuItem() {

	static int scheduleChoice = 0;

	static int state = 0;

	switch (state) {

	case 0: {
		encoderOn();
		buttonOn();
		scheduleChoice = 0;
		state++;
		break;
	}
	case 1: {
		if (checkButton()) {

			state++;
			break;
		}
		useRotaryEncoder(scheduleChoice, 0, NUMBER_OF_PUMPS - 1);
		displayLineLeft(0, getSchedule(scheduleChoice)->getName());
		char buf[NUM_LCD_COLS + 1];
		sprintf_P(buf, PSTR("%04d of %04d mL"),
				getSchedule(scheduleChoice)->getContainer()->getCurrentVolume(),
				getSchedule(scheduleChoice)->getContainer()->getSize());
		displayLineLeft(1, buf);
		break;
	}

	case 2: {
		getSchedule(scheduleChoice)->getContainer()->fill();
		getSchedule(scheduleChoice)->saveState();
		state = 0;
		encoderOff();
		buttonOff();
		return true;
	}

	} // end switch (state)

	if (reblMenu.cancelFlag) {
		state = 0;
		encoderOff();
		buttonOff();
		return true;
	}

	return false;
}

boolean addToContainerMenuItem() {

	static int scheduleChoice = 0;
	static int volumeChoice = 0;

	static int state = 0;

	switch (state) {

	case 0: {
		encoderOn();
		buttonOn();
		scheduleChoice = 0;
		volumeChoice = 0;
		state++;
		break;
	}
	case 1: {
		if (checkButton()) {

			state++;
			break;
		}
		useRotaryEncoder(scheduleChoice, 0, NUMBER_OF_PUMPS - 1);
		displayLineLeft(0, getSchedule(scheduleChoice)->getName());
		char buf[NUM_LCD_COLS + 1];
		sprintf_P(buf, PSTR("%04d of %04d mL"),
				getSchedule(scheduleChoice)->getContainer()->getCurrentVolume(),
				getSchedule(scheduleChoice)->getContainer()->getSize());
		displayLineLeft(1, buf);
		break;
	}

	case 2: {
		if (checkButton()) {

			state++;
			break;
		}
		displayLineLeft(0, F("Vol to Add:"));
		int roomLeft =
				getSchedule(scheduleChoice)->getContainer()->getSize()
						- getSchedule(scheduleChoice)->getContainer()->getCurrentVolume();
		useRotaryEncoder(volumeChoice, 0, roomLeft, CONTAINER_SIZE_STEP);
		char buf[6];
		sprintf_P(buf, PSTR("%04d%n"), volumeChoice);
		displayLineLeft(1, buf);
		break;

	}

	case 3: {
		getSchedule(scheduleChoice)->getContainer()->add(volumeChoice);
		getSchedule(scheduleChoice)->saveState();
		state = 0;
		encoderOff();
		buttonOff();
		return true;
	}

	} // end switch (state)

	if (reblMenu.cancelFlag) {
		state = 0;
		encoderOff();
		buttonOff();
		return true;
	}

	return false;
}

boolean setContainerVolumeMenuItem() {

	static int scheduleChoice = 0;
	static int volumeChoice = 0;

	static int state = 0;

	switch (state) {

	case 0: {
		encoderOn();
		buttonOn();
		scheduleChoice = 0;
		volumeChoice = 0;
		state++;
		break;
	}
	case 1: {
		if (checkButton()) {

			state++;
			break;
		}
		useRotaryEncoder(scheduleChoice, 0, NUMBER_OF_PUMPS - 1);
		displayLineLeft(0, getSchedule(scheduleChoice)->getName());
		char buf[NUM_LCD_COLS + 1];
		sprintf_P(buf, PSTR("%04d of %04d mL"),
				getSchedule(scheduleChoice)->getContainer()->getCurrentVolume(),
				getSchedule(scheduleChoice)->getContainer()->getSize());
		displayLineLeft(1, buf);
		break;
	}

	case 2: {
		if (checkButton()) {

			state++;
			break;
		}
		displayLineLeft(0, F("Vol in Cont:"));
		useRotaryEncoder(volumeChoice, 0,
				getSchedule(scheduleChoice)->getContainer()->getSize(),
				CONTAINER_SIZE_STEP);
		char buf[6];
		sprintf_P(buf, PSTR("%04d%n"), volumeChoice);
		displayLineLeft(1, buf);
		break;

	}

	case 3: {
		getSchedule(scheduleChoice)->getContainer()->setCurrentVolume(
				volumeChoice);
		getSchedule(scheduleChoice)->saveState();
		state = 0;
		encoderOff();
		buttonOff();
		return true;
	}

	} // end switch (state)

	if (reblMenu.cancelFlag) {
		state = 0;
		encoderOff();
		buttonOff();
		return true;
	}

	return false;
}

boolean setContainerSizeMenuItem() {

	static int scheduleChoice = 0;
	static int volumeChoice = 0;

	static int state = 0;

	switch (state) {

	case 0: {
		encoderOn();
		buttonOn();
		scheduleChoice = 0;
		volumeChoice = 0;
		state++;
		break;
	}
	case 1: {
		if (checkButton()) {

			state++;
			break;
		}
		useRotaryEncoder(scheduleChoice, 0, NUMBER_OF_PUMPS - 1);
		displayLineLeft(0, getSchedule(scheduleChoice)->getName());
		char buf[NUM_LCD_COLS + 1];
		sprintf_P(buf, PSTR("%04d of %04d mL"),
				getSchedule(scheduleChoice)->getContainer()->getCurrentVolume(),
				getSchedule(scheduleChoice)->getContainer()->getSize());
		displayLineLeft(1, buf);
		break;
	}

	case 2: {
		if (checkButton()) {

			state++;
			break;
		}
		displayLineLeft(0, F("Container Size:"));
		useRotaryEncoder(volumeChoice, 0, MAXIMUM_CONTAINER_SIZE,
				CONTAINER_SIZE_STEP);
		char buf[6];
		sprintf_P(buf, PSTR("%04d%n"), volumeChoice);
		displayLineLeft(1, buf);
		break;

	}

	case 3: {
		getSchedule(scheduleChoice)->getContainer()->setSize(volumeChoice);
		getSchedule(scheduleChoice)->saveSchedule();
		state = 0;
		encoderOff();
		buttonOff();
		return true;
	}

	} // end switch (state)

	if (reblMenu.cancelFlag) {
		state = 0;
		encoderOff();
		buttonOff();
		return true;
	}
	return false;
}

boolean primePumpMenuItem() {

	static int scheduleChoice = 0;
	static boolean quit = false;

	static int state = 0;

	switch (state) {

	case 0: {
		encoderOn();
		buttonOn();
		scheduleChoice = 0;
		state++;
		break;
	}
	case 1:   //  Choose a pump
	{
		if (checkButton()) {

			state++;
			break;
		}
		useRotaryEncoder(scheduleChoice, 0, NUMBER_OF_PUMPS - 1);
		displayLineLeft(0, F("Choose Pump:"));
		displayLineLeft(1, getSchedule(scheduleChoice)->getName());
		break;
	}

	case 2: {
		if (checkButton()) {

			getSchedule(scheduleChoice)->setPriming(true);
			getSchedule(scheduleChoice)->turnPumpOn();
			state++;
			break;
		}
		displayLineLeft(0, F("Turn on:"));
		displayLineLeft(1, getSchedule(scheduleChoice)->getName());
		break;
	}
	case 3: {
		if (checkButton()) {

			getSchedule(scheduleChoice)->turnPumpOff();
			state = 0;
			encoderOff();
			buttonOff();
			return true;
		}
		displayLineLeft(0, F("Turn off:"));
		displayLineLeft(1, getSchedule(scheduleChoice)->getName());
		break;
	}

	}  // end switch (state)

	return false;
}

boolean calibratePumpMenuItem() {

	static int scheduleChoice = 0;

	static int state = 0;

	static unsigned long startedTime = 0;

	static int outputVolume = 0;

	switch (state) {

	case 0: {
		buttonOn();
		encoderOn();
		scheduleChoice = 0;
		startedTime = 0;
		outputVolume = 0;
		state++;
		break;
	}
	case 1:   //  Choose a pump
	{
		if (checkButton()) {

			state++;
			break;
		}
		useRotaryEncoder(scheduleChoice, 0, NUMBER_OF_PUMPS - 1);
		displayLineLeft(0, F("Choose Pump:"));
		displayLineLeft(1, getSchedule(scheduleChoice)->getName());
		break;
	}

	case 2: {
		if (checkButton()) {

			startedTime = millis();
			getSchedule(scheduleChoice)->setPriming(true);
			getSchedule(scheduleChoice)->turnPumpOn();
			state++;
			break;
		}
		displayLineLeft(0, F("Press to start:"));
		displayLineLeft(1, getSchedule(scheduleChoice)->getName());
		break;
	}
	case 3: {
		if (millis() - startedTime >= 30000ul) {

			getSchedule(scheduleChoice)->turnPumpOff();

			encoderOn();
			state++;
			break;
		}
		displayLineLeft(0, F("Running:"));
		displayLineLeft(1, getSchedule(scheduleChoice)->getName());
		break;
	}

	case 4: {
		if (checkButton()) {
			getSchedule(scheduleChoice)->setCalibration(outputVolume * 2);
			encoderOff();
			buttonOff();
			state = 0;
			return true;
		}
		useRotaryEncoder(outputVolume, 0, MAXIMUM_VOLUME);
		displayLineLeft(0, F("Enter Volume:"));
		char buf[NUM_LCD_COLS + 1];
		sprintf_P(buf, PSTR("%03d%n"), outputVolume);
		displayLineLeft(1, buf);
		break;
	}

	}  // end switch (state)

	return false;
}
/*
 boolean pwmSelectMenuItem() {

 static int state = 0;
 static unsigned long startTime = 0;

 switch (state) {

 case 0: {
 startTime = millis();
 state++;
 // no break, go ahead and fall through to state 1.
 }
 case 1: {
 if (millis() - startTime >= 2000) {
 state = 0;
 return true;
 }
 displayLineLeft(0, F("PWM not"));
 displayLineLeft(1, F("Implemented"));
 break;
 }

 }  // end switch (state)
 return false;
 }

 */

boolean calibratePwmMenuItem() {

	static int scheduleChoice = 0;
	static int state = 0;

	static uint8_t pwmSetPoint = 0;
	static uint8_t oldSetPoint = 0;

	switch (state) {

	case 0: {
		buttonOn();
		encoderOn();
		scheduleChoice = 0;
		state++;
		break;
	}
	case 1: {  //  Choose a pump

		if (checkButton()) {
			state++;
			pwmSetPoint = getSchedule(scheduleChoice)->getPump()->getPwmRate();
			oldSetPoint = pwmSetPoint;
			break;
		}
		useRotaryEncoder(scheduleChoice, 0, NUMBER_OF_PUMPS - 1);
		displayLineLeft(0, F("Choose Pump:"));
		displayLineLeft(1, getSchedule(scheduleChoice)->getName());
		break;
	}
	case 2: {
		if (checkButton()) {
			getSchedule(scheduleChoice)->setPriming(true);
			getSchedule(scheduleChoice)->turnPumpOn();
			state++;
			break;
		}
		displayLineLeft(0, F("Press to start:"));
		displayLineLeft(1, getSchedule(scheduleChoice)->getName());
		break;
	}
	case 3: {
		if (checkButton()) {
			getSchedule(scheduleChoice)->turnPumpOff();
			if (pwmSetPoint != oldSetPoint) {
				getSchedule(scheduleChoice)->saveCal(1);  // save the new value
				getSchedule(scheduleChoice)->saveCal(0); // clear the calibration flag
			}
			encoderOff();
			buttonOff();
			scheduleChoice = 0;
			state = 0;
			return true;
		}
		useRotaryEncoder(pwmSetPoint, (byte) 0, (byte) 255);
		displayLineLeft(0, F("Press to Set:"));
		char buf[NUM_LCD_COLS + 1];
		sprintf_P(buf, PSTR("PWM = %03d%n"), pwmSetPoint);
		displayLineLeft(1, buf);
		getSchedule(scheduleChoice)->getPump()->setPwmRate(pwmSetPoint);
		getSchedule(scheduleChoice)->turnPumpOn();
		break;
	}
	}  // end switch (state)
	return false;
}
// What to do about the pump?   IS it on or off?  Have we even chosen a schedule yet?
//  No cancel can be allowed without a flag of some sort.
//	if (reblMenu.cancelFlag) {
//			state = 0;
//			encoderOff();
//			buttonOff();
//			return true;
//		}

boolean showCalibrationMenuItem() {

	static int scheduleChoice = 0;

	static int state = 0;

	switch (state) {

	case 0: {
		encoderOn();
		buttonOn();
		scheduleChoice = 0;
		state++;
		break;
	}
	case 1: {
		if (checkButton()) {

			state++;
			break;
		}
		displayLineLeft(0, F("Choose Sched:"));
		useRotaryEncoder(scheduleChoice, 0, NUMBER_OF_PUMPS - 1);
		displayLineLeft(1, getSchedule(scheduleChoice)->getName());
		break;
	}
	case 2: {
		if (checkButton()) {
			buttonOff();
			encoderOff();
			state = 0;
			return true;
		}

		char bufs[2][NUM_LCD_COLS + 1];
		sprintf_P(bufs[0], PSTR("Rate= %3d%n"),
				getSchedule(scheduleChoice)->getPump()->getFlowRate());
		sprintf_P(bufs[1], PSTR("PWM @ %03d %03d%%%n"),
				getSchedule(scheduleChoice)->getPump()->getPwmRate(),
				(getSchedule(scheduleChoice)->getPump()->getPwmRate() * 100
						/ 255));
		displayLineLeft(0, bufs[0]);
		displayLineLeft(1, bufs[1]);
		break;

	}
	} // end switch (state)

	return false;

}

