#include <uart.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <DrinkCreator6000_Pins.h>

volatile uint8_t uart_buffer_rx[UART_RX_BUFFER_SIZE]={0};
volatile uint8_t uart_rx_buffer_head=0;
volatile uint8_t uart_rx_buffer_tail=0;
volatile uint8_t uart_rx_error_counter=0;

void uart_init(void){
    // Setting up TX and RX pins
    DDRE&=~(1<<UART0_RX_PIN);
    DDRE|=(1<<UART0_TX_PIN);
    // Setting up baud rate in UBBR register
    UBRR0H=(uint8_t)(UBRR_VALUE>>8);
    UBRR0L=(uint8_t)UBRR_VALUE;
    // Setting up frame format: 8 data bits, no parity, 1 stop bit and enable receiver and transmitter
    UCSR0B=(1<<TXEN0)|(1<<RXEN0);
    UCSR0C=(1<<UCSZ01)|(1<<UCSZ00);
    // Enable RX interrupt
    UCSR0B|=(1<<RXCIE0); 
}

void uart_putc(char c){
    while(!(UCSR0A&(1<<UDRE0)));
	
    UDR0=c;
}

void uart_puts(const char*s){
    while(*s)
		uart_putc(*s++);
}

void uart_puts_P(const char*s){
    char c=pgm_read_byte(s++);
    while(c!=0){
        uart_putc(c);
        c=pgm_read_byte(s++);
    }
}

ISR(USART0_RX_vect){
    uint8_t data=UDR0; // Read received data
    uint8_t next_head=(uart_rx_buffer_head+1)%UART_RX_BUFFER_SIZE; // Calculate next head position

    if(next_head!=uart_rx_buffer_tail){
        uart_buffer_rx[uart_rx_buffer_head]=data; // Store data in buffer
        uart_rx_buffer_head=next_head; // Update head position
    }
    else{
        uart_rx_error_counter++; // Buffer overflow, increment error counter
    }
}

int16_t uart_getc(void){
    if(uart_rx_buffer_head==uart_rx_buffer_tail){ // Check if buffer is empty
        return -1;  // Buffer is empty
    }
    else{
        uint8_t data=uart_buffer_rx[uart_rx_buffer_tail]; // Read data from buffer
        uart_rx_buffer_tail=(uart_rx_buffer_tail+1)%UART_RX_BUFFER_SIZE; // Update tail position
        return data;
    }
}

int16_t uart_peekc(void){
    if(uart_rx_buffer_head==uart_rx_buffer_tail){ // Check if buffer is empty
        return -1;  // Buffer is empty
    }
    else{
        uint8_t data=uart_buffer_rx[uart_rx_buffer_tail]; // Peek data from buffer
        return data; // Return the data without removing it from the buffer
    }
}

uint8_t uart_rx_error_count(void){
    return uart_rx_error_counter; // Return the number of errors
}