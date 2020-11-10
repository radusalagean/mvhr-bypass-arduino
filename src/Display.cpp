#include "Display.h"

void Display::init()
{
    initBacklight();
    tft.init();
    tft.setRotation(3);
    tft.setTextDatum(BL_DATUM);
    tft.setTextColor(TFT_WHITE);
}

void Display::initBacklight()
{
    pinMode(PIN_BACKLIGHT, OUTPUT);
    digitalWrite(PIN_BACKLIGHT, HIGH);
}

void Display::powerDown()
{
    digitalWrite(PIN_BACKLIGHT, LOW);
    // TODO Implement Power Down in library
}

void Display::powerUp()
{
    // TODO Implement Power Up in library
    delay(200);
    digitalWrite(PIN_BACKLIGHT, HIGH);
}

void Display::clearScreen()
{
    tft.fillScreen(TFT_BLACK);
}