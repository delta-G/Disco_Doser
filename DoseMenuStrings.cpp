
#include "DoseMenu.h"
#include <avr/pgmspace.h>

#define arrSize(x) (sizeof(x) / sizeof(x[0]))


const char M1_S1[] PROGMEM = "Set Time";
const char M1_S2[] PROGMEM = "Schedule";
const char M1_S3[] PROGMEM = "Single Dose";
const char M1_S4[] PROGMEM = "Temp Adjustment";
const char M1_S5[] PROGMEM = "Pump";
const char M1_S6[] PROGMEM = "Container";
const char M1_S7[] PROGMEM = "Set Lockout";
const char M1_S8[] PROGMEM = "Run";



const char M2_S1[] PROGMEM = "Set Schedule";
const char M2_S11[] PROGMEM = "Set Name";
const char M2_S2[] PROGMEM = "Adjust Volume";
const char M2_S3[] PROGMEM = "Set Max Volume";
const char M2_S4[] PROGMEM = "Show Schedule";
const char M2_S5[] PROGMEM = "Match Schedule";
const char M2_S6[] PROGMEM = "Enable Schedule";
const char M2_S7[] PROGMEM = "Save Schedule";
const char M2_S8[] PROGMEM = "Get Saved";
const char M2_S9[] PROGMEM = "Clear Saved";
const char M2_S10[] PROGMEM = "Exit";



const char M3_S1[] PROGMEM = "Prime Pump";
const char M3_S2[] PROGMEM = "Cal Pump";
const char M3_S3[] PROGMEM = "Exit";



const char M4_S1[] PROGMEM = "Reset Cont";
const char M4_S2[] PROGMEM = "Add to Cont";
const char M4_S3[] PROGMEM = "Set Cont Vol";
const char M4_S4[] PROGMEM = "Set Cont Size";
const char M4_S5[] PROGMEM = "Exit";




const char * const menu_1[] PROGMEM =
{
  M1_S1,
  M1_S2,
  M1_S3,
  M1_S4,
  M1_S5,
  M1_S6,
  M1_S7,
  M1_S8
};

const char * const menu_2[] PROGMEM =
{
  M2_S1,
  M2_S11,
  M2_S2,
  M2_S3,
  M2_S4,
  M2_S5,
  M2_S6,
  M2_S7,
  M2_S8,
  M2_S9,
  M2_S10
};

const char * const menu_3[] PROGMEM =
{
  M3_S1,
  M3_S2,
  M3_S3
};

const char * const menu_4[] PROGMEM =
{
  M4_S1,
  M4_S2,
  M4_S3,
  M4_S4,
  M4_S5,
};

const char * const *menus[] =
{
  menu_1,
  menu_2,
  menu_3,
  menu_4
};

int menuSizes[] = 
{
  arrSize(menu_1),
  arrSize(menu_2),
  arrSize(menu_3),
  arrSize(menu_4)
};

int getMenuSize(int amenu){
  return menuSizes[amenu];
}

const char * const* getMenuText(int amenu, int aitem) {
  return &(menus[amenu][aitem]);  
}
