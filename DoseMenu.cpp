#include "DoseMenu.h"

void (*menuFunction)();

Branch_Function menu_Branches[] = { &branch_Base, &branch_Schedule,
		&branch_Pump, &branch_Container };

int current_menu;
int current_item;

boolean cancelFlag = false;

// Allows outside code to call menuFunction without exposing it. 
// And now includes the cancel code.
void callMenu() {

	if (checkButtonLongPress()) {
		cancelFlag = true;
	}

	menuFunction();
}

void initMenu() {
	current_menu = 0;
	current_item = 0;
	menuFunction = doMenu;
}

void doMenu() {
	cancelFlag = false; // If we're here it's either been cancelled or doesn't need to be.
	showCursor(false);  // If we're here we don't need it.
	if (isEncoderOn()) {
		if (peekRotaryEncoder()) { //  Don't want to accept a button press if the menu updates on this pass.
			useRotaryEncoder(current_item, 0);
			//handle any rollover of the menu
			if (current_item < 0) {
				current_item = getMenuSize(current_menu) - 1; //subtract 1 cause it's an array index
			} else if (current_item >= getMenuSize(current_menu))
				current_item = 0;

			buttonOff();  //  Kill the button since the display is updating
		} else {   //Nothing on the encoder, so check the button

			//  if button is on check and resolve it  but don't check button if there's been an encoder change
			if (isButtonOn()) {
				if (checkButton()) {
					buttonOff();
					encoderOff();
					//  Set up to do the branch function on the next go
					menuFunction = (*menu_Branches[current_menu]);
					return;
				}
			} else // ELSE turn the button on
			{
				buttonOn();
				encoderOn();
			}
		}
	} else {   //  encoder is off
		encoderOn();
	}

	displayMenu();
}

void displayMenu() {

	int currentLine = 0;
	char outBuf[NUM_LCD_COLS];

	outBuf[0] = '-';
	outBuf[1] = '>';
	strcpy_P(outBuf + 2,
			(char*) pgm_read_word((getMenuText(current_menu, current_item))));
	displayLineLeft(currentLine++, outBuf);
	outBuf[0] = ' ';
	outBuf[1] = ' ';
	strcpy_P(outBuf + 2,
			(char*) pgm_read_word((getMenuText(current_menu, (current_item + 1) % getMenuSize(current_menu)))));
	displayLineLeft(currentLine++, outBuf);
}

// Default action from the branch functions
void notImplementedError() {
	static unsigned long prevMil;
	static int state = 0;
	switch (state) {

	case 0:
		prevMil = millis();
		state = 1;
		//  No break, fallthrough and do the display too

	case 1:
		if (millis() - prevMil > 4000) {
			state = 0;
			//current_menu = (BASE_MENU);
			//current_item = 0;
			menuFunction = doMenu;
		} else {
			displayLineLeft(0, F("Oops Something's"));
			displayLineLeft(1, F("Not Implemented"));
		}
	}
}

//  This gets run if a menu item gets selected. 
void branch_Base() {
	switch (current_item) {

	case SET_TIME: {
		if (setTheTime()) {
			current_menu = (BASE_MENU);
			current_item = 0;
			menuFunction = doMenu;
		}
		break;
	}

	case SCHEDULE: {
		current_menu = (SCHEDULE_MENU);
		current_item = 0;
		menuFunction = doMenu;
		break;
	}

	case SINGLE_DOSE: {
		if (singleDoseMenuItem()) {
			current_menu = (BASE_MENU);
			current_item = 0;
			menuFunction = doMenu;
		}
		break;
	}

	case BOOSTER: {
		if (setBoosterDoseMenuItem()) {
			current_menu = (BASE_MENU);
			current_item = 0;
			menuFunction = doMenu;
		}
		break;
	}

	case PUMP: {
		current_menu = (PUMP_MENU);
		current_item = 0;
		menuFunction = doMenu;
		break;
	}

	case CONTAINER: {
		current_menu = (CONTAINER_MENU);
		current_item = 0;
		menuFunction = doMenu;
		break;
	}

	case RUN:
		setState(RUN_STATE);
		break;

	default:
		menuFunction = notImplementedError;

	}  // end switch (current_item)
}

