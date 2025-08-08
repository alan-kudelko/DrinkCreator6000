/**
 * @file uart.h
 * @brief Low-level UART driver for AVR (ATmega2560) with TX/RX buffering.
 *
 * This module provides blocking and non-blocking UART routines with transmit
 * and receive ring buffers. It is hardware-level and **independent of any RTOS**.
 *
 * Two system-level ISRs (`USART_UDRE_vect` and `USART_RX_vect`) are defined internally in `uart.c`.
 *
 * @note TX and RX buffer sizes are configurable via macros.
 * @note Use `uart_puts_P_*` variants for strings stored in flash memory (PROGMEM).
 */

#ifndef _UART_H_
    #define _UART_H_

#include <stdint.h>
#include <avr/pgmspace.h>
#include <DrinkCreator6000_Config_C.h>

/// TX buffer size in bytes.
#define UART_TX_BUFFER_SIZE 10

/// RX buffer size in bytes.
#define UART_RX_BUFFER_SIZE 96

/// Return code: UART buffer is full.
#define UART_BUFFER_FULL    1

/// Return code: UART buffer is empty.
#define UART_BUFFER_EMPTY   0

#ifdef __cplusplus
    extern "C" {
#endif

// ========================
// Global Buffer Variables
// ========================

/**
 * @brief Transmit ring buffer.
 */
extern volatile uint8_t uart_buffer_tx[UART_TX_BUFFER_SIZE];

/**
 * @brief Transmit buffer head index (write position).
 */
extern volatile uint8_t uart_tx_buffer_head;

/**
 * @brief Transmit buffer tail index (read position).
 */
extern volatile uint8_t uart_tx_buffer_tail;

/**
 * @brief Transmit error counter (e.g., overflow).
 */
extern volatile uint16_t uart_tx_error_counter;

/**
 * @brief Receive ring buffer.
 */
extern volatile uint8_t uart_buffer_rx[UART_RX_BUFFER_SIZE];

/**
 * @brief Receive buffer head index (write position).
 */
extern volatile uint8_t uart_rx_buffer_head;

/**
 * @brief Receive buffer tail index (read position).
 */
extern volatile uint8_t uart_rx_buffer_tail;

/**
 * @brief Receive error counter (e.g., overflow or framing errors).
 */
extern volatile uint16_t uart_rx_error_counter;

// ========================
// Core UART Functions
// ========================

/**
 * @brief Initializes the UART peripheral and enables interrupts.
 *
 * Must be called before any other UART operation.
 */
void uart_init(void);

/**
 * @brief Sends a single character over UART (blocking).
 *
 * Waits until space is available in the transmit buffer.
 *
 * @param c Character to send.
 */
void uart_putc_blocking(char c);

/**
 * @brief Sends a null-terminated string from RAM over UART (blocking).
 *
 * Blocks until the entire string is queued for transmission.
 *
 * @param s Pointer to null-terminated string in RAM.
 */
void uart_puts_blocking(const char*s);

/**
 * @brief Sends a null-terminated string from PROGMEM over UART (blocking).
 *
 * Blocks until the entire string is queued for transmission.
 *
 * @param s Pointer to null-terminated string in PROGMEM.
 */
void uart_puts_P_blocking(const char*s);

// ============================
// Non-blocking UART Functions
// ============================

/**
 * @brief Attempts to enqueue a character for UART transmission (non-blocking).
 *
 * @param c Character to send.
 * @param status Pointer to status variable:
 *  - `-1` if success (character accepted),
 *  - `0` if buffer full (character not sent).
 */
void uart_putc_non_blocking(char c,int8_t*status);

/**
 * @brief Attempts to enqueue a null-terminated string from RAM (non-blocking).
 *
 * Transmits as many characters as possible until buffer is full.
 *
 * @param s Pointer to null-terminated string in RAM.
 * @param status Pointer to status variable:
 *  - `-1` if entire string queued,
 *  - `>= 0` number of characters successfully queued before buffer filled.
 */
void uart_puts_non_blocking(const char*s,int8_t*status);

/**
 * @brief Attempts to enqueue a null-terminated string from PROGMEM (non-blocking).
 *
 * Transmits as many characters as possible until buffer is full.
 *
 * @param s Pointer to null-terminated string in PROGMEM.
 * @param status Pointer to status variable:
 *  - `-1` if entire string queued,
 *  - `>= 0` number of characters successfully queued before buffer filled.
 */
void uart_puts_P_non_blocking(const char*s,int8_t*status);

// ========================
// Receive Functions
// ========================

/**
 * @brief Retrieves the next character from the RX buffer.
 *
 * @return Received character (0–255) or `-1` if buffer is empty.
 */
int16_t uart_getc(void);

/**
 * @brief Peeks at the next character in the RX buffer without removing it.
 *
 * @return Next character or `-1` if buffer is empty.
 */
int16_t uart_peekc(void);

/**
 * @brief Returns the number of receive errors encountered (e.g., buffer overflows).
 *
 * @return Error counter value.
 */
uint16_t uart_rx_error_count(void);

#ifdef __cplusplus
    }
#endif

#endif // _UART_H_