#include <i2c.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <DrinkCreator6000_Pins.h>
#include <uart.h>

#if USE_RING_BUFFER_FOR_BLOCKING_OPERATIONS==0

#warning "Blocking operations bypass the ring buffer (no buffering enabled)"

#else

#warning "Timer4 used exclusively for I2C management"

#endif // USE_RING_BUFFER_FOR_BLOCKING_OPERATIONS

volatile struct I2C_DATA i2c_buffer_tx[I2C_TX_BUFFER_SIZE]={0};
volatile uint8_t i2c_tx_buffer_head=0;
volatile uint8_t i2c_tx_buffer_tail=0;
volatile uint8_t i2c_status=I2C_STATE_IDLE;
volatile uint16_t i2c_tx_error_counter=0;

void i2c_init(void){
    // Setup SDA and SCL pins as inputs
    DDRD&=~(1<<I2C_SCL_PIN);
    DDRD&=~(1<<I2C_SDA_PIN);
    // Timer 4 used as clock for driving I2C after sending STOP
#if USE_RING_BUFFER_FOR_BLOCKING_OPERATIONS==1
    TCCR4A = 0;                    // Normal mode (timer counts up to OCR4A)
    TCCR4B = 0;
    TCNT4 = 0;                    // Reset timer counter to zero
    OCR4A = 2;                    // Compare value for ~10 µs interrupt (prescaler 64, 16 MHz clock)
                                  // 16 MHz / 64 = 250 kHz → 4 µs per tick
                                  // OCR4A=2 → 3 ticks × 4 µs = 12 µs
    TCCR4B |= (1 << WGM42);       // Enable CTC mode (Clear Timer on Compare Match)
    TCCR4B |= (1 << CS42) | (1 << CS40); // Set prescaler to 64 (timer clock = 250 kHz)
    TIMSK4 |= (1 << OCIE4A);      // Enable Timer4 Compare A Match interrupt
#endif
    // Configure TWI (I2C) bit rate and enable peripheral
    TWSR&=~((1<<TWPS0)|(1<<TWPS1)); // Set prescaler bits to 0 (prescaler = 1)
    TWBR=(uint8_t)TWBR_VALUE;       // Set bit rate register (depends on desired speed)
    TWCR=(1<<TWEN);                 // Enable TWI (I2C) hardware

    //Debug for measuring time in ISR
    DDRH|=(1<<PH4);
    PORTH|=(1<<PH4);
}

void i2c_disable(void){
    TWCR&=~(1<<TWEN); // Disable TWI (I2C) peripheral
}

uint8_t i2c_get_status(void){
    return i2c_status; // Return current I2C status code
}

void i2c_tx_buffer_clear_until_next_address(void){
    uint8_t initialTail=i2c_tx_buffer_tail;
    do{
        // Stop clearing if next address package found (and not at initial position)
        if((i2c_buffer_tx[i2c_tx_buffer_tail].f_Type==ADDRESS_PACKAGE)&&
            (i2c_tx_buffer_tail!=initialTail)){
            break;
        }
        // Advance tail pointer to clear current data
        i2c_tx_buffer_tail=(i2c_tx_buffer_tail+1)%I2C_TX_BUFFER_SIZE;
        // This clears data from the buffer until the next address package is found
        // Useful when a slave fails to respond, allowing next transmission to proceed
    }while(i2c_tx_buffer_tail!=i2c_tx_buffer_head);
}

void i2c_tx_buffer_show_contet(void){

}

#if USE_RING_BUFFER_FOR_BLOCKING_OPERATIONS==1
// ISR for initializing I2C transmission on Timer4 Compare A interrupt
// Triggered after sending STOP or when first package is queued
ISR(TIMER4_COMPA_vect){
    // Start I2C transmission only if state is IDLE and TX buffer is not empty
    if((i2c_status==I2C_STATE_IDLE)&&(i2c_tx_buffer_is_empty()==0)){
        // Send START condition and enable TWI and TWI interrupt
        TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN)|(1<<TWIE);
        TIMSK4&=~(1<<OCIE4A); // Disable Timer4 Compare A interrupt
        TCNT4=0;              // Reset timer counter
    }
}

