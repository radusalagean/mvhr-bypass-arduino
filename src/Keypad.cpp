#include "Keypad.h"

const KeySpec keySpecs[5] =
    {
        {932, 1023, KEY_LEFT},
        {776, 931, KEY_RIGHT},
        {664, 775, KEY_MINUS},
        {581, 663, KEY_PLUS},
        {509, 580, KEY_SPECIAL}
    };

void Keypad::onInterrupt()
{
    if (millis() - lastProcessTime < PROCESS_DELAY_MS)
    {
        return;
    }
    if (keyToProcess != KEY_NONE)
    {
        // Serial.println(F("Previous interrupt not processed yet"));
        return;
    }
    uint16_t analogInput = analogRead(ANALOG_PIN);
    // Serial.println(analogInput);
    for (int i = 0; i < 5; i++)
    {
        KeySpec ks = keySpecs[i];
        if (ks.minRes <= analogInput && analogInput <= ks.maxRes)
        {
            keyToProcess = ks.key;
            break;
        }
    }
    // Serial.println(keyToProcess);
    lastProcessTime = millis();
}

uint8_t Keypad::getKeyToProcess()
{
    return keyToProcess;
}

void Keypad::markAsProcessed()
{
    keyToProcess = KEY_NONE;
}