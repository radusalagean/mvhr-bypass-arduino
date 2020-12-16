#include "Page.h"

const CommandAreaAction homeAction = {"HOME", OPCODE_OPEN_PAGE_HOME};
const CommandAreaAction editAction = {"EDIT", OPCODE_CONTEXTUAL_EDIT};
const CommandAreaAction cancelAction = {"CANCEL", OPCODE_CONTEXTUAL_CANCEL};
const CommandAreaAction nextAction = {"NEXT", OPCODE_CONTEXTUAL_NEXT};
const CommandAreaAction saveAction = {"SAVE", OPCODE_CONTEXTUAL_SAVE};

SettingsPage::SettingsPage(Display* display, Temperature* temperature, StateController* stateController) : Page::Page(display, temperature, stateController)
{
    leftAction = &homeAction;
    rightAction = &editAction;
    reloadTemperatures();
}

bool SettingsPage::render()
{
    if (Page::render())
    {
        return true;
    }
    drawCommandArea();
    drawLabels();
    drawHysteresisDigits();
    drawIntEv();
    drawExtAd();
    initialized = true;
    return true;
}

void SettingsPage::refreshInvalidatedAreas()
{
    if ((invalidation & SETTINGS_PAGE_INVALIDATION_COMMAND_AREA) != 0)
    {
        drawCommandArea();
        invalidation ^= SETTINGS_PAGE_INVALIDATION_COMMAND_AREA;
    }
    if ((invalidation & SETTINGS_PAGE_INVALIDATION_HYST_DIGITS) != 0)
    {
        drawHysteresisDigits();
        invalidation ^= SETTINGS_PAGE_INVALIDATION_HYST_DIGITS;
    }
    if ((invalidation & SETTINGS_PAGE_INVALIDATION_INT_EV) == SETTINGS_PAGE_INVALIDATION_INT_EV)
    {
        drawIntEv();
        invalidation ^= SETTINGS_PAGE_INVALIDATION_INT_EV;
    }
    if ((invalidation & SETTINGS_PAGE_INVALIDATION_EXT_AD) == SETTINGS_PAGE_INVALIDATION_EXT_AD)
    {
        drawExtAd();
        invalidation ^= SETTINGS_PAGE_INVALIDATION_EXT_AD;
    }
    if ((invalidation & SETTINGS_PAGE_INVALIDATION_CURRENT_TEMP) != 0)
    {
        drawIntEvBar();
        drawExtAdBar();
        invalidation ^= SETTINGS_PAGE_INVALIDATION_CURRENT_TEMP;
    }
}

bool SettingsPage::processOpcode(const uint8_t& opcode)
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
            adjustTemperature(opcode == OPCODE_CONTEXTUAL_PLUS);
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
    case OPCODE_REFRESH_TEMP_VALUES_ON_SCREEN:
        invalidation |= SETTINGS_PAGE_INVALIDATION_CURRENT_TEMP;
        break;
    case OPCODE_APPLY_STATE_TEMPERATURES:
        invalidateTemperatureAreas();
        if (editState == SETTINGS_PAGE_EDIT_STATE_NONE)
            reloadTemperatures();
        break;
    default:
        break;
    }
    return true;
}

void SettingsPage::enterEditState()
{
    leftAction = &cancelAction;
    rightAction = &nextAction;
    invalidation |= SETTINGS_PAGE_INVALIDATION_COMMAND_AREA;
    editState = SETTINGS_PAGE_EDIT_STATE_HYST;
    invalidation |= SETTINGS_PAGE_INVALIDATION_HYST_DIGITS;
}

void SettingsPage::nextEditState()
{
    editState++;
    if (editState == SETTINGS_PAGE_EDIT_STATE_EXT_AD_MAX)
    {
        rightAction = &saveAction;
        invalidation |= SETTINGS_PAGE_INVALIDATION_COMMAND_AREA;
    }
    invalidation |= SETTINGS_PAGE_INVALIDATION_HYST_DIGITS;
    invalidation |= SETTINGS_PAGE_INVALIDATION_INT_EV;
    invalidation |= SETTINGS_PAGE_INVALIDATION_EXT_AD;
}

void SettingsPage::leaveEditState()
{
    leftAction = &homeAction;
    rightAction = &editAction;
    invalidation |= SETTINGS_PAGE_INVALIDATION_COMMAND_AREA;
    editState = SETTINGS_PAGE_EDIT_STATE_NONE;
    invalidateTemperatureAreas();
}

void SettingsPage::invalidateTemperatureAreas()
{
    invalidation |= SETTINGS_PAGE_INVALIDATION_HYST_DIGITS;
    invalidation |= SETTINGS_PAGE_INVALIDATION_INT_EV;
    invalidation |= SETTINGS_PAGE_INVALIDATION_EXT_AD;
}

