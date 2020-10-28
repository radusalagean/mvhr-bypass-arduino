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

class Display 
{
private:
    Ucglib_ST7735_18x128x160_SWSPI ucg = Ucglib_ST7735_18x128x160_SWSPI(/*sclk=*/ 8, /*data=*/ 7, /*cd=*/ 5, /*cs=*/ 6, /*reset=*/ 4);
    // Ucglib_ST7735_18x128x160_HWSPI ucg = Ucglib_ST7735_18x128x160_HWSPI(/*cd=*/ 5, /*cs=*/ 6, /*reset=*/ 4);
    void initBacklight();
    void powerDown();
    void powerUp();
public:
    void init();
    void render();
};

#endif