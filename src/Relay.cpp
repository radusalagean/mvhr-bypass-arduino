#include "Relay.h"

void Relay::init()
{
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, HIGH);
}

void Relay::closeCircuit()
{
    digitalWrite(RELAY_PIN, LOW);
}

void Relay::openCircuit()
{
    digitalWrite(RELAY_PIN, HIGH);
}