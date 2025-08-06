/**
 * @file DrinkCreator6000_Init.h
 * @brief System initialization routines for the DrinkCreator6000.
 *
 * This header declares initialization functions responsible for
 * configuring I/O pins, allocating memory for RTOS objects, and
 * initializing hardware peripherals such as UART, LCD, I²C devices,
 * and shift registers.
 */

#ifndef _DRINK_CREATOR6000_INIT_H_
#define _DRINK_CREATOR6000_INIT_H_

#include "DrinkCreator6000_Config_C.h"

void initializeUART(void);

/**
 * @brief Initializes I/O pins and attaches interrupts related to pin events.
 *
 * This function configures the directions and states of GPIO pins used by
 * external devices, such as buttons, relays, and control lines.
 */
void initializeIO(void);


void initializeEEPROM(void);
/**
 * @brief Initializes memory resources for FreeRTOS.
 *
 * This includes static allocation of task stacks, queues, semaphores,
 * and mutexes. It also configures guard zones if enabled.
 */
void initializeMemory(void);

/**
 * @brief Initializes hardware peripherals.
 *
 * Sets up UART (for debug output), the LCD display via I²C, the I²C
 * keyboard using MCP23017, the 74HC595 shift register, and the temperature
 * sensor (if applicable).
 */
void initializeHardware(void);

void initializeInterrupts(void);

void init8(void)__attribute((naked,used,section(".init8")));

#endif // _DRINK_CREATOR6000_INIT_H_