ISR(TWI_vect){
    PORTH&=~(1<<PH4);
    uint8_t status=TWSR&TW_STATUS_MASK;

    switch(status){
        case TW_START:
        case TW_REP_START:
            i2c_status=I2C_STATE_START_SENT;
        break;
        case TW_MT_SLA_ACK:
            i2c_status=I2C_STATE_ADDRESS_SENT;
        break;
        case TW_MT_SLA_NACK:
        case TW_MT_DATA_NACK:
           // i2c_status=I2C_STATE_ERROR;
            //i2c_tx_error_counter++;
        //break;
        case TW_MT_DATA_ACK:
            i2c_status=I2C_STATE_DATA_SENT;
        break; 
        //default:
        //    i2c_status=I2C_STATE_ERROR;
        //    i2c_tx_error_counter++;
       // break;
    }
    if(i2c_tx_buffer_head==i2c_tx_buffer_tail){
        TWCR=(1<<TWEN)|(1<<TWINT)|(1<<TWSTO);
        i2c_status=I2C_STATE_IDLE;
        TIMSK4|=(1<<OCIE4A);
        return;
    }

    // Changed to FSM
    switch(i2c_status){
        case I2C_STATE_START_SENT:
            // Wysyłamy adres z bufora (tail)
            if(i2c_buffer_tx[i2c_tx_buffer_tail].f_Type==ADDRESS_PACKAGE){
                TWDR=i2c_buffer_tx[i2c_tx_buffer_tail].value;
                TWCR=(1<<TWIE)|(1<<TWEN)|(1<<TWINT);
            }
            else{
                i2c_tx_buffer_clear_until_next_address();
                TWCR=(1<<TWIE)|(1<<TWEN)|(1<<TWINT)|(1<<TWSTO);
                i2c_status=I2C_STATE_IDLE;
                TIMSK4|=(1<<OCIE4A);
            }
        break;
        case I2C_STATE_ADDRESS_SENT:
            i2c_tx_buffer_tail=(i2c_tx_buffer_tail+1)%I2C_TX_BUFFER_SIZE;
            if(i2c_buffer_tx[i2c_tx_buffer_tail].f_Type==DATA_PACKAGE){
                TWDR=i2c_buffer_tx[i2c_tx_buffer_tail].value;
                TWCR=(1<<TWIE)|(1<<TWEN)|(1<<TWINT);
            }
            else{
                i2c_tx_buffer_clear_until_next_address();
                TWCR=(1<<TWIE)|(1<<TWEN)|(1<<TWINT)|(1<<TWSTO);
                i2c_status=I2C_STATE_IDLE;
                TIMSK4|=(1<<OCIE4A);
            }
        break;
        case I2C_STATE_DATA_SENT:
            // Sprawdzamy czy ta paczka była ostatnia
            // Jeżeli tak to wysyłamy stop, jak nie
            // wysyłamy dane dalej
            if(i2c_buffer_tx[i2c_tx_buffer_tail].f_LP==LAST_PACKAGE){
                TWCR=(1<<TWIE)|(1<<TWEN)|(1<<TWINT)|(1<<TWSTO);
                i2c_status=I2C_STATE_IDLE;
                i2c_tx_buffer_tail=(i2c_tx_buffer_tail+1)%I2C_TX_BUFFER_SIZE;
                TIMSK4|=(1<<OCIE4A);
            }
            else{
                i2c_tx_buffer_tail=(i2c_tx_buffer_tail+1)%I2C_TX_BUFFER_SIZE;
                // Jeśli jest jeszcze paczka w buforze
                if(i2c_buffer_tx[i2c_tx_buffer_tail].f_Type==ADDRESS_PACKAGE){
                    // Obsluz blad
                    // Przejdz w idle
                    // zakoncz transmisje
                    TWCR=(1<<TWIE)|(1<<TWEN)|(1<<TWINT)|(1<<TWSTO);
                    i2c_status=I2C_STATE_IDLE;
                }
                else{
                    // Jezeli to dalej dane
                    TWDR=i2c_buffer_tx[i2c_tx_buffer_tail].value;
                    TWCR=(1<<TWIE)|(1<<TWEN)|(1<<TWINT);
                }
            }
        break;
        case I2C_STATE_ERROR:
            // Czyścimy bufor i przechodzimy do IDLE
            TWCR=(1<<TWIE)|(1<<TWEN)|(1<<TWINT)|(1<<TWSTO);
            i2c_tx_buffer_clear_until_next_address();
            i2c_status=I2C_STATE_IDLE;
            TIMSK4|=(1<<OCIE4A);
            // Wyłączamy TWI lub restartujemy jak trzeba
        break;
    }
    PORTH|=(1<<PH4);
}

#endif // USE_RING_BUFFER_FOR_BLOCKING_OPERATIONS

