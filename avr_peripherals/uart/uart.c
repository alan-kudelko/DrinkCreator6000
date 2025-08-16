#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#include <DrinkCreator6000_Pins.h>

#include <uart.h>

volatile uint8_t uart_buffer_tx[UART_TX_BUFFER_SIZE]={0};
volatile uart_tx_buf_index_t uart_tx_buffer_head=0;
volatile uart_tx_buf_index_t uart_tx_buffer_tail=0;
volatile uint16_t uart_tx_error_counter=0;

volatile uint8_t uart_buffer_rx[UART_RX_BUFFER_SIZE]={0};
volatile uart_rx_buf_index_t uart_rx_buffer_head=0;
volatile uart_rx_buf_index_t uart_rx_buffer_tail=0;
volatile uint16_t uart_rx_error_counter=0;

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
    // Check if TX buffer is not empty
    if(uart_tx_buffer_head!=uart_tx_buffer_tail){
        // Load next byte to UART data register to send
        UDR0=uart_buffer_tx[uart_tx_buffer_tail];
        // Advance tail index (circular buffer)
        uart_tx_buffer_tail=(uart_tx_buffer_tail+1)%UART_TX_BUFFER_SIZE;
    }
    else{
        // No more data to send, disable UDRE interrupt
        UCSR0B&=~(1<<UDRIE0);
    }
}

void uart_putc_blocking(char c){
    uart_tx_buf_index_t next_head=(uart_tx_buffer_head+1)%UART_TX_BUFFER_SIZE;
    // Wait until there is space in the TX buffer
    while(next_head==uart_tx_buffer_tail){
        // busy wait (blocking)
    }
    cli();
    // Place character in buffer and update head index
    uart_buffer_tx[uart_tx_buffer_head]=c;
    uart_tx_buffer_head=next_head;
    // Enable Data Register Empty Interrupt to start transmission
    UCSR0B|=(1<<UDRIE0);
    sei();
}

void uart_puts_blocking(const char*s){
    // Send characters until null terminator
    while(*s){
        uart_putc_blocking(*s++); // Send single char blocking
    }
}
// NOTE: This function is an exception to MISRA C:2025 Rule 21.17:
// Rule 21.17 restricts pointer operations to avoid accessing memory outside valid bounds.
// Here we rely on the guarantee that all PROGMEM strings are null-terminated,
// so iterating with pgm_read_byte until the null terminator is safe and intentional.
void uart_puts_P_blocking(const char*s){
    // Read first byte from PROGMEM string
    char c=pgm_read_byte(s++);
    // Loop until null terminator
    while(c){
        uart_putc_blocking(c);    // Send single char blocking
        c=pgm_read_byte(s++);   // Read next byte from PROGMEM
    }
}

void uart_putc_non_blocking(char c,int8_t*status){
    uart_tx_buf_index_t next_head=(uart_tx_buffer_head+1)%UART_TX_BUFFER_SIZE;
    // Check if buffer is full, return with status=0 if no space is available
    if(next_head==uart_tx_buffer_tail){
        *status=0;
        return;
    }
    // Copy character into TX buffer and update head index
    uart_buffer_tx[uart_tx_buffer_head]=c;
    uart_tx_buffer_head=next_head;
    // Indicate success by setting status to -1
    (*status)=-1;
    // Enable Data Register Empty interrupt to start/continue transmission
    UCSR0B|=(1<<UDRIE0);
}

void uart_put_hex_blocking(uint8_t val){
    // Lookup table for hexadecimal characters
    const char hex_chars[]="0123456789ABCDEF";
    // Send the high nibble as hex character
    uart_putc_blocking(hex_chars[(val>>4)&0x0F]);
    // Send the low nibble as hex character
    uart_putc_blocking(hex_chars[val&0x0F]);
}

void uart_puts_non_blocking(const char*s,int8_t*status){
    int8_t characterStatus=-1;
    s+=*status; // Continue from last position stored in status
    // Loop through string until null terminator
    while(*s){
        uart_putc_non_blocking(*s++,&characterStatus); // Try to enqueue each char non-blocking
        if(characterStatus==0){
            // Buffer full, stop and return, status holds how many chars were queued so far
            return;
        }
        (*status)++; // Increment count of successfully queued characters
    }
    (*status) = -1; // All characters successfully queued, indicate completion
}

void uart_puts_P_non_blocking(const char*s,int8_t*status){
    int8_t characterStatus=-1;
    s+=*status; // Continue from last position stored in status
    char c=pgm_read_byte(s++); // Read first character from PROGMEM
    // Loop until null terminator (0) is reached
    while(c!=0){
        uart_putc_non_blocking(c,&characterStatus); // Try to enqueue character non-blocking
        if(characterStatus==0){
            // Buffer full, stop and return, status holds how many chars were queued so far
            return;
        }
        c=pgm_read_byte(s++); // Read next character from PROGMEM
        (*status)++; // Increment count of successfully queued characters
    }
    (*status)=-1; // All characters successfully queued, indicate completion
}

ISR(USART0_RX_vect){
    uint8_t data=UDR0; // Read received byte from UART data register
    // Calculate next head index with wrap-around
    uart_rx_buf_index_t next_head=(uart_rx_buffer_head+1)%UART_RX_BUFFER_SIZE;
    // Check for buffer overflow
    if(next_head!=uart_rx_buffer_tail){
        uart_buffer_rx[uart_rx_buffer_head]=data; // Store received byte in RX buffer
        uart_rx_buffer_head=next_head;             // Advance head index
    }
    else{
        uart_rx_error_counter++; // RX buffer full, increment overflow error counter
    }
}

int16_t uart_getc(void){
    // Check if RX buffer is empty
    if(uart_rx_buffer_head==uart_rx_buffer_tail){
        return -1;  // No data available, return -1
    }
    else{
        // Read next byte from RX buffer
        uint8_t data=uart_buffer_rx[uart_rx_buffer_tail];
        // Advance tail index with wrap-around
        uart_rx_buffer_tail=(uart_rx_buffer_tail+1)%UART_RX_BUFFER_SIZE;
        // Return received byte as int16_t (0–255)
        return data;
    }
}

int16_t uart_peekc(void){
    // Check if RX buffer is empty
    if(uart_rx_buffer_head==uart_rx_buffer_tail){
        return -1;  // No data available, return -1
    }
    else{
        // Peek next byte from RX buffer without removing it
        uint8_t data=uart_buffer_rx[uart_rx_buffer_tail];
        // Return the peeked byte as int16_t (0–255)
        return data;
    }
}

uint16_t uart_rx_error_count(void){
    // Return the current count of UART receive errors (e.g., buffer overflows, framing errors)
    return uart_rx_error_counter;
}
