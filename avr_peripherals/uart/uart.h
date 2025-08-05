#ifndef _UART_H_
#define _UART_H_

#include <stdint.h>

#define UART_TX_BUFFER_SIZE 64
#define UART_RX_BUFFER_SIZE 64

#ifdef __cplusplus
extern "C" {
#endif

extern uint8_t uart_buffer_rx[UART_RX_BUFFER_SIZE];

void uart_init(void);

void uart_putc(char c);

void uart_puts(const char* s);

#ifdef __cplusplus
}
#endif

#endif // _UART_H_