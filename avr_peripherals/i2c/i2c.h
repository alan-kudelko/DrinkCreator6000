#ifndef _I2C_H_
#define _I2C_H_

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

#include <DrinkCreator6000_Config_C.h>
#include <stdint.h>

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