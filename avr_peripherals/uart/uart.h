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
 * 
 * @note This driver uses UART0 (USART0) peripheral on the ATmega2560, which has four UART interfaces.
 */
#ifndef _UART_H_
    #define _UART_H_

#include <stdint.h>
#include <DrinkCreator6000_Config_C.h>

/**
 * @def UART_BAUD_RATE
 * @brief UART communication baud rate in bits per second.
 *
 * Default is 9600 bps.
 */
#define UART_BAUD_RATE       9600
/**
 * @def UBRR_VALUE
 * @brief UART Baud Rate Register value calculated from F_CPU and UART_BAUD_RATE.
 *
 * Used to configure the UART baud rate generator.
 */
#define UBRR_VALUE           ((F_CPU/(16UL*UART_BAUD_RATE))-1)
/**
 * @def UART_TX_BUFFER_SIZE
 * @brief Size of the UART transmit buffer, in bytes.
 */
#define UART_TX_BUFFER_SIZE 96
/**
 * @def UART_RX_BUFFER_SIZE
 * @brief Size of the UART receive buffer, in bytes.
 */
#define UART_RX_BUFFER_SIZE 96
/**
 * @typedef uart_tx_buf_index_t
 * @brief Index type for TX buffer positions.
 *
 * Uses uint8_t if TX buffer size is 255 bytes or less, otherwise uint16_t.
 */
#if(UART_TX_BUFFER_SIZE<=255)
    typedef uint8_t uart_tx_buf_index_t;
#else
    typedef uint16_t uart_tx_buf_index_t;
#endif
/**
 * @typedef uart_rx_buf_index_t
 * @brief Index type for RX buffer positions.
 *
 * Uses uint8_t if RX buffer size is 255 bytes or less, otherwise uint16_t.
 */
#if(UART_RX_BUFFER_SIZE<=255)
    typedef uint8_t uart_rx_buf_index_t;
#else
    typedef uint16_t uart_rx_buf_index_t;
#endif
/// Return code indicating UART buffer is full.
#define UART_BUFFER_FULL    1
/// Return code indicating UART buffer is empty.
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
 * @brief Head index for TX buffer (write position).
 */
extern volatile uart_tx_buf_index_t uart_tx_buffer_head;
/**
 * @brief Tail index for TX buffer (read position).
 */
extern volatile uart_tx_buf_index_t uart_tx_buffer_tail;
/**
 * @brief Receive ring buffer.
 */
extern volatile uint8_t uart_buffer_rx[UART_RX_BUFFER_SIZE];
/**
 * @brief Head index for RX buffer (write position).
 */
extern volatile uart_rx_buf_index_t uart_rx_buffer_head;
/**
 * @brief Tail index for RX buffer (read position).
 */
extern volatile uart_rx_buf_index_t uart_rx_buffer_tail;
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
 * This function blocks until there is space available in the transmit buffer.
 *
 * @param c Character to send.
 */
void uart_putc_blocking(char c);
/**
 * @brief Sends a single byte as a hexadecimal string over UART (blocking).
 *
 * @param val Byte value to send as hexadecimal.
 */
void uart_put_hex_blocking(uint8_t val);
/**
 * @brief Sends a null-terminated string from RAM over UART (blocking).
 *
 * This function blocks until the entire string is queued for transmission.
 *
 * @param s Pointer to a null-terminated string located in RAM.
 */
void uart_puts_blocking(const char*s);
/**
 * @brief Sends a null-terminated string stored in PROGMEM over UART (blocking).
 *
 * This function blocks until the entire string is queued for transmission.
 *
 * @param s Pointer to a null-terminated string stored in program memory (PROGMEM).
 */
void uart_puts_P_blocking(const char*s);
// ============================
// Non-blocking UART Functions
// ============================
/**
 * @brief Attempts to enqueue a character for UART transmission (non-blocking).
 *
 * @param c Character to send.
 * @param status Pointer to a status variable:
 *  - `-1` if the character was accepted,
 *  - `0` if the transmit buffer was full and the character was not sent.
 */
void uart_putc_non_blocking(char c,int8_t*status);
/**
 * @brief Attempts to enqueue a null-terminated string from RAM for UART transmission (non-blocking).
 *
 * Transmits as many characters as possible until the transmit buffer is full.
 *
 * @param s Pointer to a null-terminated string located in RAM.
 * @param status Pointer to a status variable:
 *  - `-1` if the entire string was queued,
 *  - `>= 0` number of characters successfully queued before the buffer became full.
 */
void uart_puts_non_blocking(const char*s,int8_t*status);
/**
 * @brief Attempts to enqueue a null-terminated string stored in PROGMEM for UART transmission (non-blocking).
 *
 * Transmits as many characters as possible until the transmit buffer is full.
 *
 * @param s Pointer to a null-terminated string stored in program memory (PROGMEM).
 * @param status Pointer to a status variable:
 *  - `-1` if the entire string was queued,
 *  - `>= 0` number of characters successfully queued before the buffer became full.
 */
void uart_puts_P_non_blocking(const char*s,int8_t*status);
// ========================
// Receive Functions
// ========================
/**
 * @brief Retrieves the next character from the receive buffer.
 *
 * @return The received character (0–255) or `-1` if the receive buffer is empty.
 */
int16_t uart_getc(void);
/**
 * @brief Peeks at the next character in the receive buffer without removing it.
 *
 * @return The next character in the buffer or `-1` if the buffer is empty.
 */
int16_t uart_peekc(void);
/**
 * @brief Returns the number of receive errors encountered (e.g., buffer overflows or framing errors).
 *
 * @return The current receive error counter value.
 */
uint16_t uart_rx_error_count(void);

#ifdef __cplusplus
    }
#endif // __cplusplus

#endif // _UART_H_