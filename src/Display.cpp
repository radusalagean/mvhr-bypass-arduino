#include "Display.h"

void Display::init()
{
    initBacklight();
    tft.init();
    tft.setRotation(3);
    tft.setTextDatum(BL_DATUM);
    resetTextColor();
}

void Display::initBacklight()
{
    pinMode(PIN_BACKLIGHT, OUTPUT);
    digitalWrite(PIN_BACKLIGHT, HIGH);
}

void Display::clearScreen()
{
    tft.fillScreen(TFT_BLACK);
}

void Display::resetTextColor()
{
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
}

void Display::powerDown()
{
    digitalWrite(PIN_BACKLIGHT, LOW);
    tft.powerDown();
    poweredUp = false;
}

void Display::powerUp()
{
    tft.powerUp();
    delay(200);
    digitalWrite(PIN_BACKLIGHT, HIGH);
    poweredUp = true;
}
