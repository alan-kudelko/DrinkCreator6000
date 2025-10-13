/**
 * @file taskReadTemp.h
 * @brief Task for reading temperature data from an ATtiny configured as an I2C slave.
 *
 * This task communicates with an ATtiny microcontroller acting as an I2C slave
 * to read temperature sensor data and process it accordingly.
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */
#ifndef _TASK_READ_TEMP_H
    #define _TASK_READ_TEMP_H

/**
 * @brief FreeRTOS task function that reads temperature data via I2C.
 *
 * @param pvParameters Task parameters (unused).
 */
void taskReadTemp(void*pvParameters);

#endif // _TASK_READ_TEMP_H