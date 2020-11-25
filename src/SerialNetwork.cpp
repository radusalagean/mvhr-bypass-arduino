#include "SerialNetwork.h"

SerialNetwork::SerialNetwork(StateController* stateController) : 
    BaseSerialNetwork(&dataLineSerialW, &debugLineSerialRxW, &debugLineSerialTxW)
{
    this->stateController = stateController;
}

void SerialNetwork::init()
{
    // note: the last initialized serial line is put into exclusive listen mode by default!
    debugLineSerialRx.begin(REMOTE_SERIAL_BAUD_RATE);
    dataLineSerial.begin(REMOTE_SERIAL_BAUD_RATE);
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

void SerialNetwork::sendPacket(TransmissionPacket& packet)
{
    BaseSerialNetwork::sendPacket(packet);
}