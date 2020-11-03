#include "Display.h"

void Display::init()
{
    initBacklight();
    ucg.begin(UCG_FONT_MODE_SOLID);
    ucg.clearScreen();
    ucg.setRotate90();
    ucg.setFont(ucg_font_courR12_mr);
}

void Display::initBacklight()
{
    pinMode(PIN_BACKLIGHT, OUTPUT);
    digitalWrite(PIN_BACKLIGHT, HIGH);
}

void Display::powerDown()
{
    digitalWrite(PIN_BACKLIGHT, LOW);
    ucg.powerDown();
}

void Display::powerUp()
{
    ucg.powerUp();
    delay(200);
    digitalWrite(PIN_BACKLIGHT, HIGH);
}

void Display::setColorRGB(uint8_t idx, uint8_t r, uint8_t g, uint8_t b)
{
    ucg.setColor(idx, b, g, r);
}

void Display::setColorRGB(uint8_t r, uint8_t g, uint8_t b)
{
    ucg.setColor(b, g, r);
}