
#ifndef DoseDisplay_h
#define DoseDisplay_h

#include "Arduino.h"
#include "LiquidCrystal_SPI_8Bit.h"
#include "DoseHead.h"


#define DISPLAY_DELAY 500

#define NUM_LCD_ROWS 2
#define NUM_LCD_COLS 16


enum color_vars {BLACK, RED , GREEN , YELLOW , BLUE , PURPLE , CYAN , WHITE};



void initLCD();
void doBacklightColor();
void setColor(color_vars);
void doDisplay();
void displayTextLeft(int, int, int, const char*);
void displayTextLeft(int, int, int, const __FlashStringHelper*);
void displayTextRight(int, int, int, const char*);
void displayLineLeft(int ,const char* );
void displayLineLeft(int ,const __FlashStringHelper* );
void displayLineRight(int ,const char* );
void displayLabelAndValue(int ,const char* ,int);
void doCursor();
void setCursor(int, int);
void showCursor(boolean);





#endif

