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

public:
    Daemon(StateController* stateController, Temperature* temperature, UserJourney* userJourney);
    void handleOutstandingJobs();
};

#endif