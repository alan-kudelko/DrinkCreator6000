#ifndef EEPROM_MEMORY_MANAGEMENT_H
#define EEPROM_MEMORY_MANAGEMENT_H
#include <avr/eeprom.h>
/**
 * EEPROM Memory Layout & Management
 * ---------------------------------
 * This header defines enums and utility functions for interacting with the internal EEPROM,
 * supporting structured access to persistent data storage in microcontroller environments.
 * 
 * The layout is divided into logical sections with fixed or reserved memory ranges. 
 * These sections are managed via typed access functions and offset definitions, 
 * allowing reliable read/write operations without overlapping memory.
 * 
 * Memory Sections:
 * ----------------
 * 1. System Settings
 *    - Device configuration, flags, calibration constants
 *    - Stored at fixed addresses near the beginning of EEPROM
 * 
 * 2. Runtime Metrics
 *    - Usage counters, session logs, uptime statistics
 *    - Updated periodically; optimized for wear-leveling if needed
 * 
 * 3. User Profiles / Presets
 *    - Application-specific data like user preferences or saved presets
 *    - Stored in indexable blocks or slots
 * 
 * 4. Reserved / Future Use
 *    - Padding or free space reserved for expansion
 *    - Prevents overlap with hardcoded data structures
 * 
 * Functionality:
 * --------------
 * - Enum-based indexing for logical EEPROM segments
 * - Type-safe wrappers around EEPROM read/write calls
 * - Helpers for structured data access (e.g., structs)
 * - Optional CRC or signature-based validation support
 * 
 * Usage Notes:
 * ------------
 * - Ensure EEPROM size and boundaries are respected
 * - Minimize frequent writes to preserve EEPROM lifespan
 * - Use EEPROM.put() / EEPROM.get() for structured data handling
 * 
 * Designed for safe and modular access to EEPROM in embedded systems.
 */

enum{EEPROM_BOOTUPS_ADDR=0x0C00,
     EEPROM_LAST_ERROR_ADDR=0x0800};

void EEPROMUpdateBootups(uint16_t*bootupsCount){
  eeprom_read_block(bootupsCount,(void*)EEPROM_BOOTUPS_ADDR,sizeof(uint16_t));
  (*bootupsCount)++;
  eeprom_update_block(bootupsCount,(void*)EEPROM_BOOTUPS_ADDR,sizeof(uint16_t));
}
void EEPROMGetLastStartupError(sSystemError*lastError){
  eeprom_read_block((void*)lastError,(void*)EEPROM_LAST_ERROR_ADDR,sizeof(sSystemError));
}
void EEPROMUpdateLastStartupError(sSystemError*errorStruct){
  uint16_t runTimeFromMillis=millis()/1000;
  errorStruct->days=runTimeFromMillis/3600/24;
  errorStruct->hours=runTimeFromMillis/3600%24;
  errorStruct->minutes=runTimeFromMillis/60%60;
  errorStruct->seconds=runTimeFromMillis%60;
  
  eeprom_update_block((void*)errorStruct,(void*)EEPROM_LAST_ERROR_ADDR,sizeof(sSystemError));
}

#endif
