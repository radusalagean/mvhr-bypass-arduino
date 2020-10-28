#include "Clock.h"
#include "Display.h"
#include "ExternalStorage.h"

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
 * D4 - Display (RESET)
 * D5 (PWM) - Display (CD - A0)
 * D6 (PWM) - Display (CS)
 * D7 - Display (DATA - SCA)
 * D8 - Display (CLOCK - SCL)
 * D9 (PWM) - Display Backlight
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
ExternalStorage externalStorage;

void setup(void)
{
    Serial.begin(BAUD_RATE);
    clock.init();
    display.init();
    externalStorage.init();
    Serial.println(sizeof(clock));
    Serial.println(sizeof(display));
    Serial.println(sizeof(externalStorage));
    Serial.println();
}

void loop(void)
{
    Serial.println(millis());
    DateTime now = clock.now();
    Serial.println(now.unixtime());
    display.render();
    // externalStorage.printStorageInfo();
}