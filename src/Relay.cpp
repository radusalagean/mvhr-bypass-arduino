#include "Relay.h"

Relay::Relay(State* state)
{
    this->state = state;
}

void Relay::init()
{
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, HIGH);
}

void Relay::closeCircuit()
{
    digitalWrite(RELAY_PIN, LOW);
    state->hrDisabled = true;
}

void Relay::openCircuit()
{
    digitalWrite(RELAY_PIN, HIGH);
    state->hrDisabled = false;
}