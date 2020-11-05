#ifndef RELAY_H
#define RELAY_H

#include <Arduino.h>

#define RELAY_PIN 7

// HIGH opens circuit, LOW closes

class Relay
{
private:
public:
    void init();
    void closeCircuit();
    void openCircuit();
};

#endif