void SettingsPage::adjustTemperature(const bool& increment)
{
    float hystOffset = increment ? SETTINGS_PAGE_HYST_EDIT_STEP : -SETTINGS_PAGE_HYST_EDIT_STEP;
    uint8_t offset = increment ? SETTINGS_PAGE_TEMP_EDIT_STEP : -SETTINGS_PAGE_TEMP_EDIT_STEP;
    switch (editState)
    {
    case SETTINGS_PAGE_EDIT_STATE_HYST:
        if (tempRangeCheckHyst(SETTINGS_PAGE_HYST_RANGE_LOW, SETTINGS_PAGE_HYST_RANGE_HIGH, (hysteresis + hystOffset)))
        {
            hysteresis += hystOffset;
            invalidation |= SETTINGS_PAGE_INVALIDATION_HYST_DIGITS;
            invalidation |= SETTINGS_PAGE_INVALIDATION_INT_EV_BAR;
            invalidation |= SETTINGS_PAGE_INVALIDATION_EXT_AD_BAR;
        }
        break;
    case SETTINGS_PAGE_EDIT_STATE_INT_EV_MIN:
        if (tempRangeCheck(SETTINGS_PAGE_RANGE_LOW, SETTINGS_PAGE_RANGE_HIGH, intEvMin + offset))
        {
            intEvMin += offset;
            invalidation |= SETTINGS_PAGE_INVALIDATION_INT_EV;
        }
        break;
    case SETTINGS_PAGE_EDIT_STATE_EXT_AD_MIN:
        if (tempRangeCheck(SETTINGS_PAGE_RANGE_LOW, extAdMax - SETTINGS_PAGE_RANGE_MIN_VALUES, extAdMin + offset))
        {
            extAdMin += offset;
            invalidation |= SETTINGS_PAGE_INVALIDATION_EXT_AD;
        }
        break;
    case SETTINGS_PAGE_EDIT_STATE_EXT_AD_MAX:
        if (tempRangeCheck(extAdMin + SETTINGS_PAGE_RANGE_MIN_VALUES, SETTINGS_PAGE_RANGE_HIGH, extAdMax + offset))
        {
            extAdMax += offset;
            invalidation |= SETTINGS_PAGE_INVALIDATION_EXT_AD;
        }
        break;
    default:
        break;
    }
}

bool SettingsPage::tempRangeCheck(const uint8_t& low, const uint8_t& high,
                                             const uint8_t& requested)
{
    return low <= requested && requested <= high;
}

bool SettingsPage::tempRangeCheckHyst(const float& low, const float& high,
                                                 const float& requested)
{
    return low <= requested && requested <= high + SETTINGS_PAGE_HYST_EDIT_STEP;
}

void SettingsPage::drawLabels()
{
    display->tft.drawStringWithDatum("HYST +", 0, 0, 2, TL_DATUM);
    display->tft.drawStringWithDatum("INT EV", DISPLAY_WIDTH, 0, 2, TR_DATUM);
    display->tft.drawStringWithDatum("EXT AD", 0, SETTINGS_PAGE_EXT_AD_LABEL_Y, 2, TL_DATUM);
}

void SettingsPage::drawHysteresisDigits()
{
    bool inEditState = this->editState == SETTINGS_PAGE_EDIT_STATE_HYST;
    display->tft.setTextColor(inEditState ? TFT_BLACK : SETTINGS_PAGE_COLOR_HYST, 
                              inEditState ? TFT_WHITE : TFT_BLACK);
    uint8_t x = display->tft.textWidth("HYST +", 2);
    display->tft.drawFloatWithDatum(hysteresis, 1, x, 0, 2, TL_DATUM);
    display->resetTextColor();
}

float SettingsPage::getTempRangeRatio(const float& temp)
{
    return (temp - SETTINGS_PAGE_RANGE_LOW) / (float)SETTINGS_PAGE_RANGE;
}

void SettingsPage::drawIntEv()
{
    uint8_t x = drawIntEvBar();

    clearTempDigits(SETTINGS_PAGE_INT_EV_DIGITS_Y);
    drawTempDigits(intEvMin, SETTINGS_PAGE_COLOR_INT_EV, 
                   x, SETTINGS_PAGE_INT_EV_DIGITS_Y, 
                   SETTINGS_PAGE_EDIT_STATE_INT_EV_MIN);
}

uint8_t SettingsPage::drawIntEvBar()
{
    // bar
    uint8_t x = DISPLAY_WIDTH * getTempRangeRatio(intEvMin);
    uint8_t width = DISPLAY_WIDTH - x;
    if (x  > 0)
        display->tft.fillRect(0, SETTINGS_PAGE_INT_EV_BAR_Y, 
                              x, SETTINGS_PAGE_BAR_HEIGHT, 
                              TFT_BLACK);
    display->tft.fillRect(x, SETTINGS_PAGE_INT_EV_BAR_Y, 
                          width, SETTINGS_PAGE_BAR_HEIGHT, 
                          SETTINGS_PAGE_COLOR_INT_EV);

    // hysteresis area
    drawHysteresisRect(intEvMin, SETTINGS_PAGE_INT_EV_BAR_Y);

    // current temp line
    drawCurrentTempLine(temperature->getTempIntEv(), SETTINGS_PAGE_INT_EV_BAR_Y);

    return x;
}

