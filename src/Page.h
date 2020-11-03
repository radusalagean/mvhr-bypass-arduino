#ifndef PAGE_H
#define PAGE_H

#include "Display.h"
#include "Opcode.h"

#define FONT_HEIGHT 12
#define FONT_DESCENT 3
#define COMMAND_AREA_HEIGHT (FONT_HEIGHT + FONT_DESCENT + 1)

struct ControlAreaAction
{
    char* text;
    uint8_t opcode;
};

class Page
{
private:
    
protected:
    Page(Display* display);
    Display* display = NULL;
    bool invalidated = true;
    void drawControlArea();
public:
    ControlAreaAction* leftAction = NULL;
    ControlAreaAction* rightAction = NULL;
    virtual void render();
    void processControlAreaAction(ControlAreaAction& action);
    virtual bool processOpcode(const uint8_t& opcode);
};

class HomePage : public Page
{
private:
    bool hrEnabled = false;
    void drawHrState();
public:
    HomePage(Display* display);
    void render();
    bool processOpcode(const uint8_t& opcode);
};

class MenuPage : public Page
{
private:
    
public:

};

#endif