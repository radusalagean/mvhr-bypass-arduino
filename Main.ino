#include "src/Clock.h"
#include "src/Display.h"
#include "src/ExternalStorage.h"
#include "src/Keypad.h"
#include "src/Page.h"
#include "src/UserJourney.h"
#include "src/Relay.h"
// #include "src/Temperature.h"

#include <MemoryFree.h>

#define BAUD_RATE 115200

/**
 * PINs configuration (for Arduino Uno, R3):
 * IOREF -
 * RESET -
 * 3.3V -
 * 5V -
 * A0 - Keypad analog in
 * A1 -
 * A2 -
 * A3 -
 * A4 - RTC (Logging shield)
 * A5 - RTC (Logging shield)
 * D0 (RX, UART out) -
 * D1 (TX, UART in) -
 * D2 (Interrup compatible) - OneWire (Temperature sensors)
 * D3 (Interrup compatible, PWM) - Keypad interrupt
 * D4 - Display (RESET)
 * D5 (PWM) - Display (CD - A0)
 * D6 (PWM) - Display (CS)
 * D7 - Relay
 * D8 - 
 * D9 (PWM) - Display Backlight
 * D10 (SPI SS, PWM) - SD Card - CS (Logging shield)
 * D11 (SPI MOSI, PWM) - SD Card  (Logging shield) - and - Display (DATA - SCA)
 * D12 (SPI MISO) - SD Card (Logging shield)
 * D13 (SPI SCK) - SD Card (Logging shield) - and - Display (CLOCK - SCL)
 * AREF -
 * SDA (I2C Data) -
 * SCL (I2C Clock) -
 * */

// Clock clock;
Display display;
ExternalStorage externalStorage;
Keypad keypad;
Relay relay;
// Temperature temperature;
UserJourney userJourney = UserJourney(&display, &relay);

void setup(void)
{
    Serial.begin(BAUD_RATE);
    prepareInterrupts();
    // clock.init();
    display.init();
    relay.init();
    externalStorage.init();
    // temperature.init();
    userJourney.init();
}

void loop(void)
{
    uint8_t keyToProcess = keypad.getKeyToProcess();
    if (keyToProcess != KEY_NONE)
    {
        userJourney.processKey(keyToProcess);
        keypad.markAsProcessed();
    }
    userJourney.render();
    // externalStorage.printStorageInfo();
}

void printFreeMemory()
{
    Serial.print(F("Free memory: "));
    Serial.print(freeMemory());
    Serial.println(F(" bytes"));
}

void prepareInterrupts()
{
    attachInterrupt(digitalPinToInterrupt(KEYPAD_INTERRUPT_PIN), onKeypadInterrupt, RISING);
}

void onKeypadInterrupt()
{
    keypad.onInterrupt();
}