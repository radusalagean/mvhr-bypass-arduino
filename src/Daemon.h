#ifndef DAEMON_H
#define DAEMON_H

#include "State.h"
#include "Temperature.h"
#include "UserJourney.h"

#define TEMP_REFRESH_INTERVAL 5000 // 5s

class Daemon
{
private:
    State* state;
    Temperature* temperature;
    UserJourney* userJourney;
    long lastTempRefresh = 0L;

public:
    Daemon(State* state, Temperature* temperature, UserJourney* userJourney);
    void handleOutstandingJobs();
};

#endif