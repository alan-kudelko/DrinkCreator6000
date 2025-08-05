#include <i2c.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <DrinkCreator6000_Pins.h>

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

uint8_t i2c_write_address(uint8_t address,uint8_t mode){
    if(i2c_begin_transmission()){
        return 1; // Error starting transmission
    }

    TWDR=address<<1|mode; // Load address into data register (shifted left for W mode)

    TWCR=(1<<TWINT)|(1<<TWEN); // Clear interrupt flag, enable TWI and send address

    while(!(TWCR&(1<<TWINT))); // Wait for transmission to complete

    uint8_t status=(TWSR&TW_STATUS_MASK); // Get status code

    if(status!=TW_MT_SLA_ACK){ // Check if address was acknowledged
        return 1; // Error occurred
    }
    return 0; // Address sent successfully
}

uint8_t i2c_write_byte(uint8_t data){
    TWDR=data; // Load data into data register
    TWCR=(1<<TWINT)|(1<<TWEN); // Clear interrupt flag and enable TWI

    while(!(TWCR&(1<<TWINT))); // Wait for transmission to complete

    uint8_t status=(TWSR&TW_STATUS_MASK); // Get status code

    if(status!=TW_MT_DATA_ACK){ // Check if data was transmitted successfully
        return 1; // Error occurred
    }
    return 0;
}

uint8_t i2c_write_byte_to_address(uint8_t address,uint8_t data){
    if(i2c_write_address(address,WRITE_MODE)){
        return 1; // Error writing address
    }
    if(i2c_write_byte(data)){
        return 1; // Error writing data
    }
    i2c_end_transmission(); // End transmission
    return 0; // Data written successfully
}

uint8_t i2c_write_bytes_to_address(uint8_t address,const uint8_t*data,uint8_t length){
    uint8_t i=0;
    if(i2c_write_address(address,WRITE_MODE)){
        return 1; // Error writing address
    }

    for(i=0;i<length;i++){
        if(i2c_write_byte(data[i])){
            return 1; // Error writing data
        }
    }
    i2c_end_transmission(); // End transmission

    return 0;
}

uint8_t i2c_read_byte(uint8_t*data,uint8_t ack){
    if(ack){
        TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWEA); // Clear interrupt flag, enable TWI and enable ACK for next byte
    }
    else{
        TWCR=(1<<TWINT)|(1<<TWEN); // Disable ACK
    }

    while(!(TWCR&(1<<TWINT))); // Wait for transmission to complete

    uint8_t status=(TWSR&TW_STATUS_MASK); // Get status code

    if(status!=TW_MR_DATA_ACK && status!=TW_MR_DATA_NACK){ // Check if data was received successfully
        return 1; // Error occurred
    }
    *data=TWDR; // Read data from data register
    return 0;
}

uint8_t i2c_read_byte_from_address(uint8_t address,uint8_t*data){
    if(i2c_begin_transmission()){
        return 1; // Error starting transmission
    }
    if(i2c_write_address(address,READ_MODE)){ // Write address in read mode
        i2c_end_transmission(); // End transmission if error occurred
        return 1; // Error writing address
    }
    if(i2c_read_byte(data,I2C_READ_NACK)){
        i2c_end_transmission(); // End transmission if error occurred
        return 1; // Error reading byte
    }
    i2c_end_transmission(); // End transmission
    return 0;
}

uint8_t i2c_read_bytes_from_address(uint8_t address,uint8_t*data,uint8_t length){
    uint8_t i;
    if(i2c_begin_transmission()){
        return 1; // Error starting transmission
    }
    if(i2c_write_address(address,READ_MODE)){ // Write address in read mode
        i2c_end_transmission(); // End transmission if error occurred
        return 1; // Error writing address
    }
    for(i=0;i<length;i++){
        if(i2c_read_byte(&data[i],i<length-1)){ // Read byte with ACK for all but the last byte
            i2c_end_transmission(); // End transmission if error occurred
            return 1; // Error reading byte
        }
    }
    i2c_end_transmission(); // End transmission
    return 0;
}