#include "InternalStorage.h"

#ifdef EEPROM_DEBUG
void InternalStorage::printContentToSerial()
{
    Serial.print(F("EEPROM length: "));
    Serial.println(EEPROM.length());
    Serial.print(F("EEPROM content: ["));
    byte current = 0;
    for (int i = 0; i < EEPROM.length(); i++)
    {
        EEPROM.get(i, current);
        Serial.print(current, BIN);
        Serial.print(F(" "));
    }
    Serial.println(F("]"));
}
#endif

void InternalStorage::clear()
{
    for (int i = 0; i < EEPROM.length(); i++)
    {
        EEPROM.write(i, 0);
    }
}