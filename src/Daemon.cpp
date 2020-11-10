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
        if (state->hrModeAuto)
        {
            float currentIntEv = temperature->getTempIntEv();
            float currentExtAd = temperature->getTempExtAd();
            // TODO Hysteresis
            if (currentIntEv >= state->intEvMin &&
                state->extAdMin <= currentExtAd && currentExtAd <= state->extAdMax)
            {
                if (!state->hrDisabled)
                    userJourney->processOpcode(OPCODE_HR_OFF);
            }
            else if (state->hrDisabled)
            {
                userJourney->processOpcode(OPCODE_HR_ON);
            }
        }
        userJourney->processOpcode(OPCODE_REFRESH_TEMP_VALUES_ON_SCREEN);
        lastTempRefresh = millis();
    }
}