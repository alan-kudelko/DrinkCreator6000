#include <uart.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <DrinkCreator6000_Pins.h>


volatile uint8_t uart_buffer_tx[UART_TX_BUFFER_SIZE]={0};
volatile uint8_t uart_tx_buffer_head=0;
volatile uint8_t uart_tx_buffer_tail=0;
volatile uint8_t uart_tx_error_counter=0;

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

ISR(USART0_UDRE_vect){
    if(uart_tx_buffer_head!=uart_tx_buffer_tail){
        UDR0=uart_buffer_tx[uart_tx_buffer_tail];
        uart_tx_buffer_tail=(uart_tx_buffer_tail+1)%UART_TX_BUFFER_SIZE;
    }
    else{
        UCSR0B&=~(1<<UDRIE0);
    }
}

int16_t uart_putc(char c){
    uint16_t copiedCharacters=-1;

    uint8_t next_head=(uart_tx_buffer_head+1)%UART_TX_BUFFER_SIZE;

    if(next_head==uart_tx_buffer_tail){
        copiedCharacters=0;
        return copiedCharacters; // 0 if there is no space in the buffer
    }

    uart_buffer_tx[uart_tx_buffer_head]=c;
    uart_tx_buffer_head=next_head;

    UCSR0B|=(1<<UDRIE0); // Enable interrupts - data registers empty

    return copiedCharacters; // -1 if character is copied
}

int16_t uart_puts(const char*s){
    int16_t copiedCharacters=0;
    while(*s){
		if(uart_putc(*s++)>=0){
            return copiedCharacters;
        }

        copiedCharacters++;
    }
    copiedCharacters=-1;
    return copiedCharacters;
}

int16_t uart_puts_P(const char*s){
    int16_t copiedCharacters=0;

    char c=pgm_read_byte(s++);

    while(c!=0){
        if(uart_putc(c)>=0){
            return copiedCharacters;
        }
        c=pgm_read_byte(s++);
        copiedCharacters++;
    }
    copiedCharacters=-1;
    return copiedCharacters;
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