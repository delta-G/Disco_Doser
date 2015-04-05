#include "DoseDisplay.h"
#include "DoseHead.h"

extern HardwareSerial Serial;

LiquidCrystal_SPI_8Bit LCD(LCD_ENABLE_PIN, LCD_SS_PIN);

char lineBuffer[NUM_LCD_ROWS][NUM_LCD_COLS + 1]; // Leave an extra space for terminating null

color_vars current_color;

boolean cursorActive = false;
int cursorRow;
int cursorCol;

void initLCD() {
	pinMode(LCD_RED, OUTPUT);
	pinMode(LCD_GREEN, OUTPUT);
	pinMode(LCD_BLUE, OUTPUT);

#ifdef USING_SERIAL
	Serial.begin(19200);
#else
	LCD.begin(NUM_LCD_COLS, NUM_LCD_ROWS);
	LCD.noCursor();
	LCD.clear();
	setColor(BLUE);
#endif
}

void setColor(color_vars color) {
	current_color = color;
}

void doBacklightColor() {
	digitalWrite(LCD_RED, (current_color & 1));
	digitalWrite(LCD_GREEN, (current_color & 2));
	digitalWrite(LCD_BLUE, (current_color & 4));
}

void doDisplay() {
	static unsigned long previousMillis = 0;
	unsigned long currentMillis = millis();
	if (currentMillis - previousMillis >= DISPLAY_DELAY) {
#ifdef USING_SERIAL
		Serial.print(F("0: "));
		Serial.print(lineBuffer[0]);
		Serial.println(F("\\"));
		Serial.print(F("1: "));
		Serial.print(lineBuffer[1]);
		Serial.print(F("\\ RAM = "));
		Serial.println(freeRam());
		Serial.println(F("**"));
		Serial.print(F("Backlight = "));
		Serial.println(current_color);
		Serial.println(F("**"));
		delay(100);
#else
		LCD.clear();
		doBacklightColor();
		for (int i = 0; i < NUM_LCD_ROWS; i++) {
			lineBuffer[i][NUM_LCD_COLS] = 0;   // Make sure the null is there.
			LCD.setCursor(0, i);
			LCD.print(lineBuffer[i]);
		}
#endif
		doCursor();
		previousMillis = currentMillis;
	}
}

void displayTextLeft(int row, int scol, int cols, const char* message) {
	boolean over = false;
	for (int i = scol; i < cols; i++) {
		if (!over) {
			char c = message[i];
			if (c == 0) {
				over = true;
				lineBuffer[row][i] = ' ';
			} else {
				lineBuffer[row][i] = c;
			}
		} else {
			lineBuffer[row][i] = ' '; // fill with spaces to clear anything else on the line.
		}
	}
}

void displayTextLeft(int row, int scol, int cols, const __FlashStringHelper* message) {
	boolean over = false;
	PGM_P p = reinterpret_cast<PGM_P>(message);
	for(int i=scol; i<cols; i++) {
		if (!over) {
			char c = pgm_read_byte(&p[i]);
			if(c == 0) {
				over = true;
				lineBuffer[row][i] = ' ';
			}
			else {
				lineBuffer[row][i] = c;
			}
		}
		else {
			lineBuffer[row][i] = ' '; // fill with spaces to clear anything else on the line.
	}
}
}

void displayTextRight(int row, int scol, int col, const char* message) {
	int numChars = strlen(message);
	int numSpaces = 0;

	if (col > 16 - scol) {
		col = 16 - scol;
	}
	numSpaces = col - numChars;
	if (numSpaces < 0) {
		numSpaces = 0;
	}
	for (int i = scol; i < scol + col; i++) {
		if (i < numSpaces) {
			lineBuffer[row][i] = ' ';
		} else {
			lineBuffer[row][i] = message[i - numSpaces];
		}
	}
}

void displayTextRight(int row, int scol, int col, const __FlashStringHelper* message) {
	int numChars = strlen_P((const char*)message);
	int numSpaces = 0;
	PGM_P p = reinterpret_cast<PGM_P>(message);

	if (col > 16 - scol) {
		col = 16 - scol;
	}
	numSpaces = col - numChars;
	if (numSpaces < 0) {
		numSpaces = 0;
	}
	for(int i=scol; i<col; i++) {
		if(i < numSpaces) {
			lineBuffer[row][i] = ' ';
		}
		else {
			lineBuffer[row][i] = pgm_read_byte(&p[i - numSpaces]);
		}
	}
}

void displayLineLeft(int row, const char* message) {
	displayTextLeft(row, 0, NUM_LCD_COLS, message);
}

void displayLineLeft(int row, const __FlashStringHelper* message) {
	displayTextLeft(row, 0, NUM_LCD_COLS, message);
}

void displayLineRight(int row, const char* message) {
	displayTextRight(row, 0, NUM_LCD_COLS, message);
}

void displayLineRight(int row, const __FlashStringHelper* message) {
	displayTextRight(row, 0, NUM_LCD_COLS, message);
}

void showCursor(boolean aBool){
	cursorActive = aBool;
}

void setCursor(int arow, int acol){
	cursorRow = arow;
	cursorCol = acol;
	cursorActive = true;
}


void doCursor(){
	if(cursorActive){
#ifdef USING_SERIAL
	Serial.print("Cursor at ");
	Serial.print(cursorRow);
	Serial.println(cursorCol);
#else
	LCD.cursor();
	LCD.setCursor(cursorCol, cursorRow);
#endif
	}
	else {
#ifdef USING_SERIAL

#else
		LCD.noCursor();
#endif
	}
}


/*
 void displayLabelAndValue(int row, const char* alabel, int value){
 char label[NUM_LCD_COLS - 6];
 //  if(strlen(label) > NUM_LCD_COLS - 6){
 //    label[NUM_LCD_COLS - 6] = 0;          //  Truncate labels that are too long
 //  }

 strcpy(label, alabel);

 char valString[6];
 sprintf(valString, "%d%n", value);

 displayLineLeft(row, label);
 displayTextRight(row, NUM_LCD_COLS - 6, 6, valString);
 }
 */

