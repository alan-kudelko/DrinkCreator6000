#ifndef _I2C_H_
    #define _I2C_H_

#include <stdint.h>
#include <stdbool.h>

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

#define READ_FLAG             1
#define WRITE_FLAG            0
#define LAST_PACKAGE          1
#define NOT_LAST_PACKAGE      0
#define ADDRESS_PACKAGE       0
#define DATA_PACKAGE          1

// I2C FSM states
#define I2C_STATE_IDLE           0  // Idle state, no ongoing transmission
#define I2C_STATE_START_SENT     1  // START condition has been sent, waiting for acknowledgment
#define I2C_STATE_ADDRESS_SENT   2  // Address sent, waiting for ACK/NACK
#define I2C_STATE_DATA_SENT      3  // Data byte sent, waiting for ACK/NACK
#define I2C_STATE_STOP_SENT      4  // STOP condition sent, transmission finished
#define I2C_STATE_ERROR          5  // Error occurred during transmission

struct I2C_DATA{
    uint8_t value; // Address or data
    bool f_RW;    // Read 1, Write 0 flag
    bool f_LP;    // Last package flag 1 - yes, 0 - no
    bool f_Type;  // Package type 0 - address, 1 - data
};

#define I2C_RX_BUFFER_SIZE    64
#define I2C_TX_BUFFER_SIZE    64

#ifdef __cplusplus
    extern "C" {
#endif

extern volatile struct I2C_DATA i2c_buffer_tx[I2C_TX_BUFFER_SIZE];
extern volatile uint8_t i2c_tx_buffer_head;
extern volatile uint8_t i2c_tx_buffer_tail;
extern volatile uint8_t i2c_status;

void i2c_init(void);

void i2c_disable(void);

uint8_t i2c_get_status(void);

uint8_t i2c_begin_transmission(void);

void i2c_end_transmission(void);

uint8_t i2c_write_byte_blocking(uint8_t address,uint8_t data);

#ifdef __cplusplus
    }
#endif // __cplusplus

#endif // _I2C_H_