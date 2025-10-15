/**
 * @file taskMain.h
 * @brief Main task coordinating the operation of other system tasks.
 *
 * This task manages the application context and user interface state,
 * delegating input processing to helper functions corresponding to
 * different UI screens or modes.
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */
#ifndef _TASK_MAIN_H_
    #define _TASK_MAIN_H_

#include <DrinkCreator6000_DataTypes.h>

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

void taskMain_ProcessContext_taskTestHardware(uint8_t*keyboardInput,volatile struct sUIContext*UI_Context);
/**
 * @brief Main FreeRTOS task that coordinates system workflow.
 */
void taskMain(void*pvParameters);

#ifdef __cplusplus
    }
#endif // __cplusplus

#endif // _TASK_MAIN_H_