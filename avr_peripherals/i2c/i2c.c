#include <i2c.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <DrinkCreator6000_Pins.h>

volatile struct I2C_DATA i2c_buffer_tx[I2C_TX_BUFFER_SIZE]={0};
volatile uint8_t i2c_tx_buffer_head=0;
volatile uint8_t i2c_tx_buffer_tail=0;
volatile uint8_t i2c_status=I2C_STATE_IDLE;

void i2c_init(void){
    // Setup SDA and SCL pins as INPUTS
    DDRD&=~(1<<I2C_SCL_PIN);
    DDRD&=~(1<<I2C_SDA_PIN);
    // Timer 4 used as clock for driving I2C after sending stop
TCCR4A = 0;           // normal mode count to OCR4A
TCCR4B = 0;
TCNT4  = 0;           // counter's zero
OCR4A  = 249;         // 10 us przy preskalerze 64 i 16 MHz clk
TCCR4B |= (1 << WGM42);              // CTC mode
TCCR4B |= (1 << CS42) | (1 << CS40); // prescaler 64
TIMSK4 |= (1 << OCIE4A);             // enable interrupts Compare A

    TWSR&=(~((1<<TWPS0)|(1<<TWPS1))); // Set prescaler to 1
    TWBR=(uint8_t)TWBR_VALUE; // Set bit rate register for I2C speed
    TWCR=(1<<TWEN); // Enable TWI (I2C)
}

void i2c_disable(void){
    TWCR&=(~(1<<TWEN)); // Disable TWI (I2C)
}

uint8_t i2c_get_status(void){
    return i2c_status;
}

void i2c_tx_buffer_clear_until_next_address(void){
    uint8_t initialTail=i2c_tx_buffer_tail;

    do{
        if((i2c_buffer_tx[i2c_tx_buffer_tail].f_Type==ADDRESS_PACKAGE)&&(i2c_tx_buffer_tail!=initialTail)){
            break;
        }
        i2c_tx_buffer_tail=(i2c_tx_buffer_tail+1)%I2C_TX_BUFFER_SIZE;
        // Clearing data from buffer until next address is found
        // This way, if a slave failed to respond,
        // the next sending operation can be enqueued
    }while(i2c_tx_buffer_tail != i2c_tx_buffer_head);
}

uint8_t i2c_begin_transmission(void){
    TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWSTA); // Clear interrupt flag, enable TWI and send START condition

    while(!(TWCR&(1<<TWINT))); // Wait for transmission to complete

    if((TWSR&TW_STATUS_MASK)!=TW_START){ // Check if START condition was sent successfully
        return 1; // Error occurred
    }
    return 0;
}

void i2c_end_transmission(void){
    TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWSTO); // Clear interrupt flag, enable TWI and send STOP condition
}

//ISR for initializing transmission of i2c bus
//Used after sending stop or if first package is queued for sending
ISR(TIMER4_COMPA_vect){
    if((i2c_status==I2C_STATE_IDLE)&&(i2c_tx_buffer_is_empty()==0)){
        TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN)|(1<<TWIE);
        // Send I2C start signal
        TIMSK4&=~(1<<OCIE4A);
        // Turn of timer
        TCNT4=0;
        // Reset counter
    }
}

ISR(TWI_vect){
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
            i2c_status=I2C_STATE_ERROR;
        break;
        case TW_MT_DATA_ACK:
            i2c_status=I2C_STATE_DATA_SENT;
        break; 
        default:
            i2c_status=I2C_STATE_ERROR;
        break;
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
}

uint8_t i2c_write_byte_blocking(uint8_t address,uint8_t data){
    uint8_t new_head=(i2c_tx_buffer_head+1)%I2C_TX_BUFFER_SIZE;
    while(new_head==i2c_tx_buffer_tail){
        // Wait for space in the buffer
        //uart_putc_blocking('c');
    }

    i2c_tx_buffer_head=new_head;
    i2c_buffer_tx[i2c_tx_buffer_head].value=(address<<1)|WRITE_FLAG;
    i2c_buffer_tx[i2c_tx_buffer_head].f_RW=WRITE_FLAG;
    i2c_buffer_tx[i2c_tx_buffer_head].f_LP=NOT_LAST_PACKAGE;
    i2c_buffer_tx[i2c_tx_buffer_head].f_Type=ADDRESS_PACKAGE;

    new_head=(i2c_tx_buffer_head+1)%I2C_TX_BUFFER_SIZE;

    while(new_head==i2c_tx_buffer_tail){
        // Wait for space in the buffer
        //uart_putc_blocking('c');
    }

    i2c_tx_buffer_head=new_head;
    i2c_buffer_tx[i2c_tx_buffer_head].value=data;
    i2c_buffer_tx[i2c_tx_buffer_head].f_RW=WRITE_FLAG;
    i2c_buffer_tx[i2c_tx_buffer_head].f_LP=LAST_PACKAGE;
    i2c_buffer_tx[i2c_tx_buffer_head].f_Type=DATA_PACKAGE;
    
    return 0;
}
