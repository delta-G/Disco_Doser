/*
 * ProgressBar.h
 *
 *  Created on: Jul 27, 2015
 *      Author: David
 */

#ifndef PROGRESSBAR_H_
#define PROGRESSBAR_H_

#include "DoseHead.h"

extern LiquidCrystal_SPI_8Bit LCD;



class ProgressBar {

public:

	uint8_t row;
	uint8_t start_col;
	uint8_t length;



	ProgressBar(uint8_t aRow, uint8_t aStart, uint8_t aLength):row(aRow), start_col(aStart), length(aLength){}

	static void initProgBar();
	void showProgBar(uint8_t);
	void showProgBar(int, int);
	void printBar(uint8_t);



};


#endif /* PROGRESSBAR_H_ */
