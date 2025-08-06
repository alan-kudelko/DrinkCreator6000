#ifndef _UART_H_
    #define _UART_H_

#include <stdint.h>
#include <avr/pgmspace.h>

#include <DrinkCreator6000_Config_C.h>

#define UART_RX_BUFFER_SIZE 96
#define UART_TX_BUFFER_SIZE 10

#define UART_BUFFER_FULL    1
#define UART_BUFFER_EMPTY   0


#ifdef __cplusplus
    extern "C" {
#endif

extern volatile uint8_t uart_buffer_tx[UART_TX_BUFFER_SIZE];
extern volatile uint8_t uart_tx_buffer_head;
extern volatile uint8_t uart_tx_buffer_tail;
extern volatile uint8_t uart_tx_error_counter;

extern volatile uint8_t uart_buffer_rx[UART_RX_BUFFER_SIZE];
extern volatile uint8_t uart_rx_buffer_head;
extern volatile uint8_t uart_rx_buffer_tail;
extern volatile uint8_t uart_rx_error_counter;

void uart_init(void);

// Note: This module defines ISR(USART_UDRE_vect) internally in uart.c

int16_t  uart_putc(char c);

int16_t  uart_puts(const char*s);

int16_t  uart_puts_P(const char*s);

// Note: This module defines ISR(USART_RX_vect) internally in uart.c

int16_t uart_getc(void);

int16_t uart_peekc(void);

uint8_t uart_rx_error_count(void);

#ifdef __cplusplus
    }
#endif

#endif // _UART_H_