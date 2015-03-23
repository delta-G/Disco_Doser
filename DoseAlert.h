#ifndef DoseAlert_h
#define DoseAlert_h

#include "Arduino.h"


/*

 Any class that wants to be able to throw exceptions and have them displayed will need to include at least one object of this class.  
 When you want your exception displayed, set it active.  When you're ready to deactivate it, 
 just set active to false and the main program should remove it from the list.  We will keep an array with 10 slots to hold exceptions.
 If things are full we just won't take any more.  Most code will keep trying to set them.  That's why they need to be in the classes, so 
 the classes can manage their activity.  
 
 
 We will set a static function pointer to the function that needs to be called in the main program when an exception is thrown.  
 It will be the responsibility of the user to set up a function to do something with these exceptions.  And it will be the user's 
 responsibility to call the setalertHandler method and let the class know that it needs to call that function to handle exceptions.
 This only has to be done for one instance of the class and it will apply to all instances in that program.  
 
 */


enum ExceptionTypes {
  UNCLASSIFIED_EXCEPTION , CONTAINER_EXCEPTION };


class DoseAlert {
typedef void (*FireFunc)(DoseAlert*);
private: 

  static FireFunc fireAlert;  

  byte level;
  char text[2][16];  
  boolean active;

public:
  
  DoseAlert();
  DoseAlert(int, char*, char*);
  DoseAlert(int, const __FlashStringHelper*,const __FlashStringHelper*);
  boolean isActive();  
  void setActive(boolean);
  void setActive(boolean, int, char*, char*);
  void setActive(boolean, int,const __FlashStringHelper*,const __FlashStringHelper*);
  void setActive(boolean, int, char*,const __FlashStringHelper*);
  void setActive(boolean, int,const  __FlashStringHelper*, char*);
  int getLevel();
  void setLevel(int);
  char* getText(int);
  void setText(int, char*);
  void setText(int, const __FlashStringHelper*);
  
  static void setAlertHandler(void (*handler)(DoseAlert*));

};






#endif

