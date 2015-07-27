#include "DoseHead.h"

DoseSchedule Alk_Schedule;

DoseSchedule Ca_Schedule;

//  Schedule names and pointers to them.

DoseSchedule* schedules[] = { &Alk_Schedule, &Ca_Schedule };

DoseSchedule* getSchedule(int aindex) {
	return schedules[aindex];
}

//  Exceptions
DoseAlert* alertList[MAX_ALERTS];
int numberOfAlerts = 0;

void alertHandler(DoseAlert* daptr) {
	if (numberOfAlerts < MAX_ALERTS) {
		alertList[numberOfAlerts] = daptr;
		numberOfAlerts++;
	}
}

void removeAlert(int aindex) {
	if (aindex < numberOfAlerts - 1) {
		for (int i = aindex; i < numberOfAlerts - 1; i++) {
			alertList[i] = alertList[i + 1];
		}
	}
	alertList[numberOfAlerts - 1] = 0;
	numberOfAlerts--;
}

int freeRam() {
	extern int __heap_start, *__brkval;
	int v;
	return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

StateVar currentState = RUN_STATE;

int setState(StateVar astate) {
	int retval = currentState;
	currentState = astate;
	return retval;
}

void doUI() {

	static unsigned long previousMillis = 0;
	unsigned long currentMillis = millis();
	if (currentMillis - previousMillis >= DISPLAY_DELAY) {

		previousMillis = currentMillis;

		switch (currentState) {

		case RUN_STATE: {
			doRunStateUI();
			break;
		}

		case MENU_STATE: {
			doMenuStateUI();
			break;
		}

		case DOSE_STATE:   // Should set up to use the button to interrupt
		{
			doDoseStateUI();
			boolean exitMe = true;
			for (int i = 0; i < NUMBER_OF_PUMPS; i++) {
				if (getSchedule(i)->isPumpRunning()) {
					exitMe = false;
					break;  // only need to find one running
				}
			}
			if (exitMe) {
				currentState = RUN_STATE;
			}
			break;
		}

		case ALERT_STATE: {

			doAlertStateUI();
			break;

		}
		}  // end switch (currentState)
		figureTheColor();
		doDisplay();
		pollButton();
	}
}

void figureTheColor() {
	color_vars color = BLUE;
	if (currentState == DOSE_STATE) {
		color = GREEN;
	}
	if (numberOfAlerts > 0) {
		color = RED;
	}
	if (currentState == MENU_STATE) {
		color = BLUE;
	}
	setColor(color);
}

void doRunStateUI() {
	static unsigned long runStateStartTime;
	static int runStateState;

	switch (runStateState) {
	case 0: {
		runStateStartTime = millis();
		runStateState = 1;
		// no break fall through to next state
	}
		/* no break */
	case 1: {
		if (numberOfAlerts > 0) {
			if ((millis() - runStateStartTime > ALERT_DELAY)
					|| checkRotaryEncoder()) {
				buttonOff();
				encoderOff();
				currentState = ALERT_STATE;
				runStateState = 0;
				break;
			}
		}
		if (!isButtonOn()) {
			buttonOn();
		}
		if (!isEncoderOn()) {
			encoderOn();
		}
		for (int i = 0; i < NUMBER_OF_PUMPS; i++) {
			if (getSchedule(i)->isPumpRunning()) {
				buttonOff();
				encoderOff();
				currentState = DOSE_STATE;
				runStateState = 0;
				break;  // break for loop
			}
		}
		if (checkButton()) {  // can go to menu state even while dosing
			initMenu(); // sets up entry point, current menu and current item both to 0
			buttonOff();
			encoderOff(); // just in case it was left on.  I want it to reset when menu gets called.
			currentState = MENU_STATE;
			runStateState = 0;
			break;
		}
		if (currentState == RUN_STATE) {  // if it hasn't changed states
			char buf[2][NUM_LCD_COLS];
			//timePrint(now(), buf[0], buf[1]);
			buildRunStateDisplay(buf[0], buf[1]);

			displayLineLeft(0, buf[0]);
			displayLineLeft(1, buf[1]);
		}
		break;
	}
	}  // end switch (runStateState)
}

void buildRunStateDisplay(char* buf1, char* buf2) {
	time_t cur = now();
	sprintf_P(buf1, PSTR("%02d:%02d%c%3s%3d/%3d"), hour(cur), minute(cur),
			(getSchedule(0)->isEnabled() ? '*' : '-'),
			getSchedule(0)->getName(), getSchedule(0)->getVolumeDosed(),
			getSchedule(0)->getTargetVolume());
	sprintf_P(buf2, PSTR("%5s%c%3s%3d/%3d"),
			(getSchedule(0)->isEnabled() ? '*' : '-'),
			getSchedule(0)->getName(), getSchedule(0)->getVolumeDosed(),
			getSchedule(0)->getTargetVolume());
}

void doDoseStateUI() {
	for (int i = 0; i < NUMBER_OF_PUMPS; i++) {
		if (getSchedule(i)->isPumpRunning()) {

			unsigned long timeRemaining = (getSchedule(i)->getPumpRunTime()
					- (millis() - getSchedule(i)->getPumpStartTime())) / 1000;

			char buf[2][NUM_LCD_COLS];
			sprintf_P(buf[0], PSTR("Running: %s%n"), getSchedule(i)->getName());
			sprintf_P(buf[1], PSTR("%d%n"), timeRemaining);
			displayLineLeft(0, buf[0]);
			displayLineLeft(1, buf[1]);
			break;   // We can only show one pump
		}
	}
}

void doMenuStateUI() {
	callMenu();
}

void doAlertStateUI() {
	static unsigned long alertStartTime;
	static int alertState = 0;
	static int currentAlertIndex = 0;

	switch (alertState) {
	case 0: {
		alertStartTime = millis();
		alertState = 1;

		int maxLevel = 0;
		for (int i = 0; i < numberOfAlerts; i++) {

			if (alertList[i]->isActive()) {
				if (alertList[i]->getLevel() >= maxLevel) { // use >= because we want the latest highest level.  SO replace if we find the same level.
					maxLevel = alertList[i]->getLevel();
					currentAlertIndex = i;
				}
			} else {
				//removeAlert(i);  // remove any inactive alerts from the list  // may be dangerous in the middle of this loop
			}

		}

		// No break, fall through to state 1 on purpose
	}
	case 1: {
		if (millis() - alertStartTime > ALERT_DELAY) {
			if (currentAlertIndex < numberOfAlerts - 1) {
				currentAlertIndex += 1;
				alertStartTime = millis();
			} else {
				buttonOff();
				encoderOff();
				currentState = RUN_STATE;
				alertState = 0;
				break;
			}

		}
		if (!isButtonOn()) {
			buttonOn();
		}
		if (!isEncoderOn()) {
			encoderOn();
		}
		for (int i = 0; i < NUMBER_OF_PUMPS; i++) {
			if (getSchedule(i)->isPumpRunning()) {
				buttonOff();
				encoderOff();
				currentState = DOSE_STATE;
				alertState = 0;
				break;  // break for loop
			}
		}
		if (checkButton()) {  // can go to menu state even while dosing
			initMenu(); // sets up entry point, current menu and current item both to 0
			buttonOff();
			encoderOff();
			currentState = MENU_STATE;
			alertState = 0;
			break;
		}
		for (int i = numberOfAlerts - 1; i >= 0; i--) {
			if (!(alertList[i]->isActive())) {
				removeAlert(i);
			}
		}
		if (currentState == ALERT_STATE) {  // if the state hasn't changed.
			if (numberOfAlerts > 0) { // if there are any exceptions at all left.
				if (peekRotaryEncoder()) {
					useRotaryEncoder(currentAlertIndex, 0, numberOfAlerts - 1,
							1);
					alertStartTime = millis(); // reset if we're changing alerts.
				}
				displayLineLeft(0, alertList[currentAlertIndex]->getText(0));
				displayLineLeft(1, alertList[currentAlertIndex]->getText(1));
			} else {
				currentState = RUN_STATE;
				buttonOff();
				encoderOff();
				alertState = 0;
			}
		}
		break;
	}
	}  //end switch (alertState)
}

void runDosers() {
	for (int i = 0; i < NUMBER_OF_PUMPS; i++) {
		getSchedule(i)->runSchedule();
	}
}

