#include "SerialNetwork.h"

SerialNetwork::SerialNetwork(StateController* stateController) : 
    BaseSerialNetwork(&dataLineSerialW, &debugLineSerialRxW, &debugLineSerialTxW)
{
    this->stateController = stateController;
}

void SerialNetwork::init()
{
    // note: the last initialized serial line is put into exclusive listen mode by default!
    dataLineSerial.begin(REMOTE_SERIAL_BAUD_RATE);
    debugLineSerialRx.begin(REMOTE_SERIAL_BAUD_RATE);
    listenData();
}

void SerialNetwork::processPacket()
{
    switch (transmissionPacket.code)
    {
    case LOCAL_CONTRACT_CODE_REQUEST_STATE:
        sendState();
        break;
    
    default:
        break;
    }
}

void SerialNetwork::sendState()
{
    byte* buffer = new byte[sizeof(State)];
    memcpy(buffer, &stateController->getState(), sizeof(State));
    TransmissionPacket packet =
    {
        LOCAL_CONTRACT_CODE_RESPONSE_STATE, sizeof(State), buffer
    };
    sendPacket(packet);
    delete[] buffer;
}

void SerialNetwork::printRemoteDebugMessages()
{
    if (debugLineSerialRx.isListening())
    {
        if (millis() - debugListenStartTime > 5000)
        {
            listenData();
        }
        else
        {
            while (debugLineSerialRx.available())
                Serial.write(debugLineSerialRx.read());
        }
    }
}

void SerialNetwork::listenDebug()
{
    debugLineSerialRx.listen();
}

void SerialNetwork::listenData()
{
    dataLineSerial.listen();
}

void SerialNetwork::sendPacket(TransmissionPacket& packet)
{
    BaseSerialNetwork::sendPacket(packet);
    listenDebug();
    debugListenStartTime = millis();
}