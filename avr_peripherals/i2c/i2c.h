#ifndef _I2C_H_
    #define _I2C_H_

#include <stdint.h>
#include <DrinkCreator6000_Config_C.h>

// Master Transmitter
#define TW_START              0x08  // START condition transmitted
#define TW_REP_START          0x10  // Repeated START condition transmitted
#define TW_MT_SLA_ACK         0x18  // SLA+W transmitted, ACK received
#define TW_MT_SLA_NACK        0x20  // SLA+W transmitted, NACK received
#define TW_MT_DATA_ACK        0x28  // Data transmitted, ACK received
#define TW_MT_DATA_NACK       0x30  // Data transmitted, NACK received

// Master Receiver
#define TW_MR_SLA_ACK         0x40  // SLA+R transmitted, ACK received
#define TW_MR_SLA_NACK        0x48  // SLA+R transmitted, NACK received
#define TW_MR_DATA_ACK        0x50  // Data received, ACK returned
#define TW_MR_DATA_NACK       0x58  // Data received, NACK returned

// Common
#define TW_STATUS_MASK        0xF8

#define WRITE_MODE            0x00  // Write mode for I2C address
#define READ_MODE             0x01  // Read mode for I2C address

#define I2C_READ_ACK          0x01 // Acknowledge for reading next byte
#define I2C_READ_NACK         0x00 // No acknowledge for reading last byte

#define READ_FLAG             0
#define WRITE_FLAG            1
#define LAST_PACKAGE          1
#define NOT_LAST_PACKAGE      0
#define ADDRESS_PACKAGE       0
#define DATA_PACKAGE          1

struct I2C_DATA{
    uint8_t value; // Address or data
    _Bool f_RW;    // Read 0, Write 1 flag
    _Bool f_LP;    // Last package flag 1 - yes, 0 - no
    _Bool f_Type;  // Package type 0 - address, 1 - data
};

#define I2C_RX_BUFFER_SIZE    64
#define I2C_TX_BUFFER_SIZE    64

#ifdef __cplusplus
    extern "C" {
#endif

void i2c_init(void);

void i2c_disable(void);

uint8_t i2c_begin_transmission(void);

void i2c_end_transmission(void);

uint8_t i2c_write_address(uint8_t address,uint8_t mode);

uint8_t i2c_write_byte(uint8_t data);

uint8_t i2c_write_byte_to_address(uint8_t address,uint8_t data);

uint8_t i2c_write_bytes_to_address(uint8_t address,const uint8_t*data, uint8_t length);

uint8_t i2c_read_byte(uint8_t*data,uint8_t ack);

uint8_t i2c_read_byte_from_address(uint8_t address,uint8_t*data);

uint8_t i2c_read_bytes_from_address(uint8_t address,uint8_t*data,uint8_t length);

#ifdef __cplusplus
    }
#endif // __cplusplus

#endif // _I2C_H_