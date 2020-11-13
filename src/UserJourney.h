#ifndef USER_JOURNEY_H
#define USER_JOURNEY_H

#include "Display.h"
#include "Keypad.h"
#include "Page.h"
#include "Relay.h"
#include "StateController.h"
#include "Temperature.h"

class UserJourney
{
private:
    Page* currentPage = NULL;
    Display* display = NULL;
    Relay* relay = NULL;
    Temperature* temperature = NULL;
    StateController* stateController = NULL;
    void removeCurrentPage();

public:
    UserJourney(Display* display, Relay* relay, Temperature* temperature, 
                StateController* stateController);
    void init();
    void processKey(uint8_t& key);
    void renderCurrentPage();
    void processOpcode(const uint8_t& opcode);
};

#endif