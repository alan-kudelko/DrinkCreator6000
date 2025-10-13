/**
 * @file DrinkCreator6000_DataTypes.h
 * @brief Custom data types for controlling application context in the DrinkCreator6000 project.
 *
 * This module defines shared data structures and enumerations used to control
 * the application state, including:
 * - Menu and submenu numbers
 * - LCD processing task input data
 * - Information about the drinks
 * - System error signature structure
 *
 * These types are exchanged between FreeRTOS tasks and helper functions to maintain
 * a consistent application context.
 *
 * @note Designed for use in a FreeRTOS environment but can also be reused in non-RTOS builds.
 * @note All types are project-specific and not tied to AVR hardware registers or peripherals.
 *
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */
#ifndef DRINK_CREATOR6000_DATA_TYPES_H_
    #define DRINK_CREATOR6000_DATA_TYPES_H_

#include <stdint.h>

#define DRINK_NAME_LENGTH 16

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus
struct sDrinkData{
    char drinkName[DRINK_NAME_LENGTH];
    uint16_t ingredients[8];
    uint16_t numberOfOrders;
    // Can be optimzed
};
/**
 * @struct sScreenData
 * @brief Structure containing data to be passed to the taskActivatePumps.
 *
 * This structure holds the text lines to be displayed on the LCD screen
 * along with cursor position and blink control.
 *
 * @param lines Array of 4 strings, each up to 20 characters, representing
 *              the LCD lines.
 * @param lcdCursorX Cursor X position (0-19).
 * @param lcdCursorY Cursor Y position (0-3).
 * @param lcdCursorBlink Cursor blink flag (1 = blink enabled, 0 = disabled).
 */
struct sScreenData {
    char lines[4][20];
    struct {
        uint8_t lcdCursorX:5;
        uint8_t lcdCursorY:2;
        uint8_t lcdCursorBlink:1;
    };
};
/**
 * @struct sSystemError
 * @brief Represents an error event in the system.
 *
 * This structure is used to pass error details to tasks such as
 * @ref taskUpdateScreen for displaying error messages on the LCD
 * and sending them over the serial port.
 *
 * The use of bit fields minimizes memory usage while storing time
 * information and status flags.
 *
 * @var sSystemError::errorText
 * Null-terminated string containing a human-readable error description.
 *
 * @var sSystemError::seconds
 * Number of seconds since the error occurred (0–59).
 *
 * @var sSystemError::confirmed
 * Flag indicating whether the error has been acknowledged.
 * If not confirmed, it should be shown on both the LCD and the serial port during a startup.
 *
 * @var sSystemError::minutes
 * Number of minutes since the error occurred (0–59).
 *
 * @var sSystemError::hours
 * Number of hours since the error occurred (0–23).
 *
 * @var sSystemError::days
 * Number of days since the error occurred (0–255).
 *
 * @var sSystemError::taskId
 * ID of the task that caused the error (0–31).
 */
struct sSystemError{
    char errorText[50];
    uint8_t seconds   :6;
    uint8_t confirmed :1; ///< If not confirmed, display on LCD and serial port
    uint8_t minutes   :6;
    uint8_t hours     :5;
    uint8_t days      :8;
    uint8_t taskId    :5; ///< ID of the task which caused the error
};
/**
 * @brief Structure describing the current UI context of the application.
 *
 * This structure stores the current state of the user interface, including:
 * - Which task is currently bound to the UI
 * - The active menu and submenu
 * - UI behavior flags (e.g., auto-scrolling)
 */
struct sUIContext{
    /**
     * @brief Auto-scroll enable flag.
     *
     * 1 = automatic scrolling is enabled,  
     * 0 = automatic scrolling is disabled.
     */
    uint8_t autoScrollEnable:1;
    /**
     * @brief Current task bound to the UI.
     *
     * Possible values:  
     * - 0 = taskSelectDrink  
     * - 1 = taskOrderDrink  
     * - 2 = taskShowSystemInfo
     *  -3 = taskTestHardware
     */
    uint8_t currentTask:2;
    /**
     * @brief Current menu index in the given task.
     *
     * Example for taskShowSystemInfo:  
     * - 0 = showInfo_Firmware  
     * - 1 = showInfo_Temp  
     * - 2 = showInfo_Memory  
     * - 3 = showInfo_Task  
     * - 4 = showInfo_Error_Sub_N (submenu for scrolling messages)
     * - 5 = showInfo_ConfError
     *
     * The default submenu is indexed as 0.
     */
    uint8_t currentMenu:3;
    /**
     * @brief Current submenu index.
     *
     * Used for scrolling inside the current menu.  
     * Cannot be optimized with the current UI design.
     */
    uint8_t currentSubMenu;
};

#ifdef __cplusplus
    }
#endif // __cplusplus

#endif // DRINK_CREATOR6000_DATA_TYPES_H_
