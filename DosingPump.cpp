#include "DosingPump.h"

DosingPump::DosingPump(byte pin_number) {
	minimum_flow_rate = 40;
	maximum_flow_rate = 40;
	minimum_pwm_rate = 255;
	maximum_pwm_rate = 255;
	pump_pin = pin_number;
	pinMode(pump_pin, OUTPUT);
}

DosingPump::DosingPump() {
	minimum_flow_rate = 40;
	maximum_flow_rate = 40;
	minimum_pwm_rate = 255;
	maximum_pwm_rate = 255;
	pump_pin = 255;  // This should never resolve to a pin so digitalWrite should do nothing.
}

void DosingPump::initPump(byte pin_number) {
	pump_pin = pin_number;
	pinMode(pump_pin, OUTPUT);
}

int DosingPump::rateToPWM(int rate) {
	int flow_rate = constrain(rate, minimum_flow_rate, maximum_flow_rate);
	int pwm_rate = map(flow_rate, minimum_flow_rate, maximum_flow_rate,
			minimum_pwm_rate, maximum_pwm_rate);
	return pwm_rate;
}

//blocking function to run a pump
void DosingPump::runDosingPump(unsigned int volume, int rate) {

	int flow_rate = constrain (rate, minimum_flow_rate, maximum_flow_rate);
	unsigned long flow_time_ms = (volume / flow_rate) * 60ul * 1000ul;

	unsigned long start_time = millis();
	analogWrite(pump_pin, rateToPWM(flow_rate));
	while (millis() - start_time < flow_time_ms)
		;
	analogWrite(pump_pin, 0);
}

//nonblocking function, turns pump on and returns the lenght of time in ms that it should run
unsigned long DosingPump::startDosingPump(unsigned int volume, int rate) {

	int flow_rate = constrain (rate, minimum_flow_rate, maximum_flow_rate);
	unsigned long flow_time_ms = (volume * 60ul * 1000ul) / flow_rate;

	analogWrite(pump_pin, rateToPWM(flow_rate));
	return (flow_time_ms);
}

unsigned long DosingPump::startDosingPump(unsigned int volume) {
	unsigned long flow_time_ms = (volume * 60ul * 1000ul) / maximum_flow_rate;

	digitalWrite(pump_pin, HIGH);
	return (flow_time_ms);
}

void DosingPump::pumpOn() {
	digitalWrite(pump_pin, HIGH);
}

void DosingPump::pumpOff() {
	digitalWrite(pump_pin, LOW);
}

