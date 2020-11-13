#ifndef STATE_H
#define STATE_H

#include <Arduino.h>

typedef struct
{
    bool hrModeAuto;
    bool hrDisabled;
    uint8_t intEvMin;
    uint8_t extAdMin;
    uint8_t extAdMax;
} State;

#endif