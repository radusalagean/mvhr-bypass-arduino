#include "Page.h"

const CommandAreaAction homeAction = {"Home", OPCODE_OPEN_PAGE_HOME};
const CommandAreaAction editAction = {"Edit.", OPCODE_CONTEXTUAL_EDIT};
const CommandAreaAction cancelAction = {"Cancel", OPCODE_CONTEXTUAL_CANCEL};
const CommandAreaAction nextAction = {"Next.", OPCODE_CONTEXTUAL_NEXT};
const CommandAreaAction saveAction = {"Save.", OPCODE_CONTEXTUAL_SAVE};

TemperatureSettingsPage::TemperatureSettingsPage(Display* display, Temperature* temperature, State* state) : Page::Page(display, temperature, state)
{
    leftAction = &homeAction;
    rightAction = &editAction;
    reloadTemperatures();
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
    if ((invalidation & TEMP_SETTINGS_PAGE_INVALIDATION_COMMAND_AREA) != 0)
        drawCommandArea();
    if ((invalidation & TEMP_SETTINGS_PAGE_INVALIDATION_INT_EV) != 0)
        drawIntEvValue();
    if ((invalidation & TEMP_SETTINGS_PAGE_INVALIDATION_EXT_AD) != 0)
        drawExtAdValue();
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
    case OPCODE_CONTEXTUAL_MINUS:
    case OPCODE_CONTEXTUAL_PLUS:
        if (editState)
            adjustTemperature(opcode == OPCODE_CONTEXTUAL_MINUS ? -1 : +1);
        break;
    case OPCODE_CONTEXTUAL_EDIT:
        enterEditState();
        break;
    case OPCODE_CONTEXTUAL_NEXT:
        nextEditState();
        break;
    case OPCODE_CONTEXTUAL_CANCEL:
        leaveEditState();
        reloadTemperatures();
    case OPCODE_CONTEXTUAL_SAVE:
        leaveEditState();
        persistTemperatures();
        break;
    default:
        break;
    }
    return true;
}

void TemperatureSettingsPage::enterEditState()
{
    leftAction = &cancelAction;
    rightAction = &nextAction;
    invalidation |= TEMP_SETTINGS_PAGE_INVALIDATION_COMMAND_AREA;
    editState = EDIT_STATE_INT_EV_MIN;
    invalidation |= TEMP_SETTINGS_PAGE_INVALIDATION_INT_EV;
}

void TemperatureSettingsPage::nextEditState()
{
    editState++;
    if (editState == EDIT_STATE_EXT_AD_MAX)
    {
        rightAction = &saveAction;
        invalidation |= TEMP_SETTINGS_PAGE_INVALIDATION_COMMAND_AREA;
    }
    invalidation |= TEMP_SETTINGS_PAGE_INVALIDATION_INT_EV;
    invalidation |= TEMP_SETTINGS_PAGE_INVALIDATION_EXT_AD;
}

void TemperatureSettingsPage::leaveEditState()
{
    leftAction = &homeAction;
    rightAction = &editAction;
    invalidation |= TEMP_SETTINGS_PAGE_INVALIDATION_COMMAND_AREA;
    editState = EDIT_STATE_NONE;
    invalidation |= TEMP_SETTINGS_PAGE_INVALIDATION_INT_EV;
    invalidation |= TEMP_SETTINGS_PAGE_INVALIDATION_EXT_AD;
}

