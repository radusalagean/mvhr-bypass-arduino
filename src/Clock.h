#ifndef CLOCK_H
#define CLOCK_H

#include <Arduino.h>
#include <RTClib.h>

class Clock
{
private:
    RTC_DS1307 rtc;

public:
    void init();
    DateTime now();
};

#endif