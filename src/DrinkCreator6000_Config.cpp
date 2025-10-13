/**
 * @file DrinkCreator6000_Config.cpp
 * @brief Implementation of DrinkCreator6000_Config.h
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */

#include <DrinkCreator6000_Config.h>
#include <DrinkCreator6000_Config_C.h>

HD44780_LCD lcd(LCD_ADDR,LCD_WIDTH,LCD_HEIGHT);
MCP23008 mcp(MCP_ADDR);