void TemperatureSettingsPage::adjustTemperature(const int8_t& offset)
{
    switch (editState)
    {
    case EDIT_STATE_INT_EV_MIN:
        intEvMin += offset;
        invalidation |= TEMP_SETTINGS_PAGE_INVALIDATION_INT_EV;
        break;
    case EDIT_STATE_EXT_AD_MIN:
        extAdMin += offset;
        invalidation |= TEMP_SETTINGS_PAGE_INVALIDATION_EXT_AD;
        break;
    case EDIT_STATE_EXT_AD_MAX:
        extAdMax += offset;
        invalidation |= TEMP_SETTINGS_PAGE_INVALIDATION_EXT_AD;
        break;
    default:
        break;
    }
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
    uint8_t x = DISPLAY_WIDTH * getTempRangeRatio(intEvMin);
    uint8_t width = DISPLAY_WIDTH - x;
    if (x  > 0)
        display->tft.fillRect(0, TEMP_SETTINGS_PAGE_INT_EV_BAR_Y, 
                              x - 1, TEMP_SETTINGS_PAGE_BAR_HEIGHT, 
                              TFT_BLACK);
    display->tft.fillRect(x, TEMP_SETTINGS_PAGE_INT_EV_BAR_Y, 
                          width, TEMP_SETTINGS_PAGE_BAR_HEIGHT, 
                          TFT_ORANGE);

    // value
    clearTempDigits(TEMP_SETTINGS_PAGE_INT_EV_DIGITS_Y);
    drawTempDigits(intEvMin, TFT_ORANGE, 
                   x, TEMP_SETTINGS_PAGE_INT_EV_DIGITS_Y, 
                   EDIT_STATE_INT_EV_MIN);
}

void TemperatureSettingsPage::drawExtAdValue()
{
    // bar
    uint8_t xStart = DISPLAY_WIDTH * getTempRangeRatio(extAdMin);
    uint8_t xEnd = DISPLAY_WIDTH * getTempRangeRatio(extAdMax);
    uint8_t width = xEnd - xStart;
    if (xStart > 0)
        display->tft.fillRect(0, TEMP_SETTINGS_PAGE_EXT_AD_BAR_Y, 
                              xStart - 1, TEMP_SETTINGS_PAGE_BAR_HEIGHT, 
                              TFT_BLACK);
    display->tft.fillRect(xStart, TEMP_SETTINGS_PAGE_EXT_AD_BAR_Y, 
                          width, TEMP_SETTINGS_PAGE_BAR_HEIGHT, 
                          TFT_CYAN);
    if (xEnd < DISPLAY_WIDTH)
        display->tft.fillRect(xEnd, TEMP_SETTINGS_PAGE_EXT_AD_BAR_Y, 
                              DISPLAY_WIDTH - xEnd, TEMP_SETTINGS_PAGE_BAR_HEIGHT, 
                              TFT_BLACK);

    // values
    clearTempDigits(TEMP_SETTINGS_PAGE_EXT_AD_DIGITS_Y);
    drawTempDigits(extAdMin, TFT_CYAN, 
                   xStart, TEMP_SETTINGS_PAGE_EXT_AD_DIGITS_Y, 
                   EDIT_STATE_EXT_AD_MIN);
    drawTempDigits(extAdMax, TFT_CYAN, 
                   xEnd, TEMP_SETTINGS_PAGE_EXT_AD_DIGITS_Y, 
                   EDIT_STATE_EXT_AD_MAX);
}

void TemperatureSettingsPage::clearTempDigits(const uint8_t& y)
{
    display->tft.fillRect(0, y, DISPLAY_WIDTH, FONT_HEIGHT, TFT_BLACK);
}

void TemperatureSettingsPage::drawTempDigits(const uint8_t& temp, const uint16_t& color,
                                             const uint8_t& x, const uint8_t& y,
                                             const uint8_t& editState)
{
    char valStr[3];
    snprintf(valStr, 3, "%d", temp);
    display->tft.setTextColor(this->editState == editState ? TFT_BLACK : color, 
                              this->editState == editState ? TFT_WHITE : TFT_BLACK);
    display->tft.drawStringWithDatum(valStr, x, y, 2, TC_DATUM);
    display->tft.setTextColor(TFT_WHITE);
}

void TemperatureSettingsPage::reloadTemperatures()
{
    intEvMin = state->intEvMin;
    extAdMin = state->extAdMin;
    extAdMax = state->extAdMax;
}

void TemperatureSettingsPage::persistTemperatures()
{
    state->intEvMin = intEvMin;
    state->extAdMin = extAdMin;
    state->extAdMax = extAdMax;
}