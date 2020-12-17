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
    if (tempC == DEVICE_DISCONNECTED_C ||
        -100.0f >= tempC || tempC >= 100.0f)
    {
        return TEMP_READ_ERROR;
    }
    return tempC;
}

float Temperature::getTempExtEv()
{
    return getTemp(extEv);
}

float Temperature::getTempExtAd()
{
    return getTemp(extAd);
}

float Temperature::getTempIntAd()
{
    return getTemp(intAd);
}

float Temperature::getTempIntEv()
{
    return getTemp(intEv);
}

Temperatures Temperature::getTemperatures()
{
    return Temperatures
    {
        getTempExtEv(),
        getTempExtAd(),
        getTempIntAd(),
        getTempIntEv()
    };
}
