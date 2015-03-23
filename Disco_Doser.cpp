// Do not remove the include below
#include "Disco_Doser.h"
#include "Time.h"


uint8_t heartbeatPin = A0;
uint8_t heartState = 0;


void setup() {

	pinMode(7, OUTPUT); // Need a 5V line for my button and encoder right now. Easier to reach pin 7 than +5V right now.
	digitalWrite(7, HIGH); //These first two lines will come out in the final product
	pinMode(heartbeatPin, OUTPUT);
	initLCD();
	initButton();
	initEncoder();
	RTC.begin(9);
	DoseAlert::setAlertHandler (alertHandler);
	getSchedule(0)->startupCode(5, EA_ALK_SCHEDULE, "Alk");
	getSchedule(1)->startupCode(6, EA_CA_SCHEDULE, "Cal");
//setTime(1388534400);
	setSyncInterval(60ul * 60ul); // sync once an hour
	setSyncProvider(RTC.get);
// WDT will reset the board if dosers aren't checked at least once every 2 seconds.
	wdt_enable (WDTO_2S);

}

void loop() {

	runDosers();
	wdt_reset(); //pat the dog
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
