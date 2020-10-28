#include "Clock.h"
#include "Display.h"

#define BAUD_RATE 9600

/**
 * PINs configuration (for Arduino Uno, R3):
 * IOREF -
 * RESET -
 * 3.3V -
 * 5V -
 * A0 -
 * A1 -
 * A2 -
 * A3 -
 * A4 - RTC (Logging shield)
 * A5 - RTC (Logging shield)
 * D0 (RX, UART out) -
 * D1 (TX, UART in) -
 * D2 (Interrup compatible) -
 * D3 (Interrup compatible, PWM) -
 * D4 -
 * D5 (PWM) -
 * D6 (PWM) -
 * D7 -
 * D8 -
 * D9 (PWM) -
 * D10 (SPI SS, PWM) - SD Card (Logging shield)
 * D11 (SPI MOSI, PWM) - SD Card (Logging shield)
 * D12 (SPI MISO) - SD Card (Logging shield)
 * D13 (SPI SCK) - SD Card (Logging shield)
 * AREF -
 * SDA (I2C Data) -
 * SCL (I2C Clock) -
 * */

Clock clock;
Display display;

void setup(void)
{
    Serial.begin(BAUD_RATE);
    clock.init();
    display.init();
    Serial.println("Setup done");
}

void loop(void)
{
    Serial.println(millis());
    DateTime now = clock.now();
    Serial.println(now.unixtime());
    display.render();
}