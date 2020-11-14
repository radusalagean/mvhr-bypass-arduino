#include "src/Display.h"
#include "src/InternalStorage.h"
#include "src/Keypad.h"
#include "src/Page.h"
#include "src/UserJourney.h"
#include "src/Relay.h"
#include "src/Temperature.h"
#include "src/State.h"
#include "src/StateController.h"
#include "src/Daemon.h"

#define DEBUG

#ifdef DEBUG

#include <MemoryFree.h>
#define BAUD_RATE 115200
// #define MEMORY_DEBUG

#endif

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
 * A4 -
 * A5 -
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
 * D10 (SPI SS, PWM) -
 * D11 (SPI MOSI, PWM) - Display (DATA - SCA)
 * D12 (SPI MISO) -
 * D13 (SPI SCK) - Display (CLOCK - SCL)
 * AREF -
 * SDA (I2C Data) -
 * SCL (I2C Clock) -
 * */

Display display;
InternalStorage internalStorage;
Keypad keypad;
Temperature temperature;
StateController stateController = StateController(&internalStorage);
Relay relay = Relay(&stateController);
UserJourney userJourney = UserJourney(&display, &relay, &temperature, &stateController);
Daemon daemon = Daemon(&stateController, &temperature, &userJourney);

void setup(void)
{
#ifdef DEBUG
    Serial.begin(BAUD_RATE);
    while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
    }
    internalStorage.printContentToSerial();
#endif
    prepareInterrupts();
    stateController.loadPersistedState();
    display.init();
    relay.init();
    temperature.init();
    userJourney.init();
}

#ifdef MEMORY_DEBUG
unsigned long lastMemCheck = 0L;
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

#ifdef MEMORY_DEBUG
    if (millis() - lastMemCheck > 1000)
    {
        printFreeMemory();
        lastMemCheck = millis();
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