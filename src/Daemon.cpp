#include "Daemon.h"

Daemon::Daemon(State* state, Temperature* temperature, UserJourney* userJourney)
{
    this->state = state;
    this->temperature = temperature;
    this->userJourney = userJourney;
}

void Daemon::handleOutstandingJobs()
{
    // Refresh Temperature
    if (millis() - lastTempRefresh > TEMP_REFRESH_INTERVAL)
    {
        temperature->requestTemperatures();
        userJourney->processOpcode(OPCODE_REFRESH_TEMP_VALUES_ON_SCREEN);
        lastTempRefresh = millis();
    }
}