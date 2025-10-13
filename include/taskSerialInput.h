/**
 * @file taskSerialInput.h
 * @brief Task responsible for reading serial input data from UART.
 *
 * This task reads diagnostic or command data from the UART interface
 * and sends it to the main task via the external queue `qKeyboardData`.
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */
#ifndef _TASK_SERIAL_INPUT_H_
    #define _TASK_SERIAL_INPUT_H_

/**
 * @brief Reads input data from UART and enqueues to main task.
 *
 * @param pvParameters Task parameters (unused).
 */
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

void taskSerialInput(void*pvParameters);

#ifdef __cplusplus
    }
#endif // __cplusplus

#endif // _TASK_SERIAL_INPUT_H_
