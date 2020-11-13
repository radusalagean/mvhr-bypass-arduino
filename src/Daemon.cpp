#include "Daemon.h"

Daemon::Daemon(StateController* stateController, Temperature* temperature, UserJourney* userJourney)
{
    this->stateController = stateController;
    this->temperature = temperature;
    this->userJourney = userJourney;
}

void Daemon::handleOutstandingJobs()
{
    // Refresh Temperature
    if (millis() - lastTempRefresh > TEMP_REFRESH_INTERVAL)
    {
        temperature->requestTemperatures();
        if (stateController->isHrModeAuto())
        {
            float currentIntEv = temperature->getTempIntEv();
            float currentExtAd = temperature->getTempExtAd();
            // TODO Hysteresis
            if (currentIntEv >= stateController->getIntEvMin() &&
                stateController->getExtAdMin() <= currentExtAd && currentExtAd <= stateController->getExtAdMax())
            {
                if (!stateController->isHrDisabled())
                    userJourney->processOpcode(OPCODE_HR_OFF);
            }
            else if (stateController->isHrDisabled())
            {
                userJourney->processOpcode(OPCODE_HR_ON);
            }
        }
        userJourney->processOpcode(OPCODE_REFRESH_TEMP_VALUES_ON_SCREEN);
        lastTempRefresh = millis();
    }
}