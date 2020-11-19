#include "Network.h"

void Network::init()
{
    networkSerial.begin(NETWORK_BAUD_RATE);
}

void Network::handle()
{
    if (networkSerial.available()) 
    {
        Serial.write(networkSerial.read());
    }
    if (Serial.available()) 
    {
        networkSerial.write(Serial.read());
    }
}