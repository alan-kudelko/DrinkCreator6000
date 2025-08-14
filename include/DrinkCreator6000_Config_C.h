/**
 * @file DrinkCreator6000_Config_C.h
 * @brief Configuration macros for CPU frequency, UART, I2C, and task count.
 *
 * This header defines compile-time constants used throughout the project:
 * - CPU frequency (`F_CPU`)
 * - AVR microcontroller model
 * - Number of FreeRTOS tasks in the system
 */
#ifndef _DRINK_CREATOR6000_CONFIG_C_H_
    #define _DRINK_CREATOR6000_CONFIG_C_H_
/**
 * @def F_CPU
 * @brief CPU clock frequency in Hertz.
 * 
 * Default is 16 MHz (16,000,000 Hz).
 */
#ifndef F_CPU
    #define F_CPU 8000000UL
#endif
/**
 * @def __AVR_ATmega2560__
 * @brief Define to indicate the target MCU is ATmega2560.
 */
#ifndef __AVR_ATmega2560__
    #define __AVR_ATmega2560__ 1
#endif
/**
 * @brief Size of the guard zone (in bytes) placed between task stacks.
 */
#define GUARD_ZONE_SIZE 32
/**
 * @brief Memory fill pattern for guard zones, used to detect overflow.
 */
#define MEMORY_FILL_PATTERN 0xAA



#define TASK_N 12


#endif // _DRINK_CREATOR6000_CONFIG_C_H_