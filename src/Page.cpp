#include "Page.h"

ControlAreaAction controlAreaActions[] =
{
    { "HR enable", OPCODE_HR_ON },
    { "HR disable", OPCODE_HR_OFF }
};

/**
 * BASE page
 **/

Page::Page(Display* display)
{
    Serial.println(F("Base page ctor called"));
    this->display = display;
}

void Page::drawControlArea()
{
    display->tft.setTextColor(TFT_WHITE, TFT_BLACK);
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

void Page::processControlAreaAction(ControlAreaAction& action)
{
    processOpcode(action.opcode);
}

bool Page::processOpcode(const uint8_t& opcode)
{
    Serial.println(F("base attempt to process opcode"));
    // TODO Handle Go Home OPCODE
    return false;
}

/**
 * HOME page
 **/

HomePage::HomePage(Display* display) : Page::Page(display)
{
    leftAction = &controlAreaActions[1];
}

void HomePage::render()
{
    if (!invalidated)
    {
        return;
    }
    drawControlArea();
    drawHrState();
    invalidated = false;
}

void HomePage::drawHrState()
{
    display->tft.fillCircle(PAGE_MID_WIDTH, PAGE_MID_HEIGHT, 12, hrEnabled ? TFT_RED : TFT_BLUE);
}

bool HomePage::processOpcode(const uint8_t& opcode)
{
    if (Page::processOpcode(opcode))
    {
        // OPCODE handled in the base class
        return true;
    }
    switch (opcode)
    {
    case OPCODE_HR_ON:
        hrEnabled = true;
        leftAction = &controlAreaActions[1];
        invalidated = true;
        break;
    case OPCODE_HR_OFF:
        hrEnabled = false;
        leftAction = &controlAreaActions[0];
        invalidated = true;
        break;
    default:
        break;
    }
    return true;
}