// Do not remove the include below
#include "Disco_Doser.h"


uint8_t heartbeatPin = HEART_BEAT_PIN; // I know it is redundant, but I want to be able to assign it with the rest of the pins in DoseHead.h
uint8_t heartState = 0;

void setup() {

	//pinMode(3, OUTPUT); // Need a 5V line for my button and encoder right now. Easier to reach pin 3 than +5V right now.
	//digitalWrite(3, HIGH); //These first two lines will come out in the final product
	pinMode(heartbeatPin, OUTPUT);
	initLCD(LCD_RS_PIN, LCD_ENABLE_PIN, LCD_RED, LCD_GREEN, LCD_BLUE);
	initInterface(BUTTON_PIN, ENCODER_INTERRUPT_PIN, ENCODER_B_PIN);
	RTC.begin(RTC_SS_PIN);
	DoseAlert::setAlertHandler(alertHandler);
	getSchedule(0)->startupCode(ALK_PUMP_PIN, EA_ALK_SCHEDULE, "Alk");
	getSchedule(1)->startupCode(CAL_PUMP_PIN, EA_CA_SCHEDULE, "Cal");
//setTime(1388534400);
	setSyncInterval(60ul * 60ul); // sync once an hour
	setSyncProvider(RTC.get);

	splashScreen();

// WDT will reset the board if dosers aren't checked at least once every 2 seconds.
	wdt_enable(WDTO_2S);

}

void loop() {

	runDosers();
	wdt_reset();
	//pat the dog
	doUI();
	heartbeat();

}

void heartbeat() {

	static unsigned long previousBeat = millis();
	unsigned long currentMillis = millis();
	if (currentMillis - previousBeat >= 1000) {
		previousBeat = currentMillis;
		heartState ^= 1;
		digitalWrite(heartbeatPin, heartState);
	}
}

void splashScreen() {
	time_t currentTime = now();
	char bufs[2][NUM_LCD_COLS + 1];
	sprintf_P(bufs[0], PSTR("DISCO DOSER"));
	sprintf_P(bufs[1], PSTR("%02d:%02d %02d/%02d/%02d"), hour(currentTime),
			minute(currentTime), month(currentTime), day(currentTime),
			year(currentTime));
	displayLineLeft(0, bufs[0]);
	displayLineLeft(1, bufs[1]);
	doDisplay();
	delay(2000);
#ifdef GIT_HASH
	//  Enter Info Screen
	if (digitalRead(BUTTON_PIN) == LOW) {
		sprintf_P(bufs[0], PSTR("Commit hash"));
		sprintf_P(bufs[1], PSTR("%.15s"), GIT_HASH);
		displayLineLeft(0, bufs[0]);
		displayLineLeft(1, bufs[1]);
		doDisplay();
		delay(2000);
		while (digitalRead(BUTTON_PIN) == LOW)
			;  // Wait for button release to continue.
	}
#endif
}

