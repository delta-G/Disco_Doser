

#include "DoseAlert.h"

void defaultDoseAlertHandler(DoseAlert* de){

  // do nothing;
  // use this if you just want to print to serial or something simple every time that doesn't need to be set up somehwere else. 

}

DoseAlert::FireFunc DoseAlert::fireAlert = defaultDoseAlertHandler;

void DoseAlert::setAlertHandler(void (*handler)(DoseAlert*)){  
  fireAlert = handler;
}

DoseAlert::DoseAlert(){
  active = false;
  level = 0;
}

DoseAlert::DoseAlert(int alevel, char* atext, char* btext){

  level = alevel;
  setText(0, atext);
  setText(1, btext);

  active = false;
}

DoseAlert::DoseAlert(int alevel, const __FlashStringHelper* atext, const __FlashStringHelper* btext){

  level = alevel;
  setText(0, atext);
  setText(1, btext);

  active = false;
}


char* DoseAlert::getText(int aline){
  return text[aline];
}


void DoseAlert::setText(int aline, char* atext){
  if(aline <= 1){
    strcpy(text[aline], atext);
  }
}

void DoseAlert::setText(int aline, const __FlashStringHelper* atext){
  if(aline <= 1){
    strcpy_P(text[aline], (char*)atext);
  }
}

int DoseAlert::getLevel(){
  return level;
}

void DoseAlert::setLevel(int alev){
  level = alev;
}


boolean DoseAlert::isActive(){
  return active;
}

void DoseAlert::setActive(boolean aboo){
  if(active != aboo){
    if(aboo){
      fireAlert(this);
    }
    active = aboo;
  }
}

void DoseAlert::setActive(boolean aboo, int alev, char* atex0, char* atex1){
  setLevel(alev);
  setText(0, atex0);
  setText(1, atex1);
  setActive(aboo);
}

void DoseAlert::setActive(boolean aboo, int alev, const __FlashStringHelper* atex0, const __FlashStringHelper* atex1){
  setLevel(alev);
  setText(0, atex0);
  setText(1, atex1);
  setActive(aboo);
}

void DoseAlert::setActive(boolean aboo, int alev, char* atex0, const __FlashStringHelper* atex1){
  setLevel(alev);
  setText(0, atex0);
  setText(1, atex1);
  setActive(aboo);
}

void DoseAlert::setActive(boolean aboo, int alev, const __FlashStringHelper* atex0, char* atex1){
  setLevel(alev);
  setText(0, atex0);
  setText(1, atex1);
  setActive(aboo);
}
