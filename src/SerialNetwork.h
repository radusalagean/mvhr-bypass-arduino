#ifndef SERIAL_NETWORK_H
#define SERIAL_NETWORK_H

class SerialNetwork;

#include <SoftwareSerial.h>
#include "lib/mvhr-bypass-common/arduino-esp8266/BaseSerialNetwork.h"
#include "lib/mvhr-bypass-common/arduino-esp8266/InitData.h"
#include "Temperature.h"
#include "SoftwareSerialWrapper.h"
#include "HardwareSerialWrapper.h"
#include "Opcode.h"
#include "UserJourney.h"
#include "StateController.h"

#define REMOTE_SERIAL_BAUD_RATE 9600

class SerialNetwork : public BaseSerialNetwork
{
private:
    SoftwareSerial dataLineSerial = SoftwareSerial(8, 9); // RX, TX
    SoftwareSerial debugLineSerialRx = SoftwareSerial(A1, A2); // RX, TX
    SoftwareSerialWrapper dataLineSerialW = SoftwareSerialWrapper(&dataLineSerial);
    SoftwareSerialWrapper debugLineSerialRxW = SoftwareSerialWrapper(&debugLineSerialRx);
    HardwareSerialWrapper debugLineSerialTxW = HardwareSerialWrapper(&Serial);
    StateController* stateController = NULL;
    Temperature* temperature = NULL;
    UserJourney* userJourney = NULL;
    void sendInitData();
    void processPacket();
    
public:
    SerialNetwork(StateController* stateController, Temperature* temperature, UserJourney* userJourney);
    void init();
    void sendState();
    void sendTemperatures();
};

#endif