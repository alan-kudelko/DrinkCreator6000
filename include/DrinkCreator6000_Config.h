/**
 * @file DrinkCreator6000_Config.h
 * @brief Centralized configuration header for the DrinkCreator6000 system, containing global C++ variables.
 *
 * This header defines project-wide configuration macros and declares global
 * C++ objects used across the DrinkCreator6000 project, such as:
 *   - HD44780_LCD controller object
 *   - MCP2300XX I/O expander object
 *
 * @note These are global variables in C++ and accessible from any module
 *       including this header. 
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */
#ifndef _DRINK_CREATOR6000_CONFIG_H_
    #define _DRINK_CREATOR6000_CONFIG_H_

#include <HD44780_I2C.h>
#include <MCP230XX.h>

extern HD44780_LCD lcd;
extern MCP23008 mcp;

#endif // _DRINK_CREATOR6000_CONFIG_H_
