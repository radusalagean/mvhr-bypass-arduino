#ifndef INTERNAL_STORAGE_H
#define INTERNAL_STORAGE_H

#include <EEPROM.h>
#include <Arduino.h>
#include "State.h"

#define EEPROM_DEBUG

class InternalStorage
{
private:
public:
#ifdef EEPROM_DEBUG
    void printContentToSerial();
#endif
    template<typename T> inline void get(const int memAddress, T& obj) __attribute__((always_inline));
    template<typename T> inline void put(const int memAddress, const T& obj) __attribute__((always_inline));
    void clear();
};

template<typename T> void InternalStorage::get(const int memAddress, T& obj)
{
    EEPROM.get(memAddress, obj);
}

template<typename T> void InternalStorage::put(const int memAddress, const T& obj)
{
    EEPROM.put(memAddress, obj);
}

#endif