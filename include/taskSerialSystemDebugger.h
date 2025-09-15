/**
 * @file taskSerialSystemDebugger.h
 * @brief Task and utilities for serial system debugging and RAM usage reporting.
 *
 * This header provides declarations for functions and macros used to monitor
 * and report memory usage on the AVR-based system. It includes utilities
 * to dump memory sections (.data, .bss, .tdat, heap, stack) via UART.
 *
 * The serial system debugger task can periodically send RAM usage information
 * or respond to on-demand requests, facilitating debugging and performance
 * monitoring of the embedded system.
 *
 * @note All UART output is currently blocking.
 * @note Memory boundaries and sizes are reported in hexadecimal and decimal formats
 *       using macros for formatting.
 */

#ifndef TASK_SERIAL_SYSTEM_DEBUGGER_H_
    #define TASK_SERIAL_SYSTEM_DEBUGGER_H_

/**
 * @brief Length of the buffer used for formatting memory addresses and sizes.
 */    
#define RAM_DUMP_BUFFER_LENGTH 5
/**
 * @brief Format string for printing 16-bit memory addresses in hexadecimal.
 */
#define RAM_DUMP_ADDR_FMT      "%04X"
/**
 * @brief Format string for printing memory sizes in decimal.
 */
#define RAM_DUMP_SIZE_FMT      "%4d"

#ifdef __cplusplus
    extern "C"{
#endif // __cplusplus

/**
 * @brief Updates internal memory usage counters.
 *
 * This function scans current allocations and updates counters used by the
 * RAM dump routine.
 *
 * @note Compliant with MISRA C:2025 – explicit void in prototype.
 */
void updateMemoryUsage(void);

/**
 * @brief Dumps memory usage via UART.
 *
 * Prints boundaries and sizes of memory sections:
 * - .data
 * - .bss
 * - .tdat
 * - heap
 * - stack
 *
 * @details Each section's start and end addresses are printed in hexadecimal,
 *          and the size is printed in decimal. The output is sent using
 *          blocking UART functions.
 *
 * @note This function relies on const PROGMEM strings for headers, footers,
 *       and separators.
 * @note Buffer length and formatting are controlled by RAM_DUMP_BUFFER_LENGTH,
 *       RAM_DUMP_ADDR_FMT, and RAM_DUMP_SIZE_FMT macros.
 */
void ram_dump(void);

/**
 * @brief FreeRTOS task for serial system debugging.
 *
 * This task can periodically output system RAM usage information over UART
 * or respond to requests from other tasks or commands.
 *
 * @param pvParameters Pointer to FreeRTOS task parameters (unused).
 */
void taskSerialSystemDebugger(void*pvParameters);

#ifdef __cplusplus
    }
#endif // __cplusplus

#endif // TASK_SERIAL_SYSTEM_DEBUGGER_H_
