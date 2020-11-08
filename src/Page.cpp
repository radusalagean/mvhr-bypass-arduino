#include "Page.h"

CommandAreaAction commandAreaAction[] =
{
    { "HR enable", OPCODE_HR_ON },
    { "HR disable", OPCODE_HR_OFF }
};

/**
 * BASE page
 **/

Page::Page(Display* display, Temperature* temperature)
{
    Serial.println(F("Base page ctor called"));
    this->display = display;
    this->temperature = temperature;
}

void Page::drawCommandArea()
{
    display->tft.fillRect(0, DISPLAY_HEIGHT - COMMAND_AREA_HEIGHT + 1, DISPLAY_WIDTH, DISPLAY_HEIGHT, TFT_BLACK);
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

/**
 * HOME page
 **/

HomePage::HomePage(Display* display, Temperature* temperature) : Page::Page(display, temperature)
{
    leftAction = &commandAreaAction[1];
    initTablePoints();
}

void HomePage::initTablePoints()
{
    for (uint8_t row = 0; row < TABLE_ROWS; row++)
    {
        for (uint8_t col = 0; col < TABLE_COLUMNS; col++)
        {
            cellOrigin[row][col] = { 
                col * TABLE_CELL_WIDTH, 
                row * TABLE_CELL_HEIGHT 
            };
            cellCenter[row][col] = { 
                cellOrigin[row][col].x + TABLE_CELL_WIDTH_HALF, 
                cellOrigin[row][col].y + TABLE_CELL_HEIGHT_HALF 
            };
        }
    }
}

void HomePage::render()
{
    if (initialized)
    {
        if (invalidation != 0)
            refreshInvalidatedAreas();
        return;
    }
    drawCommandArea();
    drawTempTable();
    drawHrState();
    drawTempValues();
    initialized = true;
}

void HomePage::refreshInvalidatedAreas()
{
    if ((invalidation & HOME_PAGE_INVALIDATION_COMMAND_AREA) != 0)
        drawCommandArea();
    if ((invalidation & HOME_PAGE_INVALIDATION_TEMP_TABLE_CELLS) != 0)
        drawTempValues();
    if ((invalidation & HOME_PAGE_INVALIDATION_HR_STATE_CELL) != 0)
        drawHrState();
    invalidation = 0;
}

void HomePage::drawHrState()
{
    display->tft.fillCircle(cellCenter[0][0].x, cellCenter[0][0].y, 12, hrEnabled ? TFT_RED : TFT_BLUE);
}

void HomePage::drawTempTable()
{
    // Draw row separators
    for (uint8_t row = 1; row < TABLE_ROWS; row++)
    {
        display->tft.drawLine(0, cellOrigin[row][0].y,
            PAGE_WIDTH, cellOrigin[row][0].y, TFT_WHITE);
    }

    // Draw column separators
    for (uint8_t col = 1; col < TABLE_COLUMNS; col++)
    {
        display->tft.drawLine(cellOrigin[0][col].x, 0,
            cellOrigin[0][col].x, PAGE_HEIGHT, TFT_WHITE);
    }

    // Labels
    display->tft.drawStringWithDatum("AD", cellCenter[1][0].x, cellCenter[1][0].y, 2, CC_DATUM);
    display->tft.drawStringWithDatum("EV", cellCenter[2][0].x, cellCenter[2][0].y, 2, CC_DATUM);
    display->tft.drawStringWithDatum("EXT", cellCenter[0][1].x, cellCenter[0][1].y, 2, CC_DATUM);
    display->tft.drawStringWithDatum("INT", cellCenter[0][2].x, cellCenter[0][2].y, 2, CC_DATUM);
}

void HomePage::drawTempValues() // TODO Handle temp read error
{
    temperature->requestTemperatures();

    display->tft.fillRectExclusive(cellOrigin[1][1].x, cellOrigin[1][1].y, TABLE_CELL_WIDTH, TABLE_CELL_HEIGHT, TFT_BLACK);
    display->tft.fillRectExclusive(cellOrigin[1][2].x, cellOrigin[1][2].y, TABLE_CELL_WIDTH, TABLE_CELL_HEIGHT, TFT_BLACK);
    display->tft.fillRectExclusive(cellOrigin[2][1].x, cellOrigin[2][1].y, TABLE_CELL_WIDTH, TABLE_CELL_HEIGHT, TFT_BLACK);
    display->tft.fillRectExclusive(cellOrigin[2][2].x, cellOrigin[2][2].y, TABLE_CELL_WIDTH, TABLE_CELL_HEIGHT, TFT_BLACK);

    display->tft.drawFloatWithDatum(temperature->getTempExtAd(), 1, cellCenter[1][1].x, cellCenter[1][1].y, 4, CC_DATUM);
    display->tft.drawFloatWithDatum(temperature->getTempExtEv(), 1, cellCenter[2][1].x, cellCenter[2][1].y, 4, CC_DATUM);
    display->tft.drawFloatWithDatum(temperature->getTempIntAd(), 1, cellCenter[1][2].x, cellCenter[1][2].y, 4, CC_DATUM);
    display->tft.drawFloatWithDatum(temperature->getTempIntEv(), 1, cellCenter[2][2].x, cellCenter[2][2].y, 4, CC_DATUM);
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
        leftAction = &commandAreaAction[1];
        invalidation |= (HOME_PAGE_INVALIDATION_COMMAND_AREA | HOME_PAGE_INVALIDATION_HR_STATE_CELL);
        break;
    case OPCODE_HR_OFF:
        hrEnabled = false;
        leftAction = &commandAreaAction[0];
        invalidation |= (HOME_PAGE_INVALIDATION_COMMAND_AREA | HOME_PAGE_INVALIDATION_HR_STATE_CELL);
        break;
    default:
        break;
    }
    return true;
}

void HomePage::handleCronJobs()
{
    // Temp Cells Refresh
    if (millis() - lastTempRefresh > HOME_PAGE_TEMP_REFRESH_INTERVAL)
    {
        invalidation |= HOME_PAGE_INVALIDATION_TEMP_TABLE_CELLS;
        lastTempRefresh = millis();
    }
}