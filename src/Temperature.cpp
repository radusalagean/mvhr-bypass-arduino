#include "Temperature.h"

void Temperature::init()
{
    sensors.begin();
    sensors.setResolution(extEv, ONE_WIRE_RESOLUTION);
    sensors.setResolution(extAd, ONE_WIRE_RESOLUTION);
    sensors.setResolution(intAd, ONE_WIRE_RESOLUTION);
    sensors.setResolution(intEv, ONE_WIRE_RESOLUTION);
}

void Temperature::requestTemperatures()
{
    sensors.requestTemperatures();
}

/**
 * Returns the temperature or TEMP_READ_ERROR
 */
float Temperature::getTemp(DeviceAddress& addr)
{
    float tempC = sensors.getTempC(addr);
    if (tempC == DEVICE_DISCONNECTED_C)
    {
        return TEMP_READ_ERROR;
    }
    return tempC;
}

float Temperature::getTempExtEv()
{
    // return 21.f;
    return getTemp(extEv);
}

float Temperature::getTempExtAd()
{
    // return 11.f;
    return getTemp(extAd);
}

float Temperature::getTempIntAd()
{
    // return 12.f;
    return getTemp(intAd);
}

float Temperature::getTempIntEv()
{
    // return 22.f;
    return getTemp(intEv);
}
