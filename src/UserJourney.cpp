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
        if (!stateController->isHrModeAuto()) // TODO Move in separate method
        {
            uint8_t opcode = stateController->isHrDisabled() ? OPCODE_HR_ON : OPCODE_HR_OFF;
            processOpcode(opcode);
        }
        break;
    default:
        break;
    }
}

void UserJourney::renderCurrentPage()
{
    currentPage->render();
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
    case OPCODE_SWITCH_MODE:
        stateController->toggleHrModeAuto();
        currentPage->processOpcode(opcode);
        break;
    case OPCODE_REFRESH_TEMP_VALUES_ON_SCREEN:
        currentPage->processOpcode(OPCODE_REFRESH_TEMP_VALUES_ON_SCREEN);
        break;
    case OPCODE_OPEN_PAGE_TEMPERATURE_SETTINGS:
        removeCurrentPage();
        currentPage = new TemperatureSettingsPage(display, temperature, stateController);
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