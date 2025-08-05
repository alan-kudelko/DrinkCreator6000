#include <uart.h>
#include <DrinkCreator6000_Config_C.h>
#include <avr/io.h>
#include <string.h>

uint8_t uart_buffer_rx[UART_RX_BUFFER_SIZE];

void uart_init(void){
    memset((void*)uart_buffer_rx,0,sizeof(uart_buffer_rx));
    // Setting up baud rate in UBBR register
    UBRR0H=(uint8_t)(UBRR_VALUE>>8);
    UBRR0L=(uint8_t)UBRR_VALUE;
    // Setting up frame format: 8 data bits, no parity, 1 stop bit and enable receiver and transmitter
    UCSR0B=(1<<TXEN0)|(1<<RXEN0);
    UCSR0C=(1<<UCSZ01)|(1<<UCSZ00);
    // Enable RX interrupt
    UCSR0B|=(1<<RXCIE0); 
}

void uart_putc(char c) {
    while(!(UCSR0A&(1<<UDRE0)));
	
    UDR0=c;
}

void uart_puts(const char*s) {
    while(*s)
		uart_putc(*s++);
}