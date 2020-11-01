#include "Clock.h"

void Clock::init() 
{
    if (!rtc.begin()) 
    {
        // RTC Chip not found
        Serial.println(F("Unable to find RTC"));
        Serial.flush();
        abort();
    }
    Serial.println(F("RTC initialized successfully"));
}

DateTime Clock::now()
{
    return rtc.now();
}