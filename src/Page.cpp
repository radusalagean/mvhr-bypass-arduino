#include "Page.h"

ControlAreaAction controlAreaActions[] =
{
    { "HR on", OPCODE_HR_ON },
    { "HR off", OPCODE_HR_OFF }
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
    display->ucg.setFontPosBottom();
    display->setColorRGB(255, 255, 255);
    display->ucg.drawLine(0, DISPLAY_HEIGHT - COMMAND_AREA_HEIGHT,
        DISPLAY_WIDTH, DISPLAY_HEIGHT - COMMAND_AREA_HEIGHT);
    if (leftAction != NULL)
    {
        Serial.println(leftAction->text);
        display->ucg.setPrintPos(0, DISPLAY_HEIGHT);
        display->ucg.print(leftAction->text);
    }
    if (rightAction != NULL)
    {
        ucg_int_t textWidth = display->ucg.getStrWidth(rightAction->text);
        display->ucg.setPrintPos(DISPLAY_WIDTH - textWidth, DISPLAY_HEIGHT);
        display->ucg.print(rightAction->text);
    }
    display->ucg.setFontPosBaseline();
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
    leftAction = &controlAreaActions[0];
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
    if (hrEnabled)
    {
        display->setColorRGB(224, 43, 43);
    }
    else
    {
        display->setColorRGB(34, 174, 240);
    }
    display->ucg.drawDisc(DISPLAY_WIDTH / 2,
                          (DISPLAY_HEIGHT - COMMAND_AREA_HEIGHT) / 2,
                          12,
                          UCG_DRAW_ALL);
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