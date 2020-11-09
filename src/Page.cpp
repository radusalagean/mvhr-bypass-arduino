#include "Page.h"

/**
 * BASE page
 **/

Page::Page(Display* display, Temperature* temperature, State* state)
{
    Serial.println(F("Base page ctor called"));
    this->display = display;
    this->temperature = temperature;
    this->state = state;
}

void Page::drawCommandArea()
{
    display->tft.fillRect(0, DISPLAY_HEIGHT - COMMAND_AREA_HEIGHT + 1, DISPLAY_WIDTH, DISPLAY_HEIGHT, TFT_BLACK);
    display->tft.drawLine(0, PAGE_HEIGHT,
        DISPLAY_WIDTH, PAGE_HEIGHT, TFT_WHITE);
    if (leftAction != NULL)
    {
        display->tft.drawString(leftAction->text, 0, DISPLAY_HEIGHT, 2);
    }
    if (rightAction != NULL)
    {
        display->tft.drawRightString(rightAction->text, DISPLAY_WIDTH, DISPLAY_HEIGHT, 2);
    }
}

void Page::render()
{

}

void Page::refreshInvalidatedAreas()
{

}

void Page::processCommandAreaAction(CommandAreaAction& action)
{
    processOpcode(action.opcode);
}

bool Page::processOpcode(const uint8_t& opcode)
{
    Serial.println(F("base attempt to process opcode"));
    // TODO Handle Go Home OPCODE
    return false;
}

void Page::handleCronJobs()
{

}
