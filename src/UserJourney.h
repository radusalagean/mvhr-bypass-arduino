#ifndef USER_JOURNEY_H
#define USER_JOURNEY_H

class UserJourney;

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
    unsigned long lastKeyPressTime = 0L;
    void removeCurrentPage();

public:
    UserJourney(Display* display, Relay* relay, Temperature* temperature, 
                StateController* stateController);
    void init();
    void processKey(uint8_t& key);
    void renderCurrentPage();
    void setManualHrDisabled(const bool disabled);
    void processOpcode(const uint8_t& opcode);
    inline void powerUpDisplay() __attribute__((always_inline));
    inline void powerDownDisplay() __attribute__((always_inline));
    inline unsigned long getLastKeyPressTime() __attribute__((always_inline));
    inline bool isDisplayPoweredUp() __attribute__((always_inline));
};

void UserJourney::powerUpDisplay()
{
    display->powerUp();
}

void UserJourney::powerDownDisplay()
{
    display->powerDown();
}

unsigned long UserJourney::getLastKeyPressTime()
{
    return lastKeyPressTime;
}

bool UserJourney::isDisplayPoweredUp()
{
    return display->isPoweredUp();
}

#endif