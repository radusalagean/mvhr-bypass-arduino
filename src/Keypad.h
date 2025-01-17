#ifndef KEYPAD_H
#define KEYPAD_H

#include <Arduino.h>

#define KEYPAD_INTERRUPT_PIN 3
#define ANALOG_PIN A0

#define KEY_NONE 0
#define KEY_LEFT 1
#define KEY_RIGHT 2
#define KEY_MINUS 3
#define KEY_PLUS 4
#define KEY_SPECIAL 5

#define PROCESS_DELAY_MS 300

struct KeySpec
{
    uint16_t minRes;
    uint16_t maxRes;
    uint8_t key;
};

class Keypad
{
private:
    volatile uint8_t keyToProcess = KEY_NONE;
    volatile unsigned long lastProcessTime = 0L;

public:
    void onInterrupt();
    uint8_t getKeyToProcess();
    void markAsProcessed();
};

#endif