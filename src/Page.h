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
    bool invalidated = true;
    void drawControlArea(Display& display);
public:
    ControlAreaAction* leftAction = NULL;
    ControlAreaAction* rightAction = NULL;
    virtual void render(Display& display);
    void processControlAreaAction(ControlAreaAction& action);
    virtual bool processOpcode(uint8_t& opcode);
};

class HomePage : public Page
{
private:
    
public:
    HomePage();
    void render(Display& display);
    bool processOpcode(uint8_t& opcode);
};

class MenuPage : public Page
{
private:
    
public:

};

#endif