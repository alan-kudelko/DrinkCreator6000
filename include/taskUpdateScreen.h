/**
 * @file taskUpdateScreen.h
 * @brief Screen update task for DrinkCreator6000.
 *
 * This task consumes the screen data queue and handles LCD display refresh,
 * including custom characters and cursor control. It synchronizes access
 * to the I2C bus using the `mux_I2CLock` mutex.
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */
#ifndef _TASK_UPDATE_SCREEN_H_
    #define _TASK_UPDATE_SCREEN_H_

void taskUpdateScreen(void*pvParameters);

#endif // _TASK_UPDATE_SCREEN_H_
