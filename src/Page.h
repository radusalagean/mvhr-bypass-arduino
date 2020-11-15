#ifndef PAGE_H
#define PAGE_H

#include "Display.h"
#include "Opcode.h"
#include "StateController.h"
#include "Temperature.h"
#include <stdarg.h>

#define FONT_HEIGHT 16
#define COMMAND_AREA_HEIGHT (FONT_HEIGHT + 1)
#define PAGE_WIDTH DISPLAY_WIDTH
#define PAGE_HEIGHT (DISPLAY_HEIGHT - COMMAND_AREA_HEIGHT)
#define PAGE_MID_WIDTH (PAGE_WIDTH / 2)
#define PAGE_MID_HEIGHT (PAGE_HEIGHT / 2)

#define TABLE_ROWS 3
#define TABLE_COLUMNS 3
#define TABLE_CELL_WIDTH (PAGE_WIDTH / TABLE_COLUMNS)
#define TABLE_CELL_HEIGHT (PAGE_HEIGHT / TABLE_ROWS)
#define TABLE_CELL_WIDTH_HALF (TABLE_CELL_WIDTH / 2)
#define TABLE_CELL_HEIGHT_HALF (TABLE_CELL_HEIGHT / 2)

/**
 * 00 | 01 | 02
 * 10 | 11 | 12
 * 20 | 21 | 22
 */

struct Point2d
{
    uint8_t x;
    uint8_t y;
};

struct Cell
{
    uint8_t row;
    uint8_t col;
};

struct CommandAreaAction
{
    char* text;
    uint8_t opcode;
};

/**
 * BASE page
 **/

class Page
{
private:
protected:
    Page(Display* display, Temperature* temperature, StateController* stateController);
    Display* display = NULL;
    Temperature* temperature = NULL;
    StateController* stateController = NULL;
    byte invalidation = 0;
    boolean initialized = false;
    void drawCommandArea();

public:
    CommandAreaAction* leftAction = NULL;
    CommandAreaAction* rightAction = NULL;
    virtual bool render();
    virtual void refreshInvalidatedAreas();
    virtual bool processOpcode(const uint8_t& opcode);
};

/**
 * HOME page
 **/

#define HOME_PAGE_INVALIDATION_COMMAND_AREA     0b0001
#define HOME_PAGE_INVALIDATION_TEMP_TABLE_CELLS 0b0010
#define HOME_PAGE_INVALIDATION_HR_STATE_CELL    0b0100

class HomePage : public Page
{
private:
    Point2d cellOrigin[TABLE_ROWS][TABLE_COLUMNS] = {};
    Point2d cellCenter[TABLE_ROWS][TABLE_COLUMNS] = {};
    void initTablePoints();
    void drawHrState();
    void drawTempTable();
    void drawTempValues();
    void drawTempValue(const float& temp, const Point2d& centerPoint);
    void clearCell(uint8_t count, ...);

public:
    HomePage(Display* display, Temperature* temperature, StateController* stateController);
    bool render();
    void refreshInvalidatedAreas();
    bool processOpcode(const uint8_t& opcode);
    void invalidateTempTableCells();
};

/**
 * SETTINGS page
 **/

#define SETTINGS_PAGE_COLOR_HYST TFT_WHITE
#define SETTINGS_PAGE_COLOR_INT_EV TFT_ORANGE
#define SETTINGS_PAGE_COLOR_EXT_AD TFT_DARKCYAN

#define SETTINGS_PAGE_HYST_EDIT_STEP 0.1f
#define SETTINGS_PAGE_TEMP_EDIT_STEP 1
#define SETTINGS_PAGE_HYST_RANGE_LOW 0.f
#define SETTINGS_PAGE_HYST_RANGE_HIGH 1.f
#define SETTINGS_PAGE_RANGE_LOW 10
#define SETTINGS_PAGE_RANGE_HIGH 40
#define SETTINGS_PAGE_RANGE (SETTINGS_PAGE_RANGE_HIGH - SETTINGS_PAGE_RANGE_LOW)
#define SETTINGS_PAGE_RANGE_MIN_VALUES 3
#define SETTINGS_PAGE_BAR_HEIGHT 12

