#include "Clock.h"

void Clock::init() {
    if (!rtc.begin()) {
        // RTC Chip not found
        Serial.println("Unable to find RTC");
        Serial.flush();
        abort();
    }
    Serial.println("RTC initialized successfully");
}

DateTime Clock::now() {
    return rtc.now();
}