#ifndef RELAY_H
#define RELAY_H

#include "State.h"
#include <Arduino.h>

#define RELAY_PIN 7

// HIGH opens circuit, LOW closes

class Relay
{
private:
    State* state = NULL;

public:
    Relay(State* state);
    void init();
    void closeCircuit();
    void openCircuit();
};

#endif