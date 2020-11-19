#ifndef SERIAL_NETWORK_H
#define SERIAL_NETWORK_H

#include <Arduino.h>
#include <SoftwareSerial.h>

#define REMOTE_SERIAL_BAUD_RATE 9600

class SerialNetwork
{
private:
    SoftwareSerial remoteSerial = SoftwareSerial(8, 9); // RX, TX
public:
    void init();
    void handle();
};

#endif