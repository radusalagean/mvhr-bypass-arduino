#ifndef USER_JOURNEY_H
#define USER_JOURNEY_H

#include "Page.h"
#include "Keypad.h"
#include "Display.h"

class UserJourney
{
private:
    Page* currentPage = NULL;
    Display* display = NULL;
    void processKeyLeft();
public:
    UserJourney(Display* display);
    void init();
    void processKey(uint8_t& key);
    void render();
};

#endif