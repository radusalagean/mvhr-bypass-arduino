#include "Page.h"

/**
 * BASE page
 **/

Page::Page(Display* display, Temperature* temperature, State* state)
{
    this->display = display;
    this->temperature = temperature;
    this->state = state;
}

void Page::drawCommandArea()
{
    display->tft.fillRect(0, DISPLAY_HEIGHT - COMMAND_AREA_HEIGHT + 1, DISPLAY_WIDTH, DISPLAY_HEIGHT, TFT_BLACK);
    display->tft.drawLine(0, PAGE_HEIGHT,
                          DISPLAY_WIDTH - 1, PAGE_HEIGHT, TFT_WHITE);

    if (leftAction != NULL)
    {
        display->tft.drawStringWithDatum(leftAction->text, 0, DISPLAY_HEIGHT, 2, BL_DATUM);
    }
    if (rightAction != NULL)
    {
        display->tft.drawStringWithDatum(rightAction->text, DISPLAY_WIDTH, DISPLAY_HEIGHT, 2, BR_DATUM);
    }
}

bool Page::render()
{
    if (initialized)
    {
        if (invalidation != 0)
            refreshInvalidatedAreas();
        return true;
    }
    else
    {
        display->clearScreen();
    }
    return false;
}

void Page::refreshInvalidatedAreas()
{
}

bool Page::processOpcode(const uint8_t& opcode)
{
    // TODO Handle Go Home OPCODE
    return false;
}

