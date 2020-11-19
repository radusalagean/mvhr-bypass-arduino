#include "SerialNetwork.h"

void SerialNetwork::init()
{
    remoteSerial.begin(REMOTE_SERIAL_BAUD_RATE);
}

void SerialNetwork::handle()
{
    if (remoteSerial.available()) 
    {
        Serial.write(remoteSerial.read());
    }
    if (Serial.available()) 
    {
        remoteSerial.write(Serial.read());
    }
}