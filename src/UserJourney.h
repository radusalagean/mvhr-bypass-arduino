#ifndef USER_JOURNEY_H
#define USER_JOURNEY_H

#include "Display.h"
#include "Keypad.h"
#include "Page.h"
#include "Relay.h"
#include "State.h"
#include "Temperature.h"

class UserJourney
{
private:
    Page* currentPage = NULL;
    Display* display = NULL;
    Relay* relay = NULL;
    Temperature* temperature = NULL;
    State* state = NULL;
    void processOpcode(const uint8_t& opcode);

public:
    UserJourney(Display* display, Relay* relay, Temperature* temperature, State* state);
    void init();
    void processKey(uint8_t& key);
    void render();
};

#endif