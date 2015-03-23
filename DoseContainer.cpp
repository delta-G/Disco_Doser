
#include "DoseContainer.h"
#include <avr/pgmspace.h>
#include <Arduino.h>


DoseContainer::DoseContainer(){
  containerSize = 4000;
  currentVolume = 0;
}

DoseContainer::DoseContainer(char* aname, int asize, int avol){
  containerSize = asize;
  currentVolume = avol;
  setName(aname);
}

char* DoseContainer::getName(){
  return name;
}

void DoseContainer::setName(char* aname){
  for (int i=0; i < 5; i++){
    if(aname[i] != 0){
      name[i] = aname[i];
    } else {
      break;
    }
  }
  name[5] = 0;
}


int DoseContainer::getSize(){
  return containerSize;
}

int DoseContainer::getCurrentVolume(){
  return currentVolume;
}

void DoseContainer::setCurrentVolume(int avol){
  currentVolume = avol;
  if (currentVolume > containerSize){
    currentVolume = containerSize;
  }
}

void DoseContainer::setSize(int asize){
  containerSize = asize;
}

void DoseContainer::add(int avol){
  currentVolume += avol;
  if (currentVolume > containerSize){
    currentVolume = containerSize;
  }
  checkVolume();
}

void DoseContainer::fill(){
  currentVolume = containerSize;
  checkVolume();   // To reset any alerts
}

void DoseContainer::take(int avol){
  if(avol > currentVolume){
    currentVolume = 0;
  }
  else {
    currentVolume -= avol;
  }
  checkVolume();
}

void DoseContainer::checkVolume(){
  if(getCurrentVolume() <= 0){
    dalert.setActive(true, 2, name, F("EMPTY"));
  } else if (getCurrentVolume() <= containerSize / 10){
    dalert.setActive(true, 1, name, F("Below 10%"));
  } else {
    dalert.setActive(false);
  }
}  