void i2c_write_byte_blocking(uint8_t address,uint8_t data){
#if USE_RING_BUFFER_FOR_BLOCKING_OPERATIONS==1
    uint8_t new_head=(i2c_tx_buffer_head+2)%I2C_TX_BUFFER_SIZE;
    // Wait until there is space for two elements in the TX buffer
    while (new_head==i2c_tx_buffer_tail){
        // Busy wait
    }
cli(); // Disable interrupts to ensure atomic buffer update
    // Enqueue address package with write flag
    i2c_tx_buffer_head=(i2c_tx_buffer_head+1)%I2C_TX_BUFFER_SIZE;
    i2c_buffer_tx[i2c_tx_buffer_head].value=(address<<1)|WRITE_FLAG;
    i2c_buffer_tx[i2c_tx_buffer_head].f_RW=WRITE_FLAG;
    i2c_buffer_tx[i2c_tx_buffer_head].f_LP=NOT_LAST_PACKAGE;
    i2c_buffer_tx[i2c_tx_buffer_head].f_Type=ADDRESS_PACKAGE;
    // Enqueue data package and mark as last package
    i2c_tx_buffer_head=(i2c_tx_buffer_head+1)%I2C_TX_BUFFER_SIZE;
    i2c_buffer_tx[i2c_tx_buffer_head].value=data;
    i2c_buffer_tx[i2c_tx_buffer_head].f_RW=WRITE_FLAG;
    i2c_buffer_tx[i2c_tx_buffer_head].f_LP=LAST_PACKAGE;
    i2c_buffer_tx[i2c_tx_buffer_head].f_Type=DATA_PACKAGE;
sei(); // Re-enable interrupts

#else
    #define I2C_TIMEOUT 10000
    uint16_t timeout;
    // START condition
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    timeout = I2C_TIMEOUT;
    while (!(TWCR & (1 << TWINT))) {
        if (--timeout == 0) return; // timeout error
    }
    if ((TWSR & 0xF8) != TW_START) return; // error: start failed

    // Send SLA+W (address + write)
    TWDR = (address << 1) | WRITE_FLAG;
    TWCR = (1 << TWINT) | (1 << TWEN);
    timeout = I2C_TIMEOUT;
    while (!(TWCR & (1 << TWINT))) {
        if (--timeout == 0) return; // timeout error
    }
    if ((TWSR & 0xF8) != TW_MT_SLA_ACK) return; // error: SLA+W not acked

    // Send data byte
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    timeout = I2C_TIMEOUT;
    while (!(TWCR & (1 << TWINT))) {
        if (--timeout == 0) return; // timeout error
    }
    if ((TWSR & 0xF8) != TW_MT_DATA_ACK) return; // error: data not acked

    // STOP condition
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);

#endif // USE_RING_BUFFER_FOR_BLOCKING_OPERATIONS
}

void i2c_write_bytes_blocking(uint8_t address,uint8_t*data,uint8_t length){
    uint8_t new_head=(i2c_tx_buffer_head+length+1)%I2C_TX_BUFFER_SIZE;
    uint8_t i;
    while(new_head==i2c_tx_buffer_tail){
        // Wait for enough space in the buffer
        uart_putc_blocking('c');
        uart_putc_blocking('\n');
    }
cli();
    i2c_tx_buffer_head=(i2c_tx_buffer_head+1)%I2C_TX_BUFFER_SIZE;

    i2c_buffer_tx[i2c_tx_buffer_head].value=(address<<1)|WRITE_FLAG;
    i2c_buffer_tx[i2c_tx_buffer_head].f_RW=WRITE_FLAG;
    i2c_buffer_tx[i2c_tx_buffer_head].f_LP=NOT_LAST_PACKAGE;
    i2c_buffer_tx[i2c_tx_buffer_head].f_Type=ADDRESS_PACKAGE;  

    for(i=0;i<length-1;i++){
        i2c_tx_buffer_head=(i2c_tx_buffer_head+1)%I2C_TX_BUFFER_SIZE;

        i2c_buffer_tx[i2c_tx_buffer_head].value=data[i];
        i2c_buffer_tx[i2c_tx_buffer_head].f_RW=WRITE_FLAG;
        i2c_buffer_tx[i2c_tx_buffer_head].f_LP=NOT_LAST_PACKAGE;
        i2c_buffer_tx[i2c_tx_buffer_head].f_Type=DATA_PACKAGE;
    }
    i2c_tx_buffer_head=(i2c_tx_buffer_head+1)%I2C_TX_BUFFER_SIZE;

    i2c_buffer_tx[i2c_tx_buffer_head].value=data[i];
    i2c_buffer_tx[i2c_tx_buffer_head].f_RW=WRITE_FLAG;
    i2c_buffer_tx[i2c_tx_buffer_head].f_LP=LAST_PACKAGE;
    i2c_buffer_tx[i2c_tx_buffer_head].f_Type=DATA_PACKAGE;

sei();
}