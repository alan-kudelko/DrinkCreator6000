/**
 * @file taskReadInput.h
 * @brief Task responsible for reading input data from MCP23017 via I2C.
 *
 * This task reads keyboard/input data from the MCP23017 I2C I/O expander
 * and sends the data to the main task through the external queue 
 * `qKeyboardData`.
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */
#ifndef _TASK_READ_INPUT_H_
    #define _TASK_READ_INPUT_H_

/**
 * @brief Reads input data from MCP23017 and enqueues to main task.
 *
 * @param pvParameters Task parameters (unused).
 */
void taskReadInput(void*pvParameters);

#endif // _TASK_READ_INPUT_H_