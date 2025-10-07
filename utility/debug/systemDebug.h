/**
 * @file systemDebug.h
 * @brief Diagnostics and debug utilities for DrinkCreator6000.
 *
 * This module provides functions to dump system memory usage, last bootup
 * counter, last system errors, and I2C status via UART. It is designed
 * for use in debugging and system monitoring tasks.
 *
 * @note All functions use blocking UART calls for simplicity.
 * @note Buffer sizes and formatting are defined via macros in this header.
 */

#ifndef SYSTEM_DEBUG_H_
	#define SYSTEM_DEBUG_H_

#include <DrinkCreator6000_DataTypes.h>

#define LAST_BOOTUP_BUFFER_SIZE 6 /**< Buffer size for last bootup dump */

#define LAST_ERROR_BUFFER_SIZE 51 /**< Buffer size for last error dump */

#define RAM_DUMP_BUFFER_LENGTH 5   /**< Length of the buffer used for formatting memory addresses and sizes */

#define RAM_DUMP_ADDR_FMT      "%04X" /**< Format string for printing 16-bit memory addresses in hexadecimal */

#define RAM_DUMP_SIZE_FMT      "%4d"  /**< Format string for printing memory sizes in decimal */

#ifdef __cplusplus
	extern "C"{
#endif // __cplusplus

/**
 * @brief Dumps the last bootup counter value via UART.
 *
 * Prints the last bootup number with a fixed-width format and surrounds it with
 * a hash marker for easier identification.
 *
 * @param bootup Pointer to the last bootup counter value to print.
 *
 * @details The function formats the value using snprintf for safe buffer usage
 *          and sends the output using blocking UART functions.
 */
extern void lastBootup_dump(const uint16_t*bootup);

/**
 * @brief Dumps the last system error via UART.
 *
 * Prints the error message and the uptime at which the error occurred.
 *
 * @param lastError Pointer to a sSystemError structure containing:
 *                  - errorText: description of the error
 *                  - days, hours, minutes, seconds: uptime until the failure
 *
 * @details The function formats the output using snprintf with bounds checking
 *          and sends it using blocking UART functions. The output is wrapped
 *          with X markers and header/footer messages for clarity.
 */
extern void lastError_dump(const struct sSystemError*lastError);

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
extern void ram_dump(void);

/**
 * @brief Dumps information about all FreeRTOS tasks via UART.
 *
 * @details
 * Iterates over all tasks in the system and prints their key information:
 * - Task name (padded to configMAX_TASK_NAME_LEN)
 * - High water mark of the stack (usStackHighWaterMark)
 * - Current state (Ready, Running, Blocked, Suspended, Deleted, or Invalid)
 * - Task priority (uxCurrentPriority)
 * - Task index in the task array
 *
 * Each task's information is formatted in a human-readable way and sent
 * using blocking UART functions.
 *
 * @note Padding and formatting use temporary buffers on the stack.
 * @note MISRA C considerations: snprintf is used with bounds checking to
 *       avoid buffer overflows.
 * @note This function only prints information for tasks whose handles
 *       are not NULL.
 */
extern void task_dump(void);

/**
 * @brief Prints the current status of the I2C/TWI driver via UART.
 *
 * @details
 * This function provides diagnostic information about the I2C subsystem.
 * It prints the following information to UART using blocking functions:
 * - Ring buffer state (if USE_RING_BUFFER_FOR_BLOCKING_OPERATIONS is enabled)
 *   - whether the TX buffer is empty
 *   - head and tail indexes of the TX buffer
 *   - current FSM/I2C state
 * - Counters of transmission errors stored in I2C_ErrorCounters_t:
 *   - bus errors
 *   - NACK on address
 *   - NACK on data
 *   - arbitration lost
 *   - unexpected state occurrences
 *
 * @note Uses blocking UART functions (uart_puts_blocking, uart_put_hex_blocking, uart_putc_blocking)
 *       so calling this in a time-critical context may introduce latency.
 * @note The function is controlled by the macro USE_RING_BUFFER_FOR_BLOCKING_OPERATIONS:
 *       if it is set to 1, the ring buffer information is printed; otherwise,
 *       only the error counters are shown.
 */
extern void printI2C_status(void);

#ifdef __cplusplus
	}
#endif // __cplusplus
	
#endif // SYSTEM_DEBUG_H_