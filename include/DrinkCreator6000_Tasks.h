/**
 * @file DrinkCreator6000_Tasks.h
 * @brief Task includes and declarations for the DrinkCreator6000 FreeRTOS application.
 *
 * This header file includes all task-related headers and necessary FreeRTOS
 * components used throughout the DrinkCreator6000 project.
 *
 * Each included task header corresponds to a specific application task,
 * identified by a unique numeric ID (commented).
 *
 * @note This file centralizes task includes for easy management and integration.
 *
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */
#ifndef _DRINK_CREATOR_6000_TASKS_H_
    #define _DRINK_CREATOR_6000_TASKS_H_

#include <taskErrorHandler.h>          ///< Task 0: Error handling
#include <taskSerialSystemDebugger.h>  ///< Task 1: Serial system debugger
#include <taskMain.h>                  ///< Task 2: Main application logic
#include <taskReadInput.h>             ///< Task 3: Input reading
#include <taskSerialInput.h>           ///< Task 4: Serial input processing
#include <taskUpdateScreen.h>          ///< Task 5: LCD screen update
#include <taskReadTemp.h>              ///< Task 6: Temperature reading
#include <taskRegulateTemp.h>          ///< Task 7: Temperature regulation
#include <taskSelectDrink.h>           ///< Task 8: Drink selection
#include <taskOrderDrink.h>            ///< Task 9: Drink ordering
#include <taskShowSystemInfo.h>        ///< Task 10: System info display
#include <taskTestHardware.h>          ///< Task 11: Test hardware
#include <taskHardwareControl.h>       ///< Task 12: Hardware control

#endif // _DRINK_CREATOR_6000_TASKS_H_