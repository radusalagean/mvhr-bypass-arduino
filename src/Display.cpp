#include "Display.h"

void Display::init()
{
    initBacklight();
    ucg.begin(UCG_FONT_MODE_SOLID);
    ucg.clearScreen();
    ucg.setRotate90();
}

void Display::initBacklight()
{
    pinMode(PIN_BACKLIGHT, OUTPUT);
    digitalWrite(PIN_BACKLIGHT, HIGH);
}

void Display::render()
{
    uint8_t i;
    ucg.setFont(ucg_font_fub20_hr);
    ucg.setColor(255, 255, 255);
    ucg.setPrintPos(0, 30);
    ucg.print("Power");
    ucg.setPrintPos(0, 60);
    ucg.print("Off");

    for (i = 5; i > 0; i--)
    {
        ucg.setPrintPos(0, 90);
        ucg.setColor(0, 255, 0);
        ucg.print(i);
        delay(1000);
    }
    powerDown();
    ucg.setPrintPos(0, 90);
    ucg.print("fail?");
    delay(2000);
    ucg.clearScreen();
    powerUp();
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