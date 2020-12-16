#include "UserJourney.h"

UserJourney::UserJourney(Display* display, Relay* relay, Temperature* temperature, 
                         StateController* stateController)
{
    this->display = display;
    this->relay = relay;
    this->temperature = temperature;
    this->stateController = stateController;
}

void UserJourney::init()
{
    currentPage = new HomePage(display, temperature, stateController);
}

void UserJourney::processKey(uint8_t& key)
{
    lastKeyPressTime = millis();
    if (!display->isPoweredUp())
    {
        powerUpDisplay();
        return;
    }
    switch (key)
    {
    case KEY_LEFT:
        if (currentPage->leftAction != NULL)
        {
            processOpcode(currentPage->leftAction->opcode);
        }
        break;
    case KEY_RIGHT:
        if (currentPage->rightAction != NULL)
        {
            processOpcode(currentPage->rightAction->opcode);
        }
        break;
    case KEY_MINUS:
        currentPage->processOpcode(OPCODE_CONTEXTUAL_MINUS);
        break;
    case KEY_PLUS:
        currentPage->processOpcode(OPCODE_CONTEXTUAL_PLUS);
        break;
    case KEY_SPECIAL:
        setManualHrDisabled(!stateController->isHrDisabled());
        break;
    default:
        break;
    }
}

void UserJourney::renderCurrentPage()
{
    currentPage->render();
}

void UserJourney::setManualHrDisabled(const bool disabled)
{
    if (!stateController->isHrModeAuto())
    {
        uint8_t opcode = disabled ? OPCODE_HR_OFF : OPCODE_HR_ON;
        processOpcode(opcode);
    }
}

void UserJourney::processOpcode(const uint8_t& opcode)
{
    switch (opcode)
    {
    case OPCODE_HR_ON:
        relay->openCircuit();
        currentPage->processOpcode(opcode);
        break;
    case OPCODE_HR_OFF:
        relay->closeCircuit();
        currentPage->processOpcode(opcode);
        break;
    case OPCODE_SWITCH_MODE: // Used by HomePage
        stateController->toggleHrModeAuto();
        currentPage->processOpcode(opcode);
        break;
    case OPCODE_HR_MODE_AUTO: // Used by SerialNetwork
        stateController->setHrModeAuto(true);
        currentPage->processOpcode(opcode);
        break;
    case OPCODE_HR_MODE_MANUAL: // Used by SerialNetwork
        stateController->setHrModeAuto(false);
        currentPage->processOpcode(opcode);
        break;
    case OPCODE_REFRESH_TEMP_VALUES_ON_SCREEN:
        currentPage->processOpcode(OPCODE_REFRESH_TEMP_VALUES_ON_SCREEN);
        break;
    case OPCODE_OPEN_PAGE_SETTINGS:
        removeCurrentPage();
        currentPage = new SettingsPage(display, temperature, stateController);
        break;
    case OPCODE_OPEN_PAGE_HOME:
        removeCurrentPage();
        currentPage = new HomePage(display, temperature, stateController);
        break;
    case OPCODE_CONTEXTUAL_EDIT:
    case OPCODE_CONTEXTUAL_NEXT:
    case OPCODE_CONTEXTUAL_CANCEL:
    case OPCODE_CONTEXTUAL_SAVE:
        currentPage->processOpcode(opcode);
        break;
    default:
        break;
    }
}

void UserJourney::removeCurrentPage()
{
    if (currentPage != NULL)
        delete currentPage;
}