#include <i2c.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void i2c_init(void){
    TWSR&=(~((1<<TWPS0)|(1<<TWPS1))); // Set prescaler to 1
    TWBR=(uint8_t)TWBR_VALUE; // Set bit rate register for I2C speed
    TWCR=(1<<TWEN); // Enable TWI (I2C)
}

void i2c_disable(void){
    TWCR&=(~(1<<TWEN)); // Disable TWI (I2C)
}