/*
 * MenuClass.cpp
 *
 *  Created on: Jun 30, 2015
 *      Author: David
 */

#include "MenuClass.h"

MenuItem* MenuList::getItem(int aIndex) {
	// To make modulo work with negatives the way we want
	while (aIndex < 0) {
		aIndex += listSize;
	}
	return &(menuItems[aIndex % listSize]);
}

//MenuClass::MenuClass(MenuList* aList, int aNum) {
//	menu = aList;
//	numMenus = aNum;
//	currentMenu = menu[0];
//	currentItemIndex = 0;
//	cancelFlag = false;
//	runningFunction = false;
//	//menuFunc = showMenu();
//}

void MenuClass::doMenu() {

	if (runningFunction) {
		runningFunction = !currentMenu->getItem(currentItemIndex)->runItem();
	} else {
		cancelFlag = false; // If we're here it's either been cancelled or doesn't need to be.
		showCursor(false);  // If we're here we don't need it.
		if (isEncoderOn()) {
			if (peekRotaryEncoder()) { //  Don't want to accept a button press if the menu updates on this pass.
				useRotaryEncoder(currentItemIndex, 0);
				buttonOff();  //  Kill the button since the display is updating
			} else {   //Nothing on the encoder, so check the button

				//  if button is on check and resolve it  but don't check button if there's been an encoder change
				if (isButtonOn()) {
					if (checkButton()) {
						buttonOff();
						encoderOff();
						runningFunction = true;
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
}

void MenuClass::displayMenu() {

	int currentLine = 0;
	char outBuf[NUM_LCD_COLS];

	outBuf[0] = '-';
	outBuf[1] = '>';
	currentMenu->getItem(currentItemIndex)->getText(outBuf + 2);
	displayLineLeft(currentLine++, outBuf);
	outBuf[0] = ' ';
	outBuf[1] = ' ';
	currentMenu->getItem(currentItemIndex + 1)->getText(outBuf + 2);  // getItem will handle any rollover
	displayLineLeft(currentLine++, outBuf);
}


void MenuClass::setCurrentMenu(MenuList* aMenu){
	currentMenu = aMenu;
}

