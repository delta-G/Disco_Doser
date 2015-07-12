/*
 * MenuClass.h
 *
 *  Created on: Jun 30, 2015
 *      Author: David
 */



/*  WON'T WORK because you can't use F macro at global scope.  Need to declare the strings separately  */


#ifndef MENUCLASS_H_
#define MENUCLASS_H_

#include "Arduino.h"

typedef bool (*Item_Function)();

class MenuItem{

public:

	const __FlashStringHelper* text;
	Item_Function function;

	MenuItem(const __FlashStringHelper* aText, Item_Function aFunc):
		text(aText), function(aFunc){}

	boolean runItem();

};

class MenuClass{

public:


	MenuClass();
	MenuClass(MenuItem* aMenu, int asize);




};





#endif /* MENUCLASS_H_ */
