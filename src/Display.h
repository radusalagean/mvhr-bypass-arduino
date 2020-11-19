#ifndef DISPLAY_H
#define DISPLAY_H

#include "vendor/TFT_ILI9163/TFT_ILI9163.h"
#include <Arduino.h>
#include <SPI.h>

/*
  Hardware SPI Pins:
    Arduino Uno		sclk=13, data=11
*/

#define PIN_BACKLIGHT 10

#define DISPLAY_WIDTH 160
#define DISPLAY_HEIGHT 128
#define DISPLAY_MID_WIDTH (DISPLAY_WIDTH / 2)
#define DISPLAY_MID_HEIGHT (DISPLAY_HEIGHT / 2)

class Display
{
private:
    bool poweredUp = true;
    void initBacklight();

public:
    TFT_ILI9163 tft = TFT_ILI9163();
    void init();
    void clearScreen();
    void resetTextColor();
    void powerDown();
    void powerUp();
    inline bool isPoweredUp() __attribute__((always_inline));
};

bool Display::isPoweredUp()
{
    return poweredUp;
}

#endif