#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_PIN 2
#define ONE_WIRE_RESOLUTION 12

#define TEMP_READ_ERROR -100.f

class Temperature
{
private:
    DeviceAddress extEv = { 0x28, 0x22, 0x46, 0x79, 0x97, 0x09, 0x03, 0x8A };
    DeviceAddress extAd = { 0x28, 0x6A, 0x35, 0x07, 0xD6, 0x01, 0x3C, 0xD3 };
    DeviceAddress intAd = { 0x28, 0xC6, 0x4B, 0x07, 0xD6, 0x01, 0x3C, 0x3D };
    DeviceAddress intEv = { 0x28, 0x1E, 0x11, 0x94, 0x97, 0x08, 0x03, 0xCE };
    float getTemp(DeviceAddress& addr);
public:
    OneWire oneWire = OneWire(ONE_WIRE_PIN);
    DallasTemperature sensors = DallasTemperature(&oneWire);
    void init();
    void requestTemperatures();
    float getTempExtEv();
    float getTempExtAd();
    float getTempIntAd();
    float getTempIntEv();
};

#endif