void SettingsPage::drawExtAd()
{
    BarXAxisConfig bar = drawExtAdBar();
    clearTempDigits(SETTINGS_PAGE_EXT_AD_DIGITS_Y);
    uint8_t width = display->tft.textWidth("88", 2);
    uint8_t halfWidth = (float)width / 2;

    uint8_t extAdMinX = bar.xStart;
    if (extAdMinX + halfWidth >= DISPLAY_WIDTH - width)
    {
        extAdMinX = DISPLAY_WIDTH - width - halfWidth;
    }
    drawTempDigits(extAdMin, SETTINGS_PAGE_COLOR_EXT_AD, 
                   extAdMinX, SETTINGS_PAGE_EXT_AD_DIGITS_Y, 
                   SETTINGS_PAGE_EDIT_STATE_EXT_AD_MIN);
    
    uint8_t extAdMaxX = bar.xEnd;
    if (extAdMaxX - halfWidth <= width)
    {
        extAdMaxX = width + halfWidth;
    }
    drawTempDigits(extAdMax, SETTINGS_PAGE_COLOR_EXT_AD, 
                   extAdMaxX, SETTINGS_PAGE_EXT_AD_DIGITS_Y, 
                   SETTINGS_PAGE_EDIT_STATE_EXT_AD_MAX);
}

BarXAxisConfig SettingsPage::drawExtAdBar()
{
    // bar
    uint8_t xStart = DISPLAY_WIDTH * getTempRangeRatio(extAdMin);
    uint8_t xEnd = DISPLAY_WIDTH * getTempRangeRatio(extAdMax);
    uint8_t width = xEnd - xStart;
    if (xStart > 0)
        display->tft.fillRect(0, SETTINGS_PAGE_EXT_AD_BAR_Y, 
                              xStart, SETTINGS_PAGE_BAR_HEIGHT, 
                              TFT_BLACK);
    display->tft.fillRect(xStart, SETTINGS_PAGE_EXT_AD_BAR_Y, 
                          width, SETTINGS_PAGE_BAR_HEIGHT, 
                          SETTINGS_PAGE_COLOR_EXT_AD);
    if (xEnd < DISPLAY_WIDTH)
        display->tft.fillRect(xEnd, SETTINGS_PAGE_EXT_AD_BAR_Y, 
                              DISPLAY_WIDTH - xEnd, SETTINGS_PAGE_BAR_HEIGHT, 
                              TFT_BLACK);

    // hysteresis area
    drawHysteresisRect(extAdMin, SETTINGS_PAGE_EXT_AD_BAR_Y);
    drawHysteresisRect(extAdMax, SETTINGS_PAGE_EXT_AD_BAR_Y);

    // current temp line
    drawCurrentTempLine(temperature->getTempExtAd(), SETTINGS_PAGE_EXT_AD_BAR_Y);

    return BarXAxisConfig {xStart, xEnd};
}

void SettingsPage::drawCurrentTempLine(const float& temp, const uint8_t& y)
{
    if (temp >= SETTINGS_PAGE_RANGE_LOW && 
        temp <= SETTINGS_PAGE_RANGE_HIGH)
    {
        uint8_t currentTempX = DISPLAY_WIDTH * getTempRangeRatio(temp);
        display->tft.fillRect(currentTempX, y,
                              2, SETTINGS_PAGE_BAR_HEIGHT,
                              TFT_RED);
    }
}

void SettingsPage::drawHysteresisRect(const float& temp, const uint8_t& y)
{
    if (hysteresis >= 0.1f)
    {
        float hysteresisStart = temp - hysteresis;
        float hysteresisEnd = temp + hysteresis;
        uint8_t x1 = DISPLAY_WIDTH * getTempRangeRatio(hysteresisStart);
        uint8_t x2 = DISPLAY_WIDTH * getTempRangeRatio(hysteresisEnd);
        display->tft.drawRect(x1, y,
                              x2 - x1, SETTINGS_PAGE_BAR_HEIGHT, 
                              SETTINGS_PAGE_COLOR_HYST);
    }
}

void SettingsPage::clearTempDigits(const uint8_t& y)
{
    display->tft.fillRect(0, y, DISPLAY_WIDTH, FONT_HEIGHT, TFT_BLACK);
}

void SettingsPage::drawTempDigits(const uint8_t& temp, const uint16_t& color,
                                             const uint8_t& x, const uint8_t& y,
                                             const uint8_t& editState)
{
    char valStr[3];
    snprintf(valStr, 3, "%d", temp);
    display->tft.setTextColor(this->editState == editState ? TFT_BLACK : color, 
                              this->editState == editState ? TFT_WHITE : TFT_BLACK);
    display->tft.drawStringWithDatum(valStr, x, y, 2, TC_DATUM);
    display->resetTextColor();
}

void SettingsPage::reloadTemperatures()
{
    hysteresis = stateController->getHysteresis();
    intEvMin = stateController->getIntEvMin();
    extAdMin = stateController->getExtAdMin();
    extAdMax = stateController->getExtAdMax();
}

void SettingsPage::persistTemperatures()
{
    stateController->setTemperatures(hysteresis, intEvMin, extAdMin, extAdMax);
}