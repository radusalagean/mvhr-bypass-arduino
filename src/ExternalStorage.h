#ifndef EXTERNAL_STORAGE_H
#define EXTERNAL_STORAGE_H

#include <SPI.h>
#include <SD.h>

class ExternalStorage
{
private:
    Sd2Card card;
    SdVolume volume;
    SdFile root;
public:
    void init();
    void printStorageInfo();
};

#endif