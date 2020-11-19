#ifndef NETWORK_H
#define NETWORK_H

#include <Arduino.h>
#include <SoftwareSerial.h>

#define NETWORK_BAUD_RATE 9600

class Network
{
private:
    SoftwareSerial networkSerial = SoftwareSerial(8, 9); // RX, TX
public:
    void init();
    void handle();
};

#endif