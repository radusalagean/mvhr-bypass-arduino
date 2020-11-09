#ifndef RELAY_H
#define RELAY_H

#include <Arduino.h>
#include "State.h"

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