#ifndef RELAY_H
#define RELAY_H

#include "StateController.h"
#include <Arduino.h>

#define RELAY_PIN 7

// HIGH opens circuit, LOW closes

class Relay
{
private:
    StateController* stateController = NULL;

public:
    Relay(StateController* stateController);
    void init();
    void closeCircuit();
    void openCircuit();
};

#endif