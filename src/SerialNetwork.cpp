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
    case LOCAL_CONTRACT_CODE_REQUEST_APPLY_STATE_TEMPERATURES:
        State* state = receivePacketBodyObject<State>();
        stateController->setTemperatures(state->hysteresis, state->intEvMin, 
                                         state->extAdMin, state->extAdMax);
        userJourney->processOpcode(OPCODE_APPLY_STATE_TEMPERATURES);
        delete state;
        break;
    default:
        break;
    }
}

void SerialNetwork::sendInitData()
{
    InitData initData = InitData {stateController->getState(), temperature->getTemperatures()};
    send(LOCAL_CONTRACT_CODE_RESPONSE_INIT_DATA, &initData);
}

void SerialNetwork::sendState()
{
    send(LOCAL_CONTRACT_CODE_RESPONSE_STATE, &stateController->getState());
}

void SerialNetwork::sendTemperatures()
{
    send(LOCAL_CONTRACT_CODE_RESPONSE_TEMPERATURES, &temperature->getTemperatures());
}