void branch_Schedule() {
	switch (current_item) {
	case SET_SCHEDULE: {
		if (setScheduleMenuItem()) {
			current_menu = (SCHEDULE_MENU);
			current_item = 0;
			menuFunction = doMenu;
		}
		break;
	}

	case SET_SCHEDULE_NAME: {
		if (setScheduleNameMenuItem()) {
			current_menu = (SCHEDULE_MENU);
			current_item = 0;
			menuFunction = doMenu;
		}
		break;
	}

	case ADJUST_VOLUME: {
		if (adjustVolumeMenuItem()) {
			current_menu = (SCHEDULE_MENU);
			current_item = 0;
			menuFunction = doMenu;
		}
		break;
	}

	case ADJUST_MAX: {
		if (adjustMaxVolumeMenuItem()) {
			current_menu = (SCHEDULE_MENU);
			current_item = 0;
			menuFunction = doMenu;
		}
		break;
	}

	case SHOW_SCHEDULE: {
		if (showScheduleMenuItem()) {
			current_menu = (SCHEDULE_MENU);
			current_item = 0;
			menuFunction = doMenu;
		}
		break;
	}

	case SHOW_STATE: {
		if (showScheduleStateMenuItem()) {
			current_menu = (SCHEDULE_MENU);
			current_item = 0;
			menuFunction = doMenu;
		}

		break;
	}

	case MATCH_SCHEDULE: {
		if (matchScheduleMenuItem()) {
			current_menu = (SCHEDULE_MENU);
			current_item = 0;
			menuFunction = doMenu;
		}
		break;
	}

	case ENABLE_SCHEDULE: {
		if (enableScheduleMenuItem()) {
			current_menu = (SCHEDULE_MENU);
			current_item = 0;
			menuFunction = doMenu;
		}
		break;
	}

//	case SAVE_SCHEDULE: {
//		for (int pump_count = 0; pump_count < NUMBER_OF_PUMPS; pump_count++) {
//			getSchedule(pump_count)->saveSchedule();
//		}
//		current_menu = (SCHEDULE_MENU);
//		current_item = 0;
//		menuFunction = doMenu;
//		break;
//	}
//
//	case GET_SAVED: {
//		for (int pump_count = 0; pump_count < NUMBER_OF_PUMPS; pump_count++) {
//			getSchedule(pump_count)->getSchedule();
//		}
//		current_menu = (SCHEDULE_MENU);
//		current_item = 0;
//		menuFunction = doMenu;
//		break;
//	}

	case CLEAR_SAVED: {
		for (int pump_count = 0; pump_count < NUMBER_OF_PUMPS; pump_count++) {
			getSchedule(pump_count)->saveSchedule(0);
		}
		current_menu = (SCHEDULE_MENU);
		current_item = 0;
		menuFunction = doMenu;
		break;
	}

	case EXIT_SCHED_MENU: {
		current_menu = BASE_MENU;
		current_item = 0;
		menuFunction = doMenu;
		break;
	}
	default:
		menuFunction = notImplementedError;
	}  //  end switch (current_item)

}

void branch_Container() {
	switch (current_item) {
	case RESET_CONTAINER: {
		if (resetContainerMenuItem()) {
			current_menu = (CONTAINER_MENU);
			current_item = 0;
			menuFunction = doMenu;
		}

		break;
	}

	case ADD_TO_CONTAINER: {
		if (addToContainerMenuItem()) {
			current_menu = (CONTAINER_MENU);
			current_item = 0;
			menuFunction = doMenu;
		}

		break;
	}

	case SET_CONTAINER_VOLUME: {
		if (setContainerVolumeMenuItem()) {
			current_menu = (CONTAINER_MENU);
			current_item = 0;
			menuFunction = doMenu;
		}
		break;
	}

	case SET_CONTAINER_SIZE: {
		if (setContainerSizeMenuItem()) {
			current_menu = (CONTAINER_MENU);
			current_item = 0;
			menuFunction = doMenu;
		}
		break;
	}

	case EXIT_CONT_MENU: {
		current_menu = BASE_MENU;
		current_item = 0;
		menuFunction = doMenu;
		break;
	}
	default:
		menuFunction = notImplementedError;
	}  // end switch (current_item)
}

void branch_Pump() {
	switch (current_item) {
	case PRIME_PUMP: {
		if (primePumpMenuItem()) {
			current_menu = (PUMP_MENU);
			current_item = PRIME_PUMP;
			menuFunction = doMenu;
		}
		break;
	}

	case CALIBRATE_PUMP: {
		if (calibratePumpMenuItem()) {
			current_menu = (PUMP_MENU);
			current_item = CALIBRATE_PUMP;
			menuFunction = doMenu;
		}
		break;

	}

	case CALIBRATE_PWM: {
		if (calibratePwmMenuItem()) {
			current_menu = (PUMP_MENU);
			current_item = 0;
			menuFunction = doMenu;
		}
		break;
	}

	case SHOW_CALIBRATION: {
		if (showCalibrationMenuItem()) {
			current_menu = (PUMP_MENU);
			current_item = 0;
			menuFunction = doMenu;
		}
		break;
	}

	case EXIT_PUMP_MENU: {
		current_menu = BASE_MENU;
		current_item = 0;
		menuFunction = doMenu;
		break;
	}
	default:
		menuFunction = notImplementedError;
	}  // end switch (current_item)

}

