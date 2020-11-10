#ifndef STATE_H
#define STATE_H

#include <Arduino.h>

class State
{
public:
    bool hrModeAuto = false;
    bool hrDisabled = false;
    uint8_t intEvMin = 23;
    uint8_t extAdMin = 12;
    uint8_t extAdMax = 25;
};

#endif