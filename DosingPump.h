
#ifndef DosingPump_h
#define DosingPump_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif



class DosingPump{
  
        public: 
        unsigned int minimum_flow_rate;
        unsigned int maximum_flow_rate;
        int minimum_pwm_rate;
        int maximum_pwm_rate;
        byte pump_pin;
        
        DosingPump(byte pump_pin);
	    DosingPump();
        void initPump(byte);
        void runDosingPump(unsigned int volume, int rate);
        unsigned long startDosingPump(unsigned int volume, int rate);
        unsigned long startDosingPump(unsigned int volume);
        void pumpOn();
        void pumpOff();
        
        private:
        int rateToPWM(int rate);
        
};

#endif
