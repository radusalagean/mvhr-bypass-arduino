#ifndef STATE_CONTROLLER_H
#define STATE_CONTROLLER_H

#include "State.h"
#include "InternalStorage.h"

#define STATE_SIGNATURE_ADDRESS 0
#define STATE_SIGNATURE 0b10101010

#define STATE_VERSION_ADDRESS (STATE_SIGNATURE_ADDRESS + 1)
#define STATE_VERSION 2

#define STATE_ADDRESS (STATE_VERSION_ADDRESS + 1)

class StateController
{
private:
    State state;
    InternalStorage* internalStorage = NULL;
    void persist();
    void initializeStateWithDefaults();
public:
    StateController(InternalStorage* internalStorage);
    void loadPersistedState();
    // Getters
    inline bool isHrModeAuto() __attribute__((always_inline));
    inline bool isHrDisabled() __attribute__((always_inline));
    inline uint8_t getIntEvMin() __attribute__((always_inline));
    inline uint8_t getExtAdMin() __attribute__((always_inline));
    inline uint8_t getExtAdMax() __attribute__((always_inline));
    inline float getHysteresis() __attribute__((always_inline));
    // Setters
    void setHrModeAuto(const bool hrModeAuto);
    void setHrDisabled(const bool hrDisabled);
    void setTemperatures(const uint8_t intEvMin, const uint8_t extAdMin, const uint8_t extAdMax);
    void setHysteresis(const float hysteresis);
    // Helpers
    void toggleHrModeAuto();
};

// Inline Methods (Declared in the header file for inlining to work)

bool StateController::isHrModeAuto() { return state.hrModeAuto; }
bool StateController::isHrDisabled() { return state.hrDisabled; }
uint8_t StateController::getIntEvMin() { return state.intEvMin; }
uint8_t StateController::getExtAdMin() { return state.extAdMin; }
uint8_t StateController::getExtAdMax() { return state.extAdMax; }
float StateController::getHysteresis() { return state.hysteresis; }

#endif