#include "Page.h"

const CommandAreaAction testAction = {"Back", OPCODE_OPEN_PAGE_HOME};

TemperatureSettingsPage::TemperatureSettingsPage(Display* display, Temperature* temperature, State* state) : Page::Page(display, temperature, state)
{
    leftAction = &testAction;
}

bool TemperatureSettingsPage::render()
{
    if (Page::render())
    {
        return true;
    }
    drawCommandArea();
    drawLabels();
    drawIntEvValue();
    drawExtAdValue();
    initialized = true;
    return true;
}

void TemperatureSettingsPage::refreshInvalidatedAreas()
{
    invalidation = 0;
}

bool TemperatureSettingsPage::processOpcode(const uint8_t& opcode)
{
    if (Page::processOpcode(opcode))
    {
        // OPCODE handled in the base class
        return true;
    }
    switch (opcode)
    {
    case 0:

        break;
    default:
        break;
    }
    return true;
}

void TemperatureSettingsPage::drawLabels()
{
    display->tft.drawStringWithDatum("INT EV.", DISPLAY_WIDTH, 0, 2, TR_DATUM);
    display->tft.drawStringWithDatum("EXT AD", 0, 58, 2, TL_DATUM);
}

float TemperatureSettingsPage::getTempRangeRatio(const uint8_t& temp)
{
    return (temp - TEMP_SETTINGS_PAGE_RANGE_LOW) / (float)TEMP_SETTINGS_PAGE_RANGE;
}

void TemperatureSettingsPage::drawIntEvValue()
{
    // bar
    uint8_t x = DISPLAY_WIDTH * getTempRangeRatio(state->intEvMin);
    uint8_t width = DISPLAY_WIDTH - x;
    display->tft.fillRect(x, 20, width, TEMP_SETTINGS_PAGE_BAR_HEIGHT, TFT_ORANGE);

    // value
    drawTempDigits(state->intEvMin, TFT_ORANGE, x, 33);
}

void TemperatureSettingsPage::drawExtAdValue()
{
    // bar
    uint8_t xStart = DISPLAY_WIDTH * getTempRangeRatio(state->extAdMin);
    uint8_t xEnd = DISPLAY_WIDTH * getTempRangeRatio(state->extAdMax);
    uint8_t width = xEnd - xStart;
    display->tft.fillRect(xStart, 78, width, TEMP_SETTINGS_PAGE_BAR_HEIGHT, TFT_CYAN);

    // values
    drawTempDigits(state->extAdMin, TFT_CYAN, xStart, 91);
    drawTempDigits(state->extAdMax, TFT_CYAN, xEnd, 91);
}

void TemperatureSettingsPage::drawTempDigits(const uint8_t& temp, const uint16_t& color,
                                             const uint8_t& x, const uint8_t& y)
{
    char valStr[3];
    snprintf(valStr, 3, "%d", temp);
    display->tft.setTextColor(color);
    display->tft.drawStringWithDatum(valStr, x, y, 2, TC_DATUM);
    display->tft.setTextColor(TFT_WHITE);
}