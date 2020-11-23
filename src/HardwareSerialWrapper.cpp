#include "HardwareSerialWrapper.h"

HardwareSerialWrapper::HardwareSerialWrapper(HardwareSerial* hardwareSerial)
{
    this->hardwareSerial = hardwareSerial;
}

int HardwareSerialWrapper::available()
{
    return hardwareSerial->available();
}

byte HardwareSerialWrapper::peek()
{
    return hardwareSerial->peek();
}

byte HardwareSerialWrapper::read()
{
    return hardwareSerial->read();
}

void HardwareSerialWrapper::write(byte b)
{
    hardwareSerial->write(b);
}

void HardwareSerialWrapper::print(const char str[])
{
    hardwareSerial->print(str);
}

void HardwareSerialWrapper::print(char c)
{
    hardwareSerial->print(c);
}

void HardwareSerialWrapper::print(unsigned char uc, int format)
{
    hardwareSerial->print(uc, format);
}

void HardwareSerialWrapper::print(int i, int format)
{
    hardwareSerial->print(i, format);
}

void HardwareSerialWrapper::print(unsigned int ui, int format)
{
    hardwareSerial->print(ui, format);
}

void HardwareSerialWrapper::print(long l, int format)
{
    hardwareSerial->print(l, format);
}

void HardwareSerialWrapper::print(unsigned long ul, int format)
{
    hardwareSerial->print(ul, format);
}

void HardwareSerialWrapper::print(double d, int decimals)
{
    hardwareSerial->print(d, decimals);
}

void HardwareSerialWrapper::println(const char str[])
{
    hardwareSerial->println(str);
}

void HardwareSerialWrapper::println(char c)
{
    hardwareSerial->println(c);
}

void HardwareSerialWrapper::println(unsigned char uc, int format)
{
    hardwareSerial->println(uc, format);
}

void HardwareSerialWrapper::println(int i, int format)
{
    hardwareSerial->println(i, format);
}

void HardwareSerialWrapper::println(unsigned int ui, int format)
{
    hardwareSerial->println(ui, format);
}

void HardwareSerialWrapper::println(long l, int format)
{
    hardwareSerial->println(l, format);
}

void HardwareSerialWrapper::println(unsigned long ul, int format)
{
    hardwareSerial->println(ul, format);
}

void HardwareSerialWrapper::println(double d, int decimals)
{
    hardwareSerial->println(d, decimals);
}