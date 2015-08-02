/*
 * ProgressBar.cpp
 *
 *  Created on: Jul 28, 2015
 *      Author: David
 */

#include "ProgressBar.h"

uint8_t barChars[8][8] =
		{ { 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000,
				0b10000 }, { 0b11000, 0b11000, 0b11000, 0b11000, 0b11000,
				0b11000, 0b11000, 0b11000 }, { 0b11100, 0b11100, 0b11100,
				0b11100, 0b11100, 0b11100, 0b11100, 0b11100 }, { 0b11110,
				0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110 },
				{ 0b00001, 0b00001, 0b00001, 0b00001, 0b00001, 0b00001, 0b00001,
						0b00001 }, { 0b00011, 0b00011, 0b00011, 0b00011,
						0b00011, 0b00011, 0b00011, 0b00011 }, { 0b00111,
						0b00111, 0b00111, 0b00111, 0b00111, 0b00111, 0b00111,
						0b00111, }, { 0b01111, 0b01111, 0b01111, 0b01111,
						0b01111, 0b01111, 0b01111, 0b01111, }, };

void ProgressBar::initProgBar() {
	for (int i = 0; i < 8; i++) {
		LCD.createChar(i, barChars[i]);
	}
}
void ProgressBar::showProgBar(uint8_t aPercent) {

	if (aPercent < 0) {
		aPercent = 0;
	} else if (aPercent > 100) {
		aPercent = 100;
	}

	int numBars = length * 5 * aPercent / 100; // shouldn't roll an int unless length is >32.

	uint8_t numPrinted = 0;
	LCD.setCursor(start_col, row);

	while (numPrinted < numBars) {
		uint8_t numLeft = numBars - numPrinted;
		if (numLeft > 5) {
			numLeft = 5;
		}
		printBar(numLeft);
		numPrinted += numLeft;
	}
}
void ProgressBar::showProgBar(int aVal, int aMax) {
	showProgBar((uint8_t) (aVal * 100 / aMax));
}
void ProgressBar::printBar(uint8_t aNum) {
	if (aNum == 0) {
		LCD.write(' ');
	} else if (aNum < 5) {
		LCD.write((byte) aNum - 1);
	} else {
		LCD.write((byte) 0xFF);
	}
}
