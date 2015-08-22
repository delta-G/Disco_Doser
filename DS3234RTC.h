/*
 * DS3234RTC.h - library for DS3234 RTC
 * This library is intended to be used with Arduino Time.h library functions
 */
#ifndef DS3234RTC_h
#define DS3234RTC_h

#include "Time.h"

// RTC based on the DS3234 chip connected via SPI and the SPI library
class DS3234RTC {
public:
	DS3234RTC();
	static void begin(int _cs_pin);
	bool isRunning(void);
	static time_t get();
	static void set(time_t t);
	static void read(tmElements_t &tm);

	static int writeToSRAM(uint8_t, uint8_t*, int);
	static int readFromSRAM(uint8_t, uint8_t*, int);

protected:
	void static cs(int _value = 8);

private:
	static uint8_t dec2bcd(uint8_t num);
	static uint8_t bcd2dec(uint8_t num);
};

extern DS3234RTC RTC;

template<class T>
int writeRTC_SRAM(uint8_t address, T& value) {
	union {
		T type;
		uint8_t b[sizeof(T)];
	} temp;

	temp.type = value;

	return RTC.writeToSRAM(address, temp.b, sizeof(T));

}

template<class T>
int readRTC_SRAM(uint8_t address, T& value) {
	union {
			T type;
			uint8_t b[sizeof(T)];
		} temp;

		int retval = RTC.readFromSRAM(address, temp.b, sizeof(T));

		value = temp.type;
		return retval;
}

#endif
