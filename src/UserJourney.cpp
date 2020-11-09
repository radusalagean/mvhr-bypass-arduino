#include "UserJourney.h"

UserJourney::UserJourney(Display* display, Relay* relay, Temperature* temperature, State* state)
{
    this->display = display;
    this->relay = relay;
    this->temperature = temperature;
    this->state = state;
}

void UserJourney::init()
{
    currentPage = new HomePage(display, temperature, state);
}

void UserJourney::processKey(uint8_t& key)
{
    switch (key)
    {
    case KEY_LEFT:
        if (currentPage->leftAction != NULL)
        {
            processOpcode(currentPage->leftAction->opcode);
            currentPage->processCommandAreaAction(*(currentPage->leftAction));
        }
        break;
    case KEY_RIGHT:
        if (currentPage->rightAction != NULL)
        {
            processOpcode(currentPage->rightAction->opcode);
            currentPage->processCommandAreaAction(*(currentPage->rightAction));
        }
        break;
    case KEY_MINUS:

        break;
    case KEY_PLUS:

        break;
    case KEY_SPECIAL:
        if (!state->hrModeAuto) // TODO Move in separate method
        {
            uint8_t opcode = state->hrDisabled ? OPCODE_HR_OFF : OPCODE_HR_ON;
            processOpcode(opcode);
            currentPage->processOpcode(opcode);
        }
        break;
    default:
        break;
    }
}

void UserJourney::renderCurrentPage()
{
    currentPage->render();
}

void UserJourney::processOpcode(const uint8_t& opcode)
{
    switch (opcode)
    {
    case OPCODE_HR_ON:
        relay->closeCircuit();
        break;
    case OPCODE_HR_OFF:
        relay->openCircuit();
        break;
    case OPCODE_SWITCH_MODE:
        state->hrModeAuto = !state->hrModeAuto;
        break;
    case OPCODE_REFRESH_TEMP_VALUES_ON_SCREEN:
        currentPage->processOpcode(OPCODE_REFRESH_TEMP_VALUES_ON_SCREEN);
        break;
    default:
        break;
    }
}