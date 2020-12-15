#include "src/Display.h"
#include "src/InternalStorage.h"
#include "src/Keypad.h"
#include "src/Page.h"
#include "src/UserJourney.h"
#include "src/Relay.h"
#include "src/Temperature.h"
#include "src/lib/mvhr-bypass-common/arduino-esp8266/State.h"
#include "src/StateController.h"
#include "src/Daemon.h"
#include "src/SerialNetwork.h"

#define DEBUG

#ifdef DEBUG

#include <MemoryFree.h>
#define BAUD_RATE 9600
// #define MEMORY_DEBUG

#endif

/**
 * PINs configuration (for Arduino Uno, R3):
 * IOREF -
 * RESET -
 * 3.3V -
 * 5V -
 * A0 - Keypad analog in
 * A1 - DEBUG SoftwareSerial RX -> Connect to TX1 (GPIO 2) of ESP8266
 * A2 - SoftwareSerial TX (Assigned but not used)
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
 * D8 - SoftwareSerial RX -> Connect to TX of ESP8266
 * D9 (PWM) - SoftwareSerial TX -> Connect to RX of ESP8266 (through voltage divider)
 * D10 (SPI SS, PWM) - Display Backlight
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
SerialNetwork serialNetwork = SerialNetwork(&stateController, &temperature);
Daemon daemon = Daemon(&stateController, &temperature, &userJourney, &serialNetwork);

void setup(void)
{
#ifdef DEBUG
    Serial.begin(BAUD_RATE);
    while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
    }
#ifdef EEPROM_DEBUG
    internalStorage.printContentToSerial();
#endif // EEPROM_DEBUG
#endif // DEBUG
    prepareInterrupts();
    stateController.loadPersistedState();
    display.init();
    relay.init();
    temperature.init();
    userJourney.init();
    serialNetwork.init();
    daemon.refreshTemperatureData();
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
    serialNetwork.handleOutstandingPackets();

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