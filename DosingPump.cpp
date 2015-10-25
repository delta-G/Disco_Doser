#include "DosingPump.h"

DosingPump::DosingPump(byte pin_number) {
	flow_rate = 40;
	pwm_rate = 255;
	pump_pin = pin_number;
	pinMode(pump_pin, OUTPUT);
}

DosingPump::DosingPump() {
	flow_rate = 40;
	pwm_rate = 255;
	pump_pin = 255;  // This should never resolve to a pin so digitalWrite should do nothing.
}

void DosingPump::initPump(byte pin_number) {
	pump_pin = pin_number;
	pinMode(pump_pin, OUTPUT);
}

void DosingPump::setPwmRate(uint8_t aRate){
	pwm_rate = aRate;
}

uint8_t DosingPump::getPwmRate(){
	return pwm_rate;
}

int DosingPump::getFlowRate(){
	return flow_rate;
}

//nonblocking function, turns pump on and returns the lenght of time in ms that it should run


unsigned long DosingPump::startDosingPump(unsigned int volume) {

	unsigned long flow_time_ms = (volume * 60ul * 1000ul) / flow_rate;

	pumpOn();
	return (flow_time_ms);
}

void DosingPump::pumpOn() {
	analogWrite(pump_pin, pwm_rate);  // analogWrite defaults to digitalWrite for values 0 and 255
}

void DosingPump::pumpOff() {
	digitalWrite(pump_pin, LOW);
}

