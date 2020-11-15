#include "Page.h"

/**
 * HOME page
 **/

const CommandAreaAction switchModeAction = {"MODE", OPCODE_SWITCH_MODE};
const CommandAreaAction tempSettingsAction = {"SETTINGS", OPCODE_OPEN_PAGE_TEMPERATURE_SETTINGS};

HomePage::HomePage(Display* display, Temperature* temperature, StateController* stateController) : Page::Page(display, temperature, stateController)
{
    leftAction = &switchModeAction;
    rightAction = &tempSettingsAction;
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
                row * TABLE_CELL_HEIGHT};
            cellCenter[row][col] = {
                cellOrigin[row][col].x + TABLE_CELL_WIDTH_HALF,
                cellOrigin[row][col].y + TABLE_CELL_HEIGHT_HALF};
        }
    }
}

bool HomePage::render()
{
    if (Page::render())
    {
        return true;
    }
    drawCommandArea();
    drawTempTable();
    drawHrState();
    drawTempValues();
    initialized = true;
    return true;
}

void HomePage::refreshInvalidatedAreas()
{
    if ((invalidation & HOME_PAGE_INVALIDATION_COMMAND_AREA) != 0)
    {
        drawCommandArea();
        invalidation ^= HOME_PAGE_INVALIDATION_COMMAND_AREA;
    }
    if ((invalidation & HOME_PAGE_INVALIDATION_TEMP_TABLE_CELLS) != 0)
    {
        drawTempValues();
        invalidation ^= HOME_PAGE_INVALIDATION_TEMP_TABLE_CELLS;
    }
    if ((invalidation & HOME_PAGE_INVALIDATION_HR_STATE_CELL) != 0)
    {
        drawHrState();
        invalidation ^= HOME_PAGE_INVALIDATION_HR_STATE_CELL;
    }
}

void HomePage::drawHrState()
{
    display->tft.fillRect(cellOrigin[0][0].x, cellOrigin[0][0].y,
                          TABLE_CELL_WIDTH, TABLE_CELL_HEIGHT, stateController->isHrDisabled() ? TFT_BLUE : TFT_RED);
    display->tft.setTextColor(TFT_WHITE);
    display->tft.drawStringWithDatum(stateController->isHrModeAuto() ? "AUTO" : "MANUAL",
                                     cellCenter[0][0].x, cellCenter[0][0].y, 2, CC_DATUM);
    display->resetTextColor();
}

void HomePage::drawTempTable()
{
    // Draw row separators
    for (uint8_t row = 1; row < TABLE_ROWS; row++)
    {
        display->tft.drawLine(0, cellOrigin[row][0].y,
                              PAGE_WIDTH - 1, cellOrigin[row][0].y, TFT_WHITE);
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

void HomePage::drawTempValues()
{
    // We use font with bg color and padding, in this case we don't need to clear the cell
    // Kept in case I'll use transparent bg color / no padding in the future, or for reference purposes:
    // clearCell(4, Cell{1, 1}, Cell{1, 2}, Cell{2, 1}, Cell{2, 2}); 
    display->tft.setTextPadding(TABLE_CELL_WIDTH - 1);
    drawTempValue(temperature->getTempExtAd(), cellCenter[1][1]);
    drawTempValue(temperature->getTempExtEv(), cellCenter[2][1]);
    drawTempValue(temperature->getTempIntAd(), cellCenter[1][2]);
    drawTempValue(temperature->getTempIntEv(), cellCenter[2][2]);
    display->tft.setTextPadding(0);
}

void HomePage::drawTempValue(const float& temp, const Point2d& centerPoint)
{
    
    if (temp == TEMP_READ_ERROR)
    {
        display->tft.setTextColor(TFT_RED, TFT_BLACK);
        display->tft.drawStringWithDatum("ERR", centerPoint.x, centerPoint.y, 4, CC_DATUM);
        display->resetTextColor();
        return;
    }
    char strVal[6];
    display->tft.floatToString(temp, temp <= -10.f ? 0 : 1, strVal, 6);
    display->tft.drawStringWithDatum(strVal, centerPoint.x, centerPoint.y, 4, CC_DATUM);
    display->resetTextColor();
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
    case OPCODE_REFRESH_TEMP_VALUES_ON_SCREEN:
        invalidation |= HOME_PAGE_INVALIDATION_TEMP_TABLE_CELLS;
        break;
    default:
        break;
    }
    return true;
}

// void HomePage::clearCell(uint8_t count, ...)
// {
//     va_list args;
//     va_start(args, count);
//     for (int i = 0; i < count; i++)
//     {
//         Cell cell = va_arg(args, Cell);
//         display->tft.fillRectExclusive(cellOrigin[cell.row][cell.col].x, cellOrigin[cell.row][cell.col].y,
//                                        TABLE_CELL_WIDTH, TABLE_CELL_HEIGHT, TFT_BLACK);
//     }
//     va_end(args);
// }