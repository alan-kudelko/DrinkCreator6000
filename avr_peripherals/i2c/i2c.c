#include <i2c.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <DrinkCreator6000_Pins.h>

volatile struct I2C_DATA i2c_buffer_tx[I2C_TX_BUFFER_SIZE]={0};
volatile uint8_t i2c_tx_buffer_head=0;
volatile uint8_t i2c_tx_buffer_tail=0;
volatile uint8_t i2c_status=0;

void i2c_init(void){
    // Setup SDA and SCL pins as INPUTS
    DDRD&=~(1<<I2C_SCL_PIN);
    DDRD&=~(1<<I2C_SDA_PIN);

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

ISR(TWI_vect){
    uint8_t status=TWSR&TW_STATUS_MASK;
    // Changed to FSM

    
}

uint8_t i2c_write_byte_blocking(uint8_t address,uint8_t data){
    uint8_t new_head=(i2c_tx_buffer_head+1)%I2C_TX_BUFFER_SIZE;
    while(new_head==i2c_tx_buffer_tail){
        // Wait for space in the buffer
    }

    i2c_tx_buffer_head=new_head;
    i2c_buffer_tx[i2c_tx_buffer_head].value=(address<<1)|WRITE_FLAG;
    i2c_buffer_tx[i2c_tx_buffer_head].f_RW=WRITE_FLAG;
    i2c_buffer_tx[i2c_tx_buffer_head].f_LP=NOT_LAST_PACKAGE;
    i2c_buffer_tx[i2c_tx_buffer_head].f_Type=ADDRESS_PACKAGE;

    TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWIE);

    new_head=(i2c_tx_buffer_head+1)%I2C_TX_BUFFER_SIZE;

    while(new_head==i2c_tx_buffer_tail){
           // Wait for space in the buffer
    }

    i2c_tx_buffer_head=new_head;
    i2c_buffer_tx[i2c_tx_buffer_head].value=data;
    i2c_buffer_tx[i2c_tx_buffer_head].f_RW=WRITE_FLAG;
    i2c_buffer_tx[i2c_tx_buffer_head].f_LP=LAST_PACKAGE;
    i2c_buffer_tx[i2c_tx_buffer_head].f_Type=DATA_PACKAGE;

    TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWIE);

    return 0;
}
