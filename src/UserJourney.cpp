#include "UserJourney.h"

UserJourney::UserJourney(Display* display, Relay* relay)
{
    this->display = display;
    this->relay = relay;
}

void UserJourney::init()
{
    currentPage = new HomePage(display);
}

void UserJourney::processKey(uint8_t &key)
{
    switch (key)
    {
    case KEY_LEFT:
        if (currentPage->leftAction != NULL)
        {
            currentPage->processControlAreaAction(*(currentPage->leftAction));
            processOpcode(currentPage->leftAction->opcode);
        }
        break;
    case KEY_RIGHT:
        if (currentPage->rightAction != NULL)
        {
            currentPage->processControlAreaAction(*(currentPage->rightAction));
            processOpcode(currentPage->rightAction->opcode);
        }
        break;
    case KEY_MINUS:

        break;
    case KEY_PLUS:

        break;
    case KEY_HOME:

        break;
    default:
        break;
    }
}

void UserJourney::render()
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
    default:
        break;
    }
}