#include "StateController.h"

StateController::StateController(InternalStorage* internalStorage)
{
    this->internalStorage = internalStorage;
}

void StateController::init(SerialNetwork* serialNetwork)
{
    this->serialNetwork = serialNetwork;
}

void StateController::persist()
{
    internalStorage->put(STATE_SIGNATURE_ADDRESS, STATE_SIGNATURE);
    internalStorage->put(STATE_VERSION_ADDRESS, STATE_VERSION);
    internalStorage->put(STATE_ADDRESS, state);
}

void StateController::persistAndSendState()
{
    persist();
    serialNetwork->sendState();
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
    persistAndSendState();
}

void StateController::setHrDisabled(const bool hrDisabled)
{
    state.hrDisabled = hrDisabled;
    persistAndSendState();
}

void StateController::setTemperatures(const float hysteresis, const uint8_t intEvMin, 
                                      const uint8_t extAdMin, const uint8_t extAdMax)
{
    if (STATE_HYST_RANGE_LOW <= hysteresis && hysteresis <= STATE_HYST_RANGE_HIGH)
        state.hysteresis = hysteresis;
    if (STATE_TEMPERATURES_RANGE_LOW <= intEvMin && intEvMin <= STATE_TEMPERATURES_RANGE_HIGH)
        state.intEvMin = intEvMin;
    if (STATE_TEMPERATURES_RANGE_LOW <= extAdMin && extAdMin + STATE_RANGE_MIN_VALUES <= extAdMax && extAdMax <= STATE_TEMPERATURES_RANGE_HIGH)
    {
        state.extAdMin = extAdMin;
        state.extAdMax = extAdMax;
    }
    persistAndSendState();
}

// Helpers

void StateController::toggleHrModeAuto()
{
    state.hrModeAuto = !state.hrModeAuto;
    persistAndSendState();
}