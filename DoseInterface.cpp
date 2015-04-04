#include "DoseInterface.h"

const byte intPin = 2;
const byte bPin = 4;

byte intMask;
byte bMask;

volatile byte *intReg;
volatile byte *bReg;

volatile char encoderCounter;
boolean encoderIsOn = false;

const byte buttonPin = 7;
boolean buttonPressed = false;
boolean buttonLongPressed = false;
boolean killPress = false;
boolean buttonIsOn = false;

boolean buttonState = HIGH;

unsigned int debounceDelay = 50;   // 50ms debounce

void initEncoder() {
	pinMode(intPin, INPUT_PULLUP);
	pinMode(bPin, INPUT_PULLUP);
	bMask = digitalPinToBitMask(bPin);
	bReg = portInputRegister(digitalPinToPort(bPin));
}

void ISR_encoder_handler() {
	//  My encoder has a full cycle of 4 transitions per click.
	//  I only want one increment or decrement per click
	//  So we are only looking at one transition
	//  We're only using the falling edge on the interrupt pin.
	//  So we know intPin is LOW.
	//  If bPin is HIGH then they're different so decrement
	if (*bReg & bMask) {
		encoderCounter--;
	} else {
		encoderCounter++;
	}
}

void encoderOn() {
	attachInterrupt(0, ISR_encoder_handler, FALLING);
	encoderCounter = 0;
	encoderIsOn = true;
}

void encoderOff() {
	detachInterrupt(0);
	encoderCounter = 0;
	encoderIsOn = false;
}

char checkRotaryEncoder() {
	if (encoderIsOn) {
		char retval = encoderCounter;
		encoderCounter = 0;
		return retval;
	} else {
		return 0;
	}
}

void useRotaryEncoderASCII(char& var) {

	int cre = checkRotaryEncoder();

	var += cre;

	if (cre >= 0) {
		if (var < ' ') {
			var = ' ';
		} else if (var < '0' && var > ' ') {
			var = '0';
		} else if (var < 'A' && var > '9') {
			var = 'A';
		} else if (var < 'a' && var > 'Z') {
			var = 'a';
		} else if (var > 'z') {
			var = ' ';
		}
	} else {
		if (var < ' ') {
			var = 'z';
		} else if (var < '0' && var > ' ') {
			var = ' ';
		} else if (var < 'A' && var > '9') {
			var = '9';
		} else if (var < 'a' && var > 'Z') {
			var = 'Z';
		} else if (var > 'z') {
			var = 'z';
		}
	}
}

boolean peekRotaryEncoder() {
	if (encoderCounter) {
		return true;
	}
	return false;
}

boolean isEncoderOn() {
	return encoderIsOn;
}

void initButton() {
	pinMode(buttonPin, INPUT_PULLUP);
}

void pollButton() {
	static boolean lastState = HIGH;
	static unsigned long pressTime = 0;
	if (buttonIsOn) {
		boolean curState = digitalRead(buttonPin);
		if (curState != lastState) {
			pressTime = millis();
		}
		if ((millis() - pressTime) > debounceDelay) {
			if ((buttonState == LOW) && (curState == HIGH)) { // button was pressed and is being released
				if (!killPress) {   //Prevents registering a press on the release from a long press
					buttonPressed = true;    // register a button press
				}
				killPress = false;
			}
			buttonState = curState;
		}

		if ((buttonState == LOW) && (millis() - pressTime > 2000)) {
			buttonLongPressed = true;
			killPress = true;    //  Force to skip next press when button is released after long press
		}

		lastState = curState;
	}
}

void clearButton() {
	buttonPressed = false;
	buttonLongPressed = false;
}

boolean checkButtonLongPress() {
	boolean retval = buttonLongPressed;
	buttonLongPressed = false;
	return retval;
}

boolean checkButton() {
	boolean retval = buttonPressed;
	buttonPressed = false;
	return retval;
}

void buttonOn() {
	buttonIsOn = true;
	clearButton();
}

void buttonOff() {
	buttonIsOn = false;
	clearButton();
}

boolean isButtonOn() {
	return buttonIsOn;
}

