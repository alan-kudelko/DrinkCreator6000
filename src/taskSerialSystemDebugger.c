/**
 * @file taskSerialSystemDebugger.c
 * @brief Implementation of taskSerialSystemDebugger.h
 *
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */

#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include <taskSerialSystemDebugger.h>
#include <DrinkCreator6000_Progmem.h>
#include <DrinkCreator6000_Config_C.h>
#include <DrinkCreator6000_RamStats.h>
#include <systemDebug.h>
#include <uart.h>

void taskSerialSystemDebugger(void*pvParameters){
    for(;;){
        task_dump();

        updateMemoryUsage();

	    ram_dump();
        
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
