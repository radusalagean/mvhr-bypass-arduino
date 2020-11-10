#include "src/Clock.h"
#include "src/Display.h"
#include "src/ExternalStorage.h"
#include "src/Keypad.h"
#include "src/Page.h"
#include "src/UserJourney.h"
#include "src/Relay.h"
#include "src/Temperature.h"
#include "src/State.h"
#include "src/Daemon.h"

#include <MemoryFree.h>

#define BAUD_RATE 115200
// #define MEMORY_DEBUG

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
Temperature temperature;
State state;
Relay relay = Relay(&state);
UserJourney userJourney = UserJourney(&display, &relay, &temperature, &state);
Daemon daemon = Daemon(&state, &temperature, &userJourney);

void setup(void)
{
    Serial.begin(BAUD_RATE);
    prepareInterrupts();
    // clock.init();
    display.init();
    relay.init();
    externalStorage.init();
    temperature.init();
    userJourney.init();
}

#ifdef MEMORY_DEBUG
unsigned long lastTime = 0L;
#endif

void loop(void)
{
    daemon.handleOutstandingJobs();
    uint8_t keyToProcess = keypad.getKeyToProcess();
    if (keyToProcess != KEY_NONE)
    {
        userJourney.processKey(keyToProcess);
        keypad.markAsProcessed();
    }
    userJourney.renderCurrentPage();
    // externalStorage.printStorageInfo();

#ifdef MEMORY_DEBUG
    if (millis() - lastTime > 1000)
    {
        printFreeMemory();
        lastTime = millis();
    }
#endif
}

#ifdef MEMORY_DEBUG
void printFreeMemory()
{
    Serial.print(F("Free memory: "));
    Serial.print(freeMemory());
    Serial.println(F(" bytes"));
}
#endif

void prepareInterrupts()
{
    attachInterrupt(digitalPinToInterrupt(KEYPAD_INTERRUPT_PIN), onKeypadInterrupt, RISING);
}

void onKeypadInterrupt()
{
    keypad.onInterrupt();
}