#define SETTINGS_PAGE_EDIT_STATE_NONE 0
#define SETTINGS_PAGE_EDIT_STATE_HYST 1
#define SETTINGS_PAGE_EDIT_STATE_INT_EV_MIN 2
#define SETTINGS_PAGE_EDIT_STATE_EXT_AD_MIN 3
#define SETTINGS_PAGE_EDIT_STATE_EXT_AD_MAX 4

#define SETTINGS_PAGE_INT_EV_BAR_Y 20
#define SETTINGS_PAGE_INT_EV_DIGITS_Y 33
#define SETTINGS_PAGE_EXT_AD_LABEL_Y 58
#define SETTINGS_PAGE_EXT_AD_BAR_Y 78
#define SETTINGS_PAGE_EXT_AD_DIGITS_Y 91

#define SETTINGS_PAGE_INVALIDATION_COMMAND_AREA    0b000001
#define SETTINGS_PAGE_INVALIDATION_HYST_DIGITS     0b000010
#define SETTINGS_PAGE_INVALIDATION_INT_EV_BAR      0b000100
#define SETTINGS_PAGE_INVALIDATION_INT_EV_DIGITS   0b001000
#define SETTINGS_PAGE_INVALIDATION_INT_EV          (SETTINGS_PAGE_INVALIDATION_INT_EV_BAR | SETTINGS_PAGE_INVALIDATION_INT_EV_DIGITS)
#define SETTINGS_PAGE_INVALIDATION_EXT_AD_BAR      0b010000
#define SETTINGS_PAGE_INVALIDATION_EXT_AD_DIGITS   0b100000
#define SETTINGS_PAGE_INVALIDATION_EXT_AD          (SETTINGS_PAGE_INVALIDATION_EXT_AD_BAR | SETTINGS_PAGE_INVALIDATION_EXT_AD_DIGITS)
#define SETTINGS_PAGE_INVALIDATION_CURRENT_TEMP    (SETTINGS_PAGE_INVALIDATION_INT_EV_BAR | SETTINGS_PAGE_INVALIDATION_EXT_AD_BAR)

typedef struct
{
    uint8_t xStart;
    uint8_t xEnd;
} BarXAxisConfig;

class SettingsPage : public Page
{
private:
    uint8_t intEvMin;
    uint8_t extAdMin;
    uint8_t extAdMax;
    float hysteresis;
    uint8_t editState = SETTINGS_PAGE_EDIT_STATE_NONE;
    void drawLabels();
    void drawHysteresisDigits();
    float getTempRangeRatio(const float& temp);
    void drawIntEv();
    uint8_t drawIntEvBar();
    void drawExtAd();
    BarXAxisConfig drawExtAdBar();
    void drawCurrentTempLine(const float& temp, const uint8_t& y);
    void drawHysteresisRect(const float& temp, const uint8_t& y);
    void clearTempDigits(const uint8_t& y);
    void drawTempDigits(const uint8_t& temp, const uint16_t& color,
                        const uint8_t& x, const uint8_t& y,
                        const uint8_t& editState);
    void adjustTemperature(const bool& increment);
    void reloadTemperatures();
    void persistTemperatures();
    void enterEditState();
    void nextEditState();
    void leaveEditState();
    bool tempRangeCheck(const uint8_t& low, const uint8_t& high,
                        const uint8_t& requested);
    bool tempRangeCheckHyst(const float& low, const float& high,
                            const float& requested);

public:
    SettingsPage(Display* display, Temperature* temperature, StateController* stateController);
    bool render();
    void refreshInvalidatedAreas();
    bool processOpcode(const uint8_t& opcode);
};

#endif