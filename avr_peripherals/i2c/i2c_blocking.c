#include <avr/io.h>

#include <i2c_common.h>

#if USE_RING_BUFFER_FOR_I2C_OPERATIONS==0

#include <i2c_blocking.h>

#include <DrinkCreator6000_Pins.h>

#warning "I2C operations are blocking"

void i2c_init(void){
    // Configure SDA and SCL as inputs (open-drain mode handled by pull-ups)
    DDRD&=~(1<<I2C_SCL_PIN);
    DDRD&=~(1<<I2C_SDA_PIN);
    // Configure TWI bit rate
    TWSR&=~((1<<TWPS0)|(1<<TWPS1));      // Prescaler = 1
    TWBR=(uint8_t)TWBR_VALUE;            // Bit rate
    TWCR=(1<<TWINT); // Enable TWI + interrupts

    // Debug pins for timing measurement
    DDRH|=(1<<PH4); // Debug pin for ISR(TIMER4_COMPA_vect)
    DDRH|=(1<<PH3); // Debug pin for ISR(TWI_vect)
}

void i2c_enable(void){
    TWCR=(1<<TWEN)|(1<<TWIE)|(1<<TWINT);
}

void i2c_disable(void){
    TWCR&=~(1<<TWEN); // Disable TWI (I2C) peripheral
}

void i2c_write_byte_to_address_blocking(uint8_t address,uint8_t data){
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
    TWDR = (address << 1) | WRITE_MODE;
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
}

void i2c_write_bytes_to_address_blocking(uint8_t address,uint8_t*data,uint8_t length){
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
    TWDR = (address << 1) | WRITE_MODE;
    TWCR = (1 << TWINT) | (1 << TWEN);
    timeout = I2C_TIMEOUT;
    while (!(TWCR & (1 << TWINT))) {
        if (--timeout == 0) return; // timeout error
    }
    if ((TWSR & 0xF8) != TW_MT_SLA_ACK) return; // error: SLA+W not acked

    // Send data byte
    TWDR = data[0];
    TWCR = (1 << TWINT) | (1 << TWEN);
    timeout = I2C_TIMEOUT;
    while (!(TWCR & (1 << TWINT))) {
        if (--timeout == 0) return; // timeout error
    }
    if ((TWSR & 0xF8) != TW_MT_DATA_ACK) return; // error: data not acked

        // Send data byte
    TWDR = data[1];
    TWCR = (1 << TWINT) | (1 << TWEN);
    timeout = I2C_TIMEOUT;
    while (!(TWCR & (1 << TWINT))) {
        if (--timeout == 0) return; // timeout error
    }
    if ((TWSR & 0xF8) != TW_MT_DATA_ACK) return; // error: data not acked
    // STOP condition
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

uint8_t i2c_read_reg_from_adddress_blocking(uint8_t address,uint8_t reg){
        #define I2C_TIMEOUT 10000
    uint16_t timeout;
    uint8_t retval=0;
    // Send start condition
    TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWSTA);
            timeout = I2C_TIMEOUT;
    while(!(TWCR&(1<<TWINT))){
        if (--timeout == 0) return 0; // timeout error
    }
    // Send address with write mode
    // to specify target register
    TWDR=(address<<1)|WRITE_MODE;
    TWCR=(1<<TWINT)|(1<<TWEN);
        timeout = I2C_TIMEOUT;
    while(!(TWCR&(1<<TWINT))){
         if (--timeout == 0) return 0; // timeout error       
    }
    // Send target register
    TWDR=reg;
    TWCR=(1<<TWINT)|(1<<TWEN);
            timeout = I2C_TIMEOUT;
    while(!(TWCR&(1<<TWINT))){
        if (--timeout == 0) return 0; // timeout error
    }
    // Send repeated start
    TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWSTA);
            timeout = I2C_TIMEOUT;
    while(!(TWCR&(1<<TWINT))){
        if (--timeout == 0) return 0; // timeout error
    }
    TWDR=(address<<1)|READ_MODE;
    TWCR=(1<<TWINT)|(1<<TWEN);
            timeout = I2C_TIMEOUT;
    while(!(TWCR&(1<<TWINT))){
        if (--timeout == 0) return 0; // timeout error
    }
    // Read byte
    TWCR=(1<<TWINT)|(1<<TWEN);
            timeout = I2C_TIMEOUT;
    while(!(TWCR&(1<<TWINT))){
        if (--timeout == 0) return 0; // timeout error
    }
    retval=TWDR;
    TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWSTO);

    return retval;
}

uint8_t i2c_read_regs_from_address_blocking(uint8_t address,uint8_t reg,uint8_t*data,uint8_t count){
    return 0;
}

#endif // USE_RING_BUFFER_FOR_I2C_OPERATIONS==0