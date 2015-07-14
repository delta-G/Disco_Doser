
#ifndef DosingPump_h
#define DosingPump_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif



class DosingPump{
  
        public: 
        unsigned int flow_rate;
        int pwm_rate;
        byte pump_pin;
        
        DosingPump(byte pump_pin);
	    DosingPump();
        void initPump(byte);
        void setPwmRate(uint8_t);
        uint8_t getPwmRate();

        unsigned long startDosingPump(unsigned int volume);
        void pumpOn();
        void pumpOff();
        
        
};

#endif
