#include "Page.h"

ControlAreaAction controlAreaActions[] =
{
    { "Left", OPCODE_TEST_L },
    { "Right", OPCODE_TEST_R }
};

/**
 * BASE page
 **/

void Page::drawControlArea(Display& display)
{
    display.ucg.setFontPosBottom();
    display.ucg.drawLine(0, DISPLAY_HEIGHT - COMMAND_AREA_HEIGHT,
        DISPLAY_WIDTH, DISPLAY_HEIGHT - COMMAND_AREA_HEIGHT);
    if (leftAction != NULL)
    {
        display.ucg.setPrintPos(0, DISPLAY_HEIGHT);
        display.ucg.print(leftAction->text);
    }
    if (rightAction != NULL)
    {
        ucg_int_t textWidth = display.ucg.getStrWidth(rightAction->text);
        display.ucg.setPrintPos(DISPLAY_WIDTH - textWidth, DISPLAY_HEIGHT);
        display.ucg.print(rightAction->text);
    }
    display.ucg.setFontPosBaseline();
}

void Page::processControlAreaAction(ControlAreaAction& action)
{
    processOpcode(action.opcode);
}

bool Page::processOpcode(uint8_t& opcode)
{
    Serial.println(F("base attempt to process opcode"));
    // TODO Handle Go Home OPCODE
    return false;
}

/**
 * HOME page
 **/

HomePage::HomePage()
{
    leftAction = &controlAreaActions[0];
    rightAction = &controlAreaActions[1];
}

void HomePage::render(Display& display)
{
    if (!invalidated)
    {
        return;
    }
    Serial.println(F("draw page"));
    drawControlArea(display);
    invalidated = false;
}

bool HomePage::processOpcode(uint8_t& opcode)
{
    if (Page::processOpcode(opcode))
    {
        // OPCODE handled in the base class
        return true;
    }
    switch (opcode)
    {
    case OPCODE_TEST_L:
        Serial.println(F("OPCODE_TEST_L handled"));
        break;
    case OPCODE_TEST_R:
        Serial.println(F("OPCODE_TEST_R handled"));
        break;
    default:
        break;
    }
    return true;
}