boolean inputTime(time_t& var) {

	static int state = 0;
	static tmElements_t tmElem;
	breakTime(var, tmElem);

	switch (state) {

	case 0: {      // get set up
		encoderOn();
		buttonOn();
		state++;
		break;
	}
	case 1: {  // input the hours
		if (checkButton()) {
			state++;
			break;
		}
		int setHour = tmElem.Hour;   // Have to do this for wraparound
		useRotaryEncoder(setHour, 0, 23);  // Otherwise this uses unsigned vars.
		tmElem.Hour = setHour;
		setCursor(0, 1);
		break;
	}
	case 2: {  // input the minutes
		if (checkButton()) {
			state++;
			break;
		}
		int setMinute = tmElem.Minute;
		useRotaryEncoder(setMinute, 0, 59);
		tmElem.Minute = setMinute;
		setCursor(0, 4);
		break;
	}
	case 3: {   // input the year
		if (checkButton()) {
			state++;
			break;
		}
		useRotaryEncoder(tmElem.Year, 1);
		setCursor(1, 9);
		break;
	}
	case 4: {  // input the month

		if (checkButton()) {
			state++;
			break;
		}
		useRotaryEncoder(tmElem.Month, (byte) 1, (byte) 12); // OK to use unsigned vars since 0 will wrap around.
		setCursor(1, 1);
		break;
	}
	case 5: {  // input the day

		if (checkButton()) {
			state++;
			break;
		}
		byte monthDays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
		if (LEAP_YEAR(tmElem.Year)) {
			monthDays[1] = 29;
		}
		useRotaryEncoder(tmElem.Day, (byte) 1, monthDays[tmElem.Month - 1]);
		setCursor(1, 4);
		break;
	}
	case 6: { // wrap it up and let the caller know we're done with his variable. var should still be OK from the last time we touched it.
		state = 0;
		encoderOff();
		buttonOff();
		showCursor(false);
		return true;
	}

	}  // end of switch (state)

	if (cancelFlag) {
		state = 0;
		encoderOff();
		buttonOff();
		showCursor(false);
		return true;
	}

	var = makeTime(tmElem);
	return false;
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

	if (cancelFlag) {
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
			return true;
		}
		break;
	}
	}  // end swtich (state)

	tmElements_t tmE;
	breakTime(timeSetTime, tmE);

	char outBuf[2][NUM_LCD_COLS + 1];
	sprintf_P(outBuf[0], PSTR("%02d:%02d:%02d"), tmE.Hour, tmE.Minute,
			tmE.Second);
	sprintf_P(outBuf[1], PSTR("%02d-%02d-%04d"), tmE.Month, tmE.Day,
			tmE.Year + 1970);

	displayLineLeft(0, outBuf[0]);
	displayLineLeft(1, outBuf[1]);

	if (cancelFlag) {
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

	if (cancelFlag) {
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
		//  **TODO
		//  Fix this to also take other boosters into account
		//  Need that boosterActive flag in DoseSchedule done

		//  **TODO
		//  Fix this to allow for negative numbers

		int maxAllow = ((getSchedule(scheduleChoice)->getMaxVolume()
				- getSchedule(scheduleChoice)->getDailyVolume())
				* (MAXIMUM_BOOSTER_DAYS - 1));
		if (maxAllow > MAXIMUM_BOOSTER_DOSE) {
			maxAllow = MAXIMUM_BOOSTER_DOSE;
		}
		useRotaryEncoder(volumeChoice, 0, maxAllow);
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

		int minDays = (volumeChoice
				/ (getSchedule(scheduleChoice)->getMaxVolume()
						- getSchedule(scheduleChoice)->getDailyVolume())) + 1;

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

	if (cancelFlag) {
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

	if (cancelFlag) {
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

	if (cancelFlag) {
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

	if (cancelFlag) {
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
		sprintf_P(bufs[0], PSTR("%03d dosed of %03d%n"),
				getSchedule(scheduleChoice)->getVolumeDosed(),
				getSchedule(scheduleChoice)->getTargetVolume());
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
		state = 0;
		encoderOff();
		buttonOff();
		return true;
	}

	} // end switch (state)

	if (cancelFlag) {
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
		state = 0;
		encoderOff();
		buttonOff();
		return true;
	}

	} // end switch (state)

	if (cancelFlag) {
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
		state = 0;
		encoderOff();
		buttonOff();
		return true;
	}

	} // end switch (state)

	if (cancelFlag) {
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
		state = 0;
		encoderOff();
		buttonOff();
		return true;
	}

	} // end switch (state)

	if (cancelFlag) {
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
				getSchedule(scheduleChoice)->saveCal(0);  // clear the calibration flag
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
//	if (cancelFlag) {
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
