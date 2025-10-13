/**
 * @file DrinkCreator6000_EEPROM.h
 * @brief EEPROM access functions for bootup counts and last error storage.
 *
 * This header provides addresses and functions to read/write
 * persistent data stored in the AVR EEPROM memory, including
 * the bootup counter and the last system error.
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */
#ifndef _DRINK_CREATOR6000_EEPROM_H_
    #define _DRINK_CREATOR6000_EEPROM_H_

/** EEPROM address for storing the number of system boot-ups */
#define EEPROM_BOOTUPS_ADDR      0x0C00

/** EEPROM address for storing the last system error */
#define EEPROM_LAST_ERROR_ADDR   0x0800

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus
/** Forward declaration of system error structure */
struct sSystemError;
/**
 * @brief Updates the stored bootup count in EEPROM.
 * 
 * @param[in,out] bootupsCount Pointer to the bootup count variable to update.
 */
extern void EEPROMUpdateBootups(uint16_t*bootupsCount);
/**
 * @brief Retrieves the last startup error from EEPROM.
 * 
 * @param[out] lastError Pointer to a structure where the last error will be copied.
 */
extern void EEPROMGetLastStartupError(struct sSystemError*lastError);
/**
 * @brief Updates the last startup error stored in EEPROM.
 * 
 * @param[in] errorStruct Pointer to the system error structure to be saved.
 */
extern void EEPROMUpdateLastStartupError(struct sSystemError*errorStruct);

#ifdef __cplusplus
    }
#endif // __cplusplus

#endif // _DRINK_CREATOR6000_EEPROM_H_