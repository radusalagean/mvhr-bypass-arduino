#ifndef USER_JOURNEY_H
#define USER_JOURNEY_H

#include "Page.h"
#include "Keypad.h"

class UserJourney
{
private:
    Page* currentPage = NULL;
    void processKeyLeft();
public:
    void init();
    void processKey(uint8_t& key);
    void render(Display& display);
};

#endif