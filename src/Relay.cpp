#include "Relay.h"

Relay::Relay(StateController* stateController)
{
    this->stateController = stateController;
}

void Relay::init()
{
    pinMode(RELAY_PIN, OUTPUT);
    // Sync with state
    digitalWrite(RELAY_PIN, stateController->isHrDisabled() ? LOW : HIGH);
}

void Relay::closeCircuit()
{
    digitalWrite(RELAY_PIN, LOW);
    stateController->setHrDisabled(true);
}

void Relay::openCircuit()
{
    digitalWrite(RELAY_PIN, HIGH);
    stateController->setHrDisabled(false);
}