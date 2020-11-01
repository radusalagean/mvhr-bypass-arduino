#include "UserJourney.h"

void UserJourney::init()
{
    currentPage = new HomePage();
}

void UserJourney::processKey(uint8_t &key)
{
    switch (key)
    {
    case KEY_LEFT:
        if (currentPage->leftAction != NULL)
        {
            currentPage->processControlAreaAction(*(currentPage->leftAction));
        }
        break;
    case KEY_RIGHT:
        if (currentPage->rightAction != NULL)
        {
            currentPage->processControlAreaAction(*(currentPage->rightAction));
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

void UserJourney::render(Display &display)
{
    currentPage->render(display);
}