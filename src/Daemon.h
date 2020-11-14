#ifndef DAEMON_H
#define DAEMON_H

#include "StateController.h"
#include "Temperature.h"
#include "UserJourney.h"

#define TEMP_REFRESH_INTERVAL 5000 // 5s

class Daemon
{
private:
    StateController* stateController;
    Temperature* temperature;
    UserJourney* userJourney;
    long lastTempRefresh = 0L;
    inline bool isTempOutsideOfBufferZone(const float& candidate, const TempConfig& config) __attribute__((always_inline));

public:
    Daemon(StateController* stateController, Temperature* temperature, UserJourney* userJourney);
    void handleOutstandingJobs();
};

bool Daemon::isTempOutsideOfBufferZone(const float& candidate, const TempConfig& config)
{
    Serial.print(config.bufferLow);
    Serial.print(" ");
    Serial.print(candidate);
    Serial.print(" ");
    Serial.println(config.bufferHigh);
    return candidate <= config.bufferLow || candidate >= config.bufferHigh;
}

#endif