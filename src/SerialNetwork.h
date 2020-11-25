#ifndef SERIAL_NETWORK_H
#define SERIAL_NETWORK_H

#include <SoftwareSerial.h>
#include "lib/mvhr-bypass-common/arduino-esp8266/BaseSerialNetwork.h"
#include "StateController.h"
#include "SoftwareSerialWrapper.h"
#include "HardwareSerialWrapper.h"

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
    void sendState();
    
public:
    SerialNetwork(StateController* stateController);
    void init();
    void processPacket();
    void sendPacket(TransmissionPacket& packet);
};

#endif