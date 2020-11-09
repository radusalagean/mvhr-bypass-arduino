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
    virtual void render();
    virtual void refreshInvalidatedAreas();
    void processCommandAreaAction(CommandAreaAction& action);
    virtual bool processOpcode(const uint8_t& opcode);
};

/**
 * Invalidation flag bits:
 * Bit 1: Command area
 * Bit 2: Temp table cells
 * Bit 3: HR State cell
 */
#define HOME_PAGE_INVALIDATION_COMMAND_AREA 0b0001
#define HOME_PAGE_INVALIDATION_TEMP_TABLE_CELLS 0b0010
#define HOME_PAGE_INVALIDATION_HR_STATE_CELL 0b0100



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
    void render();
    void refreshInvalidatedAreas();
    bool processOpcode(const uint8_t& opcode);
    void invalidateTempTableCells();
};

class MenuPage : public Page
{
private:
public:
};

#endif