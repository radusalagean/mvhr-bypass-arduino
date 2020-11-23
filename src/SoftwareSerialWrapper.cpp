#include "SoftwareSerialWrapper.h"

SoftwareSerialWrapper::SoftwareSerialWrapper(SoftwareSerial* softwareSerial)
{
    this->softwareSerial = softwareSerial;
}

int SoftwareSerialWrapper::available()
{
    return softwareSerial->available();
}

byte SoftwareSerialWrapper::peek()
{
    return softwareSerial->peek();
}

byte SoftwareSerialWrapper::read()
{
    return softwareSerial->read();
}

void SoftwareSerialWrapper::write(byte b)
{
    softwareSerial->write(b);
}

void SoftwareSerialWrapper::print(const char str[])
{
    softwareSerial->print(str);
}

void SoftwareSerialWrapper::print(char c)
{
    softwareSerial->print(c);
}

void SoftwareSerialWrapper::print(unsigned char uc, int format)
{
    softwareSerial->print(uc, format);
}

void SoftwareSerialWrapper::print(int i, int format)
{
    softwareSerial->print(i, format);
}

void SoftwareSerialWrapper::print(unsigned int ui, int format)
{
    softwareSerial->print(ui, format);
}

void SoftwareSerialWrapper::print(long l, int format)
{
    softwareSerial->print(l, format);
}

void SoftwareSerialWrapper::print(unsigned long ul, int format)
{
    softwareSerial->print(ul, format);
}

void SoftwareSerialWrapper::print(double d, int decimals)
{
    softwareSerial->print(d, decimals);
}

void SoftwareSerialWrapper::println(const char str[])
{
    softwareSerial->println(str);
}

void SoftwareSerialWrapper::println(char c)
{
    softwareSerial->println(c);
}

void SoftwareSerialWrapper::println(unsigned char uc, int format)
{
    softwareSerial->println(uc, format);
}

void SoftwareSerialWrapper::println(int i, int format)
{
    softwareSerial->println(i, format);
}

void SoftwareSerialWrapper::println(unsigned int ui, int format)
{
    softwareSerial->println(ui, format);
}

void SoftwareSerialWrapper::println(long l, int format)
{
    softwareSerial->println(l, format);
}

void SoftwareSerialWrapper::println(unsigned long ul, int format)
{
    softwareSerial->println(ul, format);
}

void SoftwareSerialWrapper::println(double d, int decimals)
{
    softwareSerial->println(d, decimals);
}