/*
 * MenuClass.h
 *
 *  Created on: Jun 30, 2015
 *      Author: David
 */

#ifndef MENUCLASS_H_
#define MENUCLASS_H_

#include "Arduino.h"
//#include "DoseHead.h"
#include "DoseDisplay.h"
#include "DoseInterface.h"

typedef bool (*Item_Function)();

class MenuItem {

public:

	const char* const text;
	Item_Function function;

	MenuItem(const char* const aText, Item_Function aFunc) :
			text(aText), function(aFunc) {
	}

	boolean runItem() {
		return function();
	}

	void getText(char* aBuf) {
		strcpy_P(aBuf, (char*) pgm_read_word(text));
	}
};

class MenuList {

	MenuItem* menuItems;
	int listSize;

public:

	MenuList();
	MenuList(MenuItem* aMenu, int asize) :
			menuItems(aMenu), listSize(asize) {
	}

	MenuItem* getItem(int);

};



class MenuClass {

	static MenuList *currentMenu;
	int currentItemIndex;
	static boolean cancelFlag;
	boolean runningFunction;

	MenuClass();

	void doMenu();
	void displayMenu();
	static void setCurrentMenu(MenuList*);

};


#endif /* MENUCLASS_H_ */
