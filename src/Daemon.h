#ifndef DAEMON_H
#define DAEMON_H

#include "StateController.h"
#include "Temperature.h"
#include "UserJourney.h"
#include "SerialNetwork.h"

#define TEMP_REFRESH_INTERVAL 5000 // 5s

#define DISABLE_SCREEN_TIMEOUT // can be uncommented for debugging sessions
#define SCREEN_TIMEOUT 30000 // 30s

class Daemon
{
private:
    StateController* stateController;
    Temperature* temperature;
    UserJourney* userJourney;
    SerialNetwork* serialNetwork;
    unsigned long lastTempRefresh = 0L;
    inline bool isTempOutsideOfBufferZone(const float& candidate, const TempConfig& config) __attribute__((always_inline));

public:
    Daemon(StateController* stateController, Temperature* temperature, UserJourney* userJourney, SerialNetwork* serialNetwork);
    void refreshTemperatureData();
    void handleOutstandingJobs();
};

bool Daemon::isTempOutsideOfBufferZone(const float& candidate, const TempConfig& config)
{
    return candidate <= config.bufferLow || candidate >= config.bufferHigh;
}

#endif