#ifndef USER_JOURNEY_H
#define USER_JOURNEY_H

#include "Page.h"
#include "Keypad.h"
#include "Display.h"
#include "Relay.h"

class UserJourney
{
private:
    Page* currentPage = NULL;
    Display* display = NULL;
    Relay* relay = NULL;
    void processOpcode(const uint8_t& opcode);
public:
    UserJourney(Display* display, Relay* relay);
    void init();
    void processKey(uint8_t& key);
    void render();
};

#endif