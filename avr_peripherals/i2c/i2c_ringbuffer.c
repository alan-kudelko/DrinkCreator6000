#include <avr/io.h>
#include <avr/interrupt.h>

#include <i2c_common.h>

#if USE_RING_BUFFER_FOR_I2C_OPERATIONS==1

#include <i2c_ringbuffer.h>

#include <DrinkCreator6000_Pins.h>

#warning "Timer4 used exclusively for I2C management"

volatile I2C_Data_t i2c_buffer_tx[I2C_TX_BUFFER_SIZE]={0};
volatile i2c_tx_buf_index_t i2c_tx_buffer_head=0;
volatile i2c_tx_buf_index_t i2c_tx_buffer_tail=0;

volatile uint8_t i2c_status=0;
volatile uint8_t i2c_state=I2C_STATE_IDLE;

void i2c_init(void){
    // Configure SDA and SCL as inputs (open-drain mode handled by pull-ups)
    DDRD&=~(1<<I2C_SCL_PIN);
    DDRD&=~(1<<I2C_SDA_PIN);
    // Configure TWI bit rate
    TWSR&=~((1<<TWPS0)|(1<<TWPS1));      // Prescaler = 1
    TWBR=(uint8_t)TWBR_VALUE;            // Bit rate
    TWCR=(1<<TWINT); // Enable TWI + interrupts

    // Configure Timer4 for post-STOP polling
    TCCR4A=0; 
    TCCR4B=0; 
    TCNT4=0;
    OCR4A=TIMER4_POLLING_CYCLE;
    TCCR4B|=(1<<WGM42);              // CTC mode
    TCCR4B|=(1<<CS41)|(1<<CS40);     // Prescaler /64 (~4 µs tick)

    // Debug pins for timing measurement
    DDRH|=(1<<PH4); // Debug pin for ISR(TIMER4_COMPA_vect)
    DDRH|=(1<<PH3); // Debug pin for ISR(TWI_vect)
}

void i2c_enable(void){
    TWCR=(1<<TWEN)|(1<<TWIE)|(1<<TWINT);

    TIMSK4|=(1<<OCIE4A);
}

void i2c_disable(void){
    TWCR&=~(1<<TWEN); // Disable TWI (I2C) peripheral
    TIMSK4&=~(1<<OCIE4A); // Disable timer4
}

void i2c_tx_buffer_clear_until_next_address(void){
    i2c_tx_buf_index_t initialTail=i2c_tx_buffer_tail;
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

// ISR for initializing I2C transmission on Timer4 Compare A interrupt
// Triggered after sending STOP or when first package is queued
ISR(TIMER4_COMPA_vect){
    PORTH^=(1<<PH4);
    // Start I2C transmission only if state is IDLE and TX buffer is not empty
    // Additionally i Check wheter stop signal is not present and
    // if hardware successfully sent package
    if((i2c_state==I2C_STATE_IDLE)&&(i2c_tx_buffer_is_empty()==0)&&(!(TWCR&(1<<TWSTO)))){
        OCR4A=TIMER4_SENDING_CYCLE; // If there is some data in the buffer, process it as quickly as possible
        TIMSK4&=~(1<<OCIE4A); // Disable Timer4 Compare A interrupt
        TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN)|(1<<TWIE); // Send start signal
        // After this trigger ISR(TWI_vect) handles transmission
    }
}

ISR(TWI_vect){
    PORTH^=(1<<PH3); // Debug
    i2c_status=TWSR&TW_STATUS_MASK;

    switch(i2c_status){
        case TW_START:
        case TW_REP_START:
            i2c_state=I2C_STATE_START_SENT;
        break;
        case TW_MT_SLA_ACK:
            i2c_state=I2C_STATE_ADDRESS_SENT;
        break;
        case TW_MT_SLA_NACK:
        case TW_MR_SLA_NACK:
            i2c_state=I2C_STATE_ERROR;
            i2c_error_counters.nack_address++;
        break;
        case TW_MT_DATA_ACK:
            i2c_state=I2C_STATE_DATA_SENT;
        break; 
        case TW_MT_DATA_NACK:
        case TW_MR_DATA_NACK:
            i2c_state=I2C_STATE_ERROR;
            i2c_error_counters.nack_data++;
        break;
        case TW_MR_SLA_ACK:
            i2c_state=I2C_STATE_ADDRESS_SENT;
        break;
        case TW_MR_DATA_ACK:
            i2c_state=I2C_STATE_DATA_RECEIVED;
        break;
        case TW_BUS_ERROR:
            i2c_state=I2C_STATE_ERROR;
            i2c_error_counters.bus_error++;
        break;
        default:
            i2c_state=I2C_STATE_ERROR;
            i2c_error_counters.unexpected_state++;
    }
    // Up to this moment this looks logical
    // Changed to FSM
    switch(i2c_state){
        case I2C_STATE_START_SENT:
            //Wysyłamy adres z bufora (tail)
            if(i2c_buffer_tx[i2c_tx_buffer_tail].f_Type==ADDRESS_PACKAGE){
                TWDR=i2c_buffer_tx[i2c_tx_buffer_tail].value;
                TWCR=(1<<TWIE)|(1<<TWEN)|(1<<TWINT);
            } // To nie generuje problemu z przesuwaniem
            else{
                // Rather obsolete since data is validated during copying to buffer
                //i2c_tx_buffer_tail=(i2c_tx_buffer_tail+1)%I2C_TX_BUFFER_SIZE;
                i2c_tx_buffer_clear_until_next_address();
                TWCR=(1<<TWIE)|(1<<TWEN)|(1<<TWINT)|(1<<TWSTO);
                i2c_state=I2C_STATE_IDLE;
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
                i2c_state=I2C_STATE_IDLE;
                TIMSK4|=(1<<OCIE4A);
            }
        break;
        case I2C_STATE_DATA_SENT:
            // Sprawdzamy czy ta paczka była ostatnia
            // Jeżeli tak to wysyłamy stop, jak nie
            // wysyłamy dane dalej
            if(i2c_buffer_tx[i2c_tx_buffer_tail].f_LP==LAST_PACKAGE){
                TWCR=(1<<TWIE)|(1<<TWEN)|(1<<TWINT)|(1<<TWSTO);
                i2c_state=I2C_STATE_IDLE;
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
                    TIMSK4|=(1<<OCIE4A);
                    TWCR=(1<<TWIE)|(1<<TWEN)|(1<<TWINT)|(1<<TWSTO);
                    i2c_state=I2C_STATE_IDLE;
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
            //i2c_tx_buffer_tail=(i2c_tx_buffer_tail+1)%I2C_TX_BUFFER_SIZE;
            i2c_tx_buffer_clear_until_next_address();
            i2c_state=I2C_STATE_IDLE;
            TIMSK4|=(1<<OCIE4A);
            // Wyłączamy TWI lub restartujemy jak trzeba
        break;
    }

    if(i2c_tx_buffer_head==i2c_tx_buffer_tail){
        TWCR=(1<<TWIE)|(1<<TWEN)|(1<<TWINT);
        i2c_state=I2C_STATE_IDLE;
        OCR4A=TIMER4_POLLING_CYCLE;
        TIMSK4|=(1<<OCIE4A);
    }
}

void i2c_write_byte_to_address_blocking(uint8_t address,uint8_t data){
    i2c_tx_buf_index_t new_head=(i2c_tx_buffer_head+1)%I2C_TX_BUFFER_SIZE;
    // Wait until there is space for two elements in the TX buffer
    while (new_head==i2c_tx_buffer_tail){
        // Busy wait
    }
    cli(); // Disable interrupts to ensure atomic buffer update
    // Enqueue address package with write flag
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
    i2c_tx_buffer_head=(i2c_tx_buffer_head+1)%I2C_TX_BUFFER_SIZE;
    sei(); // Re-enable interrupts
}

void i2c_write_bytes_to_address_blocking(uint8_t address,uint8_t*data,uint8_t length){
    i2c_tx_buf_index_t new_head=(i2c_tx_buffer_head+length+1)%I2C_TX_BUFFER_SIZE;
    uint8_t i;

    while(new_head==i2c_tx_buffer_tail){
        // Wait for enough space in the buffer
    }

    cli();
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
    i2c_tx_buffer_head=(i2c_tx_buffer_head+1)%I2C_TX_BUFFER_SIZE;

    sei();
}
#endif // USE_RING_BUFFER_FOR_I2C_OPERATIONS==1