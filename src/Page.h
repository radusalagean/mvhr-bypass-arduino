#ifndef PAGE_H
#define PAGE_H

#include "Display.h"
#include "Opcode.h"
#include "State.h"
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
    Page(Display* display, Temperature* temperature, State* state);
    Display* display = NULL;
    Temperature* temperature = NULL;
    State* state = NULL;
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

/**
 * Invalidation flag bits:
 * Bit 1: Command area
 * Bit 2: Temp table cells
 * Bit 3: HR State cell
 */
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
    void clearCell(uint8_t count, ...);

public:
    HomePage(Display* display, Temperature* temperature, State* state);
    bool render();
    void refreshInvalidatedAreas();
    bool processOpcode(const uint8_t& opcode);
    void invalidateTempTableCells();
};

/**
 * TemperatureSettings page
 **/

#define TEMP_SETTINGS_PAGE_RANGE_LOW 10
#define TEMP_SETTINGS_PAGE_RANGE_HIGH 40
#define TEMP_SETTINGS_PAGE_RANGE (TEMP_SETTINGS_PAGE_RANGE_HIGH - TEMP_SETTINGS_PAGE_RANGE_LOW)
#define TEMP_SETTINGS_PAGE_RANGE_MIN_VALUES 3
#define TEMP_SETTINGS_PAGE_BAR_HEIGHT 12

#define EDIT_STATE_NONE 0
#define EDIT_STATE_INT_EV_MIN 1
#define EDIT_STATE_EXT_AD_MIN 2
#define EDIT_STATE_EXT_AD_MAX 3

#define TEMP_SETTINGS_PAGE_INT_EV_BAR_Y 20
#define TEMP_SETTINGS_PAGE_INT_EV_DIGITS_Y 33
#define TEMP_SETTINGS_PAGE_EXT_AD_LABEL_Y 58
#define TEMP_SETTINGS_PAGE_EXT_AD_BAR_Y 78
#define TEMP_SETTINGS_PAGE_EXT_AD_DIGITS_Y 91

/**
 * Invalidation flag bits:
 * Bit 1: Command area
 * Bit 2: INT EV
 * Bit 3: EXT AD
 */
#define TEMP_SETTINGS_PAGE_INVALIDATION_COMMAND_AREA 0b0001
#define TEMP_SETTINGS_PAGE_INVALIDATION_INT_EV       0b0010
#define TEMP_SETTINGS_PAGE_INVALIDATION_EXT_AD       0b0100

class TemperatureSettingsPage : public Page
{
private:
    uint8_t intEvMin;
    uint8_t extAdMin;
    uint8_t extAdMax;
    uint8_t editState = EDIT_STATE_NONE;
    void drawLabels();
    float getTempRangeRatio(const uint8_t& temp);
    void drawIntEvValue();
    void drawExtAdValue();
    void clearTempDigits(const uint8_t& y);
    void drawTempDigits(const uint8_t& temp, const uint16_t& color,
                        const uint8_t& x, const uint8_t& y,
                        const uint8_t& editState);
    void adjustTemperature(const int8_t& offset);
    void reloadTemperatures();
    void persistTemperatures();
    void enterEditState();
    void nextEditState();
    void leaveEditState();
    bool tempRangeCheck(const uint8_t& low, const uint8_t& high,
                        const uint8_t& requested);

public:
    TemperatureSettingsPage(Display* display, Temperature* temperature, State* state);
    bool render();
    void refreshInvalidatedAreas();
    bool processOpcode(const uint8_t& opcode);
};

#endif