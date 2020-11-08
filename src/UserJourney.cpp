#include "UserJourney.h"

UserJourney::UserJourney(Display* display, Relay* relay, Temperature* temperature)
{
    this->display = display;
    this->relay = relay;
    this->temperature = temperature;
}

void UserJourney::init()
{
    currentPage = new HomePage(display, temperature);
}

void UserJourney::processKey(uint8_t &key)
{
    switch (key)
    {
    case KEY_LEFT:
        if (currentPage->leftAction != NULL)
        {
            currentPage->processCommandAreaAction(*(currentPage->leftAction));
            processOpcode(currentPage->leftAction->opcode);
        }
        break;
    case KEY_RIGHT:
        if (currentPage->rightAction != NULL)
        {
            currentPage->processCommandAreaAction(*(currentPage->rightAction));
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
    currentPage->handleCronJobs();
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