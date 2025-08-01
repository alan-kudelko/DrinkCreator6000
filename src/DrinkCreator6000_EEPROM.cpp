#include "DrinkCreator6000_EEPROM.h"

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

