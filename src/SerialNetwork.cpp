#include "SerialNetwork.h"

SerialNetwork::SerialNetwork(StateController* stateController, Temperature* temperature, UserJourney* userJourney) : 
    BaseSerialNetwork(&dataLineSerialW, &debugLineSerialRxW, &debugLineSerialTxW)
{
    this->stateController = stateController;
    this->temperature = temperature;
    this->userJourney = userJourney;
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
    case LOCAL_CONTRACT_CODE_REQUEST_INIT_DATA:
        sendInitData();
        break;
    case LOCAL_CONTRACT_CODE_REQUEST_HR_MODE_AUTO:
        userJourney->processOpcode(OPCODE_HR_MODE_AUTO);
        break;
    case LOCAL_CONTRACT_CODE_REQUEST_HR_MODE_MANUAL:
        userJourney->processOpcode(OPCODE_HR_MODE_MANUAL);
        break;
    case LOCAL_CONTRACT_CODE_REQUEST_ENABLE_HR:
        userJourney->setManualHrDisabled(false);
        break;
    case LOCAL_CONTRACT_CODE_REQUEST_DISABLE_HR:
        userJourney->setManualHrDisabled(true);
        break;
    default:
        break;
    }
}

void SerialNetwork::sendInitData()
{
    InitData initData = InitData {stateController->getState(), temperature->getTemperatures()};
    send(&initData, LOCAL_CONTRACT_CODE_RESPONSE_INIT_DATA);
}

void SerialNetwork::sendState()
{
    send(&stateController->getState(), LOCAL_CONTRACT_CODE_RESPONSE_STATE);
}

void SerialNetwork::sendTemperatures()
{
    send(&temperature->getTemperatures(), LOCAL_CONTRACT_CODE_RESPONSE_TEMPERATURES);
}

template<typename T>
void SerialNetwork::send(T* t, uint8_t code)
{
    byte* buffer = new byte[sizeof(T)];
    memcpy(buffer, t, sizeof(T));
    TransmissionPacket packet =
    {
        code, sizeof(T), buffer
    };
    sendPacket(packet);
    delete[] buffer;
}