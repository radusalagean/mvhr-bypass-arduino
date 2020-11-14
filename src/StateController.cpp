#include "StateController.h"

StateController::StateController(InternalStorage* internalStorage)
{
    this->internalStorage = internalStorage;
}

void StateController::persist()
{
    internalStorage->put(STATE_SIGNATURE_ADDRESS, STATE_SIGNATURE);
    internalStorage->put(STATE_VERSION_ADDRESS, STATE_VERSION);
    internalStorage->put(STATE_ADDRESS, state);
}

void StateController::initializeStateWithDefaults()
{
    // Clear the internal storage
    internalStorage->clear();
    // Initialize the state and persist it
    state = State
    {
        false, /* hrModeAuto */
        false, /* hrDisabled */
        23, /* intEvMin */
        12, /* extAdMin */
        25, /* extAdMax */
        0.5f /* hysteresis */
    };
    persist();
}

void StateController::loadPersistedState()
{
    // Check if we have the state signature at the first address
    byte signature = 0b0;
    uint8_t version = 0;
    internalStorage->get(STATE_SIGNATURE_ADDRESS, signature);
    if (signature == STATE_SIGNATURE) // The state was persisted before
    {
        internalStorage->get(STATE_VERSION_ADDRESS, version);
        if (version == STATE_VERSION)
        {
            // load the previous state
            internalStorage->get(STATE_ADDRESS, state);
        }
        else
            initializeStateWithDefaults();
    }
    else
        initializeStateWithDefaults();
}

// Setters

void StateController::setHrModeAuto(const bool hrModeAuto)
{
    state.hrModeAuto = hrModeAuto;
    persist();
}

void StateController::setHrDisabled(const bool hrDisabled)
{
    state.hrDisabled = hrDisabled;
    persist();
}

void StateController::setTemperatures(const uint8_t intEvMin, const uint8_t extAdMin, const uint8_t extAdMax)
{
    state.intEvMin = intEvMin;
    state.extAdMin = extAdMin;
    state.extAdMax = extAdMax;
    persist();
}

void StateController::setHysteresis(const float hysteresis)
{
    state.hysteresis = hysteresis;
    persist();
}

// Helpers

void StateController::toggleHrModeAuto()
{
    state.hrModeAuto = !state.hrModeAuto;
    persist();
}