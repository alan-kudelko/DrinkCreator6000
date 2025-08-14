/**
 * @file i2c.h
 * @brief I2C (TWI) Master driver for AVR (ATmega2560).
 *
 * This module implements blocking and non-blocking I2C communication in Master mode
 * using a TX buffer that stores data packets (address, data, RW flag).
 * Both blocking and non-blocking data reading are supported.
 * The driver is RTOS-independent.
 *
 * @note TX and RX buffer sizes can be configured via macros.
 * @note The driver handles basic bus states and TWI status codes.
 * @note Communication is performed through a buffer and ISR (in the source file).
 *
 * @note Timer4 is used exclusively for I2C management, configured to generate
 * interrupts at approximately 10 microseconds interval (with prescaler 64 and 16 MHz CPU clock).
 *
 * Timer4 Compare A interrupt (`TIMER4_COMPA_vect`) and TWI interrupt (`TWI_vect`) handlers
 * are implemented internally in `i2c.c`.
 */
#ifndef _I2C_H_
    #define _I2C_H_

#include <stdint.h>
#include <stdbool.h>

#include <DrinkCreator6000_Config_C.h>

/**
 * @def I2C_FREQ
 * @brief I2C bus frequency in Hertz.
 *
 * Default is 100 kHz.
 */
#define I2C_FREQ       100000UL  // 100 kHz I2C frequency
/**
 * @def TWBR_VALUE
 * @brief TWI Bit Rate Register value calculated from F_CPU and I2C_FREQ.
 *
 * Used to configure the I2C clock speed.
 */
#define TWBR_VALUE     (((F_CPU/I2C_FREQ)-32UL)/2UL)
/**
 * @def USE_RING_BUFFER_FOR_BLOCKING_OPERATIONS
 * @brief Enables (1) or disables (0) the use of a ring buffer for blocking I2C operations.
 *
 * When enabled, blocking I2C operations use a ring buffer to queue data before transmission,
 * allowing asynchronous processing via interrupts. When disabled, blocking operations
 * bypass the buffer and send data directly.
 */
#define USE_RING_BUFFER_FOR_BLOCKING_OPERATIONS 1
/**
 * @name I2C TWI Master Transmitter status codes
 * @{
 */
#define TW_START              0x08  /**< START condition transmitted */
#define TW_REP_START          0x10  /**< Repeated START condition transmitted */
#define TW_MT_SLA_ACK         0x18  /**< SLA+W transmitted, ACK received */
#define TW_MT_SLA_NACK        0x20  /**< SLA+W transmitted, NACK received */
#define TW_MT_DATA_ACK        0x28  /**< Data transmitted, ACK received */
#define TW_MT_DATA_NACK       0x30  /**< Data transmitted, NACK received */
/** @} */

/**
 * @name I2C TWI Master Receiver status codes
 * @{
 */
#define TW_MR_SLA_ACK         0x40  /**< SLA+R transmitted, ACK received */
#define TW_MR_SLA_NACK        0x48  /**< SLA+R transmitted, NACK received */
#define TW_MR_DATA_ACK        0x50  /**< Data received, ACK returned */
#define TW_MR_DATA_NACK       0x58  /**< Data received, NACK returned */
/** @} */

/**
 * @name Common I2C TWI definitions
 * @{
 */
#define TW_STATUS_MASK        0xF8  /**< Mask for TWI status bits */
#define TW_BUS_ERROR          0x00  /**< Bus error due to illegal start or stop condition */
#define TW_NO_INFO            0xF8  /**< No relevant state information */
#define TW_STO                0xF8  /**< STOP condition transmitted (used when TWI is ready) */

#define WRITE_MODE            0x00  /**< Write mode flag for I2C address */
#define READ_MODE             0x01  /**< Read mode flag for I2C address */

#define I2C_READ_ACK          0x01  /**< Acknowledge bit when reading next byte */
#define I2C_READ_NACK         0x00  /**< No acknowledge bit when reading last byte */

#define READ_FLAG             1     /**< Flag indicating read operation */
#define WRITE_FLAG            0     /**< Flag indicating write operation */
#define LAST_PACKAGE          1     /**< Flag indicating last package */
#define NOT_LAST_PACKAGE      0     /**< Flag indicating not last package */
#define ADDRESS_PACKAGE       0     /**< Flag indicating address package */
#define DATA_PACKAGE          1     /**< Flag indicating data package */
/** @} */

/**
 * @name I2C Finite State Machine (FSM) states
 * @{
 */
#define I2C_STATE_IDLE           0  /**< Idle state, no ongoing transmission */
#define I2C_STATE_START_SENT     1  /**< START condition sent, waiting for acknowledgment */
#define I2C_STATE_ADDRESS_SENT   2  /**< Address sent, waiting for ACK/NACK */
#define I2C_STATE_DATA_SENT      3  /**< Data byte sent, waiting for ACK/NACK */
#define I2C_STATE_ERROR          4  /**< Error occurred during transmission */
/** @} */

/**
 * @brief Structure representing a single I2C data packet.
 *
 * Contains the value (address or data), flags indicating read/write,
 * whether this is the last package, and the package type (address or data).
 */
struct I2C_DATA {
    uint8_t value; /**< Address or data byte */
    bool f_RW;     /**< Read/write flag: 1 = read, 0 = write */
    bool f_LP;     /**< Last package flag: 1 = last package, 0 = not last */
    bool f_Type;   /**< Package type: 0 = address, 1 = data */
};
/**
 * @def I2C_TX_BUFFER_SIZE
 * @brief Size of the I2C transmit buffer, in bytes.
 */
#define I2C_TX_BUFFER_SIZE    255
/**
 * @def I2C_RX_BUFFER_SIZE
 * @brief Size of the I2C receive buffer, in bytes.
 */
#define I2C_RX_BUFFER_SIZE    64
/**
 * @typedef i2c_tx_buf_index_t
 * @brief Index type for I2C TX buffer positions.
 *
 * Uses uint8_t if TX buffer size is 255 bytes or less, otherwise uint16_t.
 */
#if(I2C_TX_BUFFER_SIZE<=255)
    typedef uint8_t i2c_tx_buf_index_t;
#else
    typedef uint16_t i2c_tx_buf_index_t;
#endif
/**
 * @typedef i2c_rx_buf_index_t
 * @brief Index type for I2C RX buffer positions.
 *
 * Uses uint8_t if RX buffer size is 255 bytes or less, otherwise uint16_t.
 */
#if(I2C_RX_BUFFER_SIZE<=255)
    typedef uint8_t i2c_rx_buf_index_t;
#else
    typedef uint16_t i2c_rx_buf_index_t;
#endif

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus
// ========================
// Global Buffer Variables
// ========================
/**
 * @brief Transmit buffer holding I2C data packets to be sent.
 */
extern volatile struct I2C_DATA i2c_buffer_tx[I2C_TX_BUFFER_SIZE];
/**
 * @brief Index of the head position in the TX buffer.
 */
extern volatile uint8_t i2c_tx_buffer_head;
/**
 * @brief Index of the tail position in the TX buffer.
 */
extern volatile uint8_t i2c_tx_buffer_tail;
/**
 * @brief Current status of the I2C state machine.
 */
extern volatile uint8_t i2c_status;

extern volatile uint8_t i2c_state;
/**
 * @brief Counter of transmission errors detected on the I2C bus.
 */
extern volatile uint16_t i2c_tx_error_counter;
// ========================
// Core I2C Functions
// ========================
/**
 * @brief Initializes the I2C hardware and software buffers.
 *
 * Configures the TWI peripheral, timer, and prepares internal buffers
 * for I2C communication in Master mode.
 */
void i2c_init(void);
/**
 * @brief Disables the I2C hardware and related interrupts.
 *
 * Stops the TWI peripheral and disables the timer used for I2C management.
 */
void i2c_disable(void);
/**
 * @brief Returns the current status code of the I2C state machine.
 *
 * @return I2C status code (one of the defined TW_* or I2C_STATE_* values).
 */
uint8_t i2c_get_status(void);
/**
 * @brief Checks if the I2C TX buffer is empty.
 *
 * @return true if the TX buffer is empty, false otherwise.
 */
static inline bool i2c_tx_buffer_is_empty(void){
    return i2c_tx_buffer_head==i2c_tx_buffer_tail;
}
/**
 * @brief Clears the TX buffer until the next address package is found.
 *
 * This function advances the TX buffer tail index, effectively removing
 * data packets until it reaches the next address package or the buffer head.
 * It is used to discard pending data if a slave failed to respond,
 * allowing the next transmission to be properly enqueued.
 */
void i2c_tx_buffer_clear_until_next_address(void);


void i2c_tx_buffer_show_contet(void);
/**
 * @brief Sends a single data byte to the specified I2C slave address in blocking mode.
 *
 * This function enqueues the address and data byte into the TX buffer and waits
 * until there is enough space in the buffer before returning.
 * Interrupts are temporarily disabled during buffer updates to ensure atomicity
 * and data consistency.
 *
 * @param address 7-bit I2C slave address.
 * @param data    Single byte to be transmitted.
 */
void i2c_write_byte_blocking(uint8_t address,uint8_t data);
/**
 * @brief Sends multiple data bytes to the specified I2C slave address in blocking mode.
 *
 * This function enqueues the address and data bytes into the TX buffer and waits
 * until there is enough space in the buffer before returning.
 * Interrupts are temporarily disabled during buffer updates to ensure atomicity
 * and data consistency.
 *
 * @param address 7-bit I2C slave address.
 * @param data    Pointer to the data array to be transmitted.
 * @param length  Number of bytes to send.
 */
void i2c_write_bytes_blocking(uint8_t address,uint8_t*data,uint8_t length);

#ifdef __cplusplus
    }
#endif // __cplusplus

#endif // _I2C_H_