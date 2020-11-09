#include "Page.h"

/**
 * HOME page
 **/

const CommandAreaAction switchModeAction = { "Switch mode", OPCODE_SWITCH_MODE };

HomePage::HomePage(Display* display, Temperature* temperature, State* state) : Page::Page(display, temperature, state)
{
    leftAction = &switchModeAction;
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
    display->tft.fillRect(cellOrigin[0][0].x, cellOrigin[0][0].y, 
            TABLE_CELL_WIDTH, TABLE_CELL_HEIGHT, state->hrDisabled ? TFT_BLUE : TFT_RED);
    display->tft.drawStringWithDatum(state->hrModeAuto ? "auto" : "manual", 
        cellCenter[0][0].x, cellCenter[0][0].y, 2, CC_DATUM);
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
    clearCell(4, Cell {1, 1}, Cell {1, 2}, Cell {2, 1}, Cell {2, 2});

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
    case OPCODE_HR_OFF:
    case OPCODE_SWITCH_MODE:
        invalidation |= HOME_PAGE_INVALIDATION_HR_STATE_CELL;
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

void HomePage::clearCell(uint8_t count, ...)
{
    va_list args;
    va_start(args, count);
    for (int i = 0; i < count; i++)
    {
        Cell cell = va_arg(args, Cell);
        display->tft.fillRectExclusive(cellOrigin[cell.row][cell.col].x, cellOrigin[cell.row][cell.col].y, 
            TABLE_CELL_WIDTH, TABLE_CELL_HEIGHT, TFT_BLACK);
    }
    va_end(args);
}