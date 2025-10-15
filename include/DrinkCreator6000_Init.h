/**
 * @file DrinkCreator6000_Init.h
 * @brief System initialization routines for the DrinkCreator6000.
 *
 * This header declares initialization functions responsible for
 * configuring I/O pins, allocating memory for RTOS objects, and
 * initializing hardware peripherals such as UART, LCD, I²C devices,
 * and shift registers.
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */
#ifndef _DRINK_CREATOR6000_INIT_H_
    #define _DRINK_CREATOR6000_INIT_H_

/**
 * @brief Initialize UART peripheral and related settings.
 *
 * Configures the UART for serial communication (typically for debugging),
 * enables global interrupts, and sends a ready message.
 */
void initializeUART(void);
/**
 * @brief Initialize I/O pins and attach pin-related interrupts.
 *
 * Configures GPIO directions and states for connected hardware components
 * such as buttons, relays, sensors, and control lines.
 */
void initializeIO(void);
/**
 * @brief Initialize EEPROM memory access.
 *
 * Waits for EEPROM readiness and prepares it for read/write operations.
 */
void initializeEEPROM(void);
/**
 * @brief Initializes memory resources for FreeRTOS.
 *
 * This includes static allocation of task stacks, queues, semaphores,
 * and mutexes. It also configures guard zones if enabled.
 */
void initializeMemory(void);
/**
 * @brief Initialize RTOS memory resources and guard zones.
 *
 * Performs static allocation of task stacks, queues, semaphores, mutexes,
 * and sets up memory guard zones to detect overruns.
 */
void initializeHardware(void);
/**
 * @brief System startup routine executed early in the boot sequence.
 *
 * Calls all initialization functions in the correct order to prepare
 * the system for operation.
 *
 * @note Marked with GCC `naked` and `used` attributes, and placed in the `.init8`
 *       linker section for execution during startup.
 */
void init8(void)__attribute((naked,used,section(".init8")));

#endif // _DRINK_CREATOR6000_INIT_H_