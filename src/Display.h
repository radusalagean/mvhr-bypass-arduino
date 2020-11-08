#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <SPI.h>
#include "vendor/TFT_ILI9163/TFT_ILI9163.h"

/*
  Hardware SPI Pins:
    Arduino Uno		sclk=13, data=11
*/

#define PIN_BACKLIGHT 9

#define DISPLAY_WIDTH 160
#define DISPLAY_HEIGHT 128
#define DISPLAY_MID_WIDTH (DISPLAY_WIDTH / 2)
#define DISPLAY_MID_HEIGHT (DISPLAY_HEIGHT / 2)

class Display 
{
private:
    void initBacklight();
    void powerDown();
    void powerUp();
public:
    TFT_ILI9163 tft = TFT_ILI9163();
    void init();
};

#endif