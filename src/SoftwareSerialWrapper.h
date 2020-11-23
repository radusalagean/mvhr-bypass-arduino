#ifndef SOFTWARE_SERIAL_WRAPPER_H
#define SOFTWARE_SERIAL_WRAPPER_H

#include "lib/mvhr-bypass-common/arduino-esp8266/SerialWrapper.h"
#include <SoftwareSerial.h>

class SoftwareSerialWrapper : public SerialWrapper
{
private:
    SoftwareSerial* softwareSerial = NULL;

public:
    SoftwareSerialWrapper(SoftwareSerial* softwareSerial);
    
    int available();
    byte peek();
    byte read();
    void write(byte b);

    void print(const char str[]);
    void print(char c);
    void print(unsigned char uc, int format = DEC);
    void print(int i, int format = DEC);
    void print(unsigned int ui, int format = DEC);
    void print(long l, int format = DEC);
    void print(unsigned long ul, int format = DEC);
    void print(double d, int decimals = 2);

    void println(const char str[]);
    void println(char c);
    void println(unsigned char uc, int format = DEC);
    void println(int i, int format = DEC);
    void println(unsigned int ui, int format = DEC);
    void println(long l, int format = DEC);
    void println(unsigned long ul, int format = DEC);
    void println(double d, int decimals = 2);
};

#endif