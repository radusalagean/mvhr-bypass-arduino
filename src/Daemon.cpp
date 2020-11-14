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
            TempConfig intEvMinConfig =
            {
                stateController->getIntEvMin() - stateController->getHysteresis(), /* bufferLow */
                stateController->getIntEvMin(), /* target */
                stateController->getIntEvMin() + stateController->getHysteresis() /* bufferHigh */
            };
            TempConfig extAdMinConfig =
            {
                stateController->getExtAdMin() - stateController->getHysteresis(), /* bufferLow */
                stateController->getExtAdMin(), /* target */
                stateController->getExtAdMin() + stateController->getHysteresis() /* bufferHigh */
            };
            TempConfig extAdMaxConfig =
            {
                stateController->getExtAdMax() - stateController->getHysteresis(), /* bufferLow */
                stateController->getExtAdMax(), /* target */
                stateController->getExtAdMax() + stateController->getHysteresis() /* bufferHigh */
            };
            // Check if the current temps lay outside the buffer zones
            if (isTempOutsideOfBufferZone(temperature->getTempIntEv(), intEvMinConfig) &&
                isTempOutsideOfBufferZone(temperature->getTempExtAd(), extAdMinConfig) &&
                isTempOutsideOfBufferZone(temperature->getTempExtAd(), extAdMaxConfig))
            {
                Serial.println(F("Temp outside buffer zones (OK)"));
                if (temperature->getTempIntEv() >= stateController->getIntEvMin() &&
                    stateController->getExtAdMin() <= temperature->getTempExtAd() && 
                    temperature->getTempExtAd() <= stateController->getExtAdMax())
                {
                    if (!stateController->isHrDisabled())
                        userJourney->processOpcode(OPCODE_HR_OFF);
                }
                else if (stateController->isHrDisabled())
                {
                    userJourney->processOpcode(OPCODE_HR_ON);
                }
            }
            else
            {
                Serial.println(F("Temp inside buffer zones (WAIT)"));
            }
        }
        userJourney->processOpcode(OPCODE_REFRESH_TEMP_VALUES_ON_SCREEN);
        lastTempRefresh = millis();
    }
}