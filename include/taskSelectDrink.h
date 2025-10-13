/**
 * @file taskSelectDrink.h
 * @brief Task responsible for selecting a drink to order.
 *
 * This task allows the user to select a drink, then sends
 * the screen update data to the `taskUpdateScreen` task
 * via the external queue `qScreenData`.
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */
#ifndef _TASK_SELECT_DRINK_H_
    #define _TASK_SELECT_DRINK_H_

/**
 * @brief Handles the drink selection process.
 *
 * @param pvParameters Task parameters (unused).
 */

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

void taskSelectDrink(void*pvParameters);

#ifdef __cplusplus
    }
#endif // __cplusplus

#endif // _TASK_SELECT_DRINK_H_