#ifndef _DRINK_CREATOR6000_EEPROM_H_
#define _DRINK_CREATOR6000_EEPROM_H_
#include <avr/eeprom.h>

#include "DrinkCreator6000_DataTypes.h"

enum{
  EEPROM_BOOTUPS_ADDR=0x0C00,
  EEPROM_LAST_ERROR_ADDR=0x0800
};

void EEPROMUpdateBootups(uint16_t*bootupsCount);

void EEPROMGetLastStartupError(sSystemError*lastError);

void EEPROMUpdateLastStartupError(sSystemError*errorStruct);

#endif // _DRINK_CREATOR6000_EEPROM_H
