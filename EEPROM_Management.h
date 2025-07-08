#ifndef _EEPROM_MEMORY_MANAGEMENT_H_
#define _EEPROM_MEMORY_MANAGEMENT_H_
#include <avr/eeprom.h>

enum{
  EEPROM_BOOTUPS_ADDR=0x0C00,
  EEPROM_LAST_ERROR_ADDR=0x0800
};

void EEPROMUpdateBootups(uint16_t*bootupsCount){
  eeprom_read_block(bootupsCount,(void*)EEPROM_BOOTUPS_ADDR,sizeof(uint16_t));
  (*bootupsCount)++;
  eeprom_update_block(bootupsCount,(void*)EEPROM_BOOTUPS_ADDR,sizeof(uint16_t));
}
void EEPROMGetLastStartupError(sSystemError*lastError){
  eeprom_read_block((void*)lastError,(void*)EEPROM_LAST_ERROR_ADDR,sizeof(sSystemError));
}
void EEPROMUpdateLastStartupError(sSystemError*errorStruct){
  eeprom_update_block((void*)errorStruct,(void*)EEPROM_LAST_ERROR_ADDR,sizeof(sSystemError));
}

#endif // _EEPROM_MEMORY_MANAGEMENT_H_
