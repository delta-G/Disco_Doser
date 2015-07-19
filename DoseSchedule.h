
#ifndef DoseSchedule_h
#define DoseSchedule_h

#include "Arduino.h"
#include "TimeOfDay.h"
#include "DosingPump.h"
#include "EepromFuncs.h"
#include "DoseContainer.h"
#include "DoseAlert.h"
#include "DS3234RTC.h"

#define PWM_ENABLED false

class DoseSchedule {

private:

  char name[4];

  int eeprom_location;
  
  
  boolean enabled;
  boolean priming;

  // user input variables
  TimeOfDay start_time;
  TimeOfDay end_time;
  TimeOfDay interval;
  int set_volume;
  //int rate;
  int maxVolume;

  // tracking variables
  TimeOfDay last_time;
  int volume_dosed;
  int target_volume;
  boolean reset_flag;

  // variables to track the pump
  boolean pump_is_running;

  unsigned long pump_start_time;
  unsigned long pump_run_time;

  //  booster variables
  int booster_volume;
  int booster_days;

  // container variables
  DoseContainer container;
    
  //  The Pump
  
  DosingPump pump;
  
  // Alerts

  DoseAlert volExceedAlert;
  DoseAlert notCalibratedAlert;

  public:

  //  FUNCTION PROTOTYPES

  void startupCode(byte, int, char*);

  DoseSchedule();

  void setSchedule(TimeOfDay, TimeOfDay, TimeOfDay, int);
  char* getName();
  void setDailyVolume(int);
  int getDailyVolume();
  unsigned long getPumpStartTime();
  unsigned long getPumpRunTime();
  TimeOfDay getStartTime();
  TimeOfDay getEndTime();
  TimeOfDay getInterval();
  int getMaxVolume();
  void setMaxVolume(int);
  void setName(char*);
  
  int getVolumeDosed();
  int getTargetVolume();
  int getBoosterVolume();
  int getBoosterDays();
  
  DosingPump* getPump();
  
  void setEnabled(boolean);
  boolean isEnabled();
  void setPriming(boolean);
  
  void setCalibration(int);
  
  DoseContainer* getContainer();
  
  void resetSchedule();
  void runSchedule();
  void initSchedule();
  void listSchedule();
  boolean pumpTimer();
  boolean checkTimer();
  
  void turnPumpOn();
  void turnPumpOff();
  void runPump(int);
  boolean isPumpRunning();
    
  
  void singleDose(int);

  void saveState();
  boolean getState();
  void clearState();

  void saveSchedule(int cl = 1);
  boolean getScheduleFromEEPROM();
  void saveCal(int cl = 1);
  boolean getCal();
  int isCal();

  boolean isInRange(TimeOfDay);
  int calculateVolume(TimeOfDay, int);

  void setBooster();
  void createBooster(int, int);
  void addBooster();

};

#endif

