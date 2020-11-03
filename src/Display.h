#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <SPI.h>
#include <Ucglib.h>

/*
  Hardware SPI Pins:
    Arduino Uno		sclk=13, data=11
*/

#define PIN_BACKLIGHT 9

#define DISPLAY_WIDTH 160
#define DISPLAY_HEIGHT 128

class Display 
{
private:
    void initBacklight();
    void powerDown();
    void powerUp();
public:
    // Ucglib_ST7735_18x128x160_SWSPI ucg = Ucglib_ST7735_18x128x160_SWSPI(/*sclk=*/ 13, /*data=*/ 11, /*cd=*/ 5, /*cs=*/ 6, /*reset=*/ 4);
    Ucglib_ST7735_18x128x160_HWSPI ucg = Ucglib_ST7735_18x128x160_HWSPI(/*cd=*/ 5, /*cs=*/ 6, /*reset=*/ 4);
    void init();
    // The colors on my display are swapped, it interprets R as B and B as R, 
    //  so I added helper methods here to account for that
    void setColorRGB(uint8_t idx, uint8_t r, uint8_t g, uint8_t b);
    void setColorRGB(uint8_t r, uint8_t g, uint8_t b);
};

#endif