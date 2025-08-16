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
 * interrupts at approximately 10 microseconds interval (with prescaler 64 and 8 MHz CPU clock).
 *
 * @note The `TIMER4_POLLING_CYCLE` macro defines the number of Timer4 ticks between
 * bus polling operations, while the `TIMER4_SENDING_CYCLE` macro specifies the number
 * of ticks between consecutive data bytes sent on the bus.
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

#if USE_RING_BUFFER_FOR_BLOCKING_OPERATIONS==1
/**
 * @def TIMER4_POLLING_CYCLE
 * @brief OCR4A value for Timer 4 in "polling" mode.
 *
 * Timer 4 generates an interrupt at a defined period, used to check whether 
 * there are data in the I2C buffer to be sent. The OCR4A value sets the time 
 * between consecutive ISR calls.
 *
 * Calculation of OCR4A:
 * @f[
 * OCR4A = \frac{T_{interrupt}}{T_{tick}} - 1
 * @f]
 * where:
 * - @f$T_{interrupt}@f$ is the desired interrupt period (in seconds),
 * - @f$T_{tick}@f$ is the timer tick period (here 4 µs = 4e-6 s).
 *
 * For OCR4A = 255 with 4 µs tick:
 * @f[
 * T_{interrupt} = (OCR4A + 1) * T_{tick} = (255 + 1) * 4\mu s \approx 1.024\ ms
 * @f]
 */
#define TIMER4_POLLING_CYCLE 255
/**
 * @def TIMER4_SENDING_CYCLE
 * @brief OCR4A value for Timer 4 in "sending" mode (fast ISR during active I2C transmission).
 *
 * The timer interval is reduced to accelerate polling while sending data.
 *
 * Calculation of OCR4A:
 * @f[
 * T_{interrupt} = (OCR4A + 1) * T_{tick}
 * @f]
 *
 * For OCR4A = 4 with 4 µs tick:
 * @f[
 * T_{interrupt} = (14 + 1) * 4\mu s = 60\mu s
 * @f]
 */
#define TIMER4_SENDING_CYCLE 14
/**
 * @name I2C TWI Master Transmitter status codes
 * @{
 */
#endif // USE_RING_BUFFER_FOR_BLOCKING_OPERATIONS==1

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
#define I2C_STATE_DATA_RECEIVED  5  /**< Data byte received from slave, waiting for next action */
/** @} */

/**
 * @brief Structure representing a single I2C data packet.
 *
 * Contains the value (address or data), flags indicating read/write,
 * whether this is the last package, and the package type (address or data).
 */
typedef struct{
    uint8_t value; /**< Address or data byte */
    bool f_RW;     /**< Read/write flag: 1 = read, 0 = write */
    bool f_LP;     /**< Last package flag: 1 = last package, 0 = not last */
    bool f_Type;   /**< Package type: 0 = address, 1 = data */
}I2C_Data_t;
/**
 * @brief Counters for different I2C/TWI error types
 */
typedef struct{
    uint16_t bus_error;          /**< Bus error (illegal start/stop condition) */
    uint16_t nack_address;       /**< NACK received after sending SLA+W or SLA+R */
    uint16_t nack_data;          /**< NACK received after sending data byte */
    uint16_t arbitration_lost;   /**< Arbitration lost during transmission */
    uint16_t unexpected_state;   /**< Any unexpected/unhandled TWI status */
}I2C_ErrorCounters_t;
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
extern volatile I2C_Data_t i2c_buffer_tx[I2C_TX_BUFFER_SIZE];
/**
 * @brief Index of the head position in the TX buffer.
 */
extern volatile i2c_tx_buf_index_t i2c_tx_buffer_head;
/**
 * @brief Index of the tail position in the TX buffer.
 */
extern volatile i2c_tx_buf_index_t i2c_tx_buffer_tail;
/**
 * @brief Current status of the I2C state machine.
 */
/**
 * @brief Status of the current I²C operation.
 *
 * Stores the last TWI/I²C status code (from the TWSR register),
 * indicating the current bus condition, e.g.:
 * - START condition transmitted
 * - SLA+W / SLA+R transmitted and ACK/NACK received
 * - Data byte transmitted/received with ACK/NACK
 * - Bus errors (arbitration lost, ACK failure, etc.)
 */
extern volatile uint8_t i2c_status;

/**
 * @brief Current state of the I²C finite state machine (FSM).
 *
 * Represents the current step of the I²C driver’s FSM during transmission.
 * The FSM is updated in the `TWI_vect` interrupt handler
 * in response to TWI status codes.
 */
extern volatile uint8_t i2c_state;
/**
 * @brief I²C error counters.
 *
 * Holds cumulative counts of specific I²C/TWI error events
 * detected by the driver during operation.
 *
 * These counters can be used for diagnostics and long-term
 * monitoring of bus reliability.
 *
 * Typical fields inside ::I2C_ErrorCounters_t may include:
 * - **arbitration_lost** — number of times bus arbitration was lost
 * - **nack_on_address** — number of NACKs received after sending SLA+W or SLA+R
 * - **nack_on_data** — number of NACKs received after sending a data byte
 * - **bus_error** — number of illegal START/STOP conditions detected
 *
 * The structure is updated inside the `TWI_vect` ISR
 * whenever an error-related TWI status code is encountered.
 */
extern volatile I2C_ErrorCounters_t i2c_error_counters;
// ========================
// Core I2C Functions
// ========================
/**
 * @brief Initialize I²C (TWI) hardware and optional Timer4 support.
 *
 * Configures SDA/SCL pins, sets TWI speed, and optionally sets up Timer4
 * for polling cycles after STOP condition (used in blocking ring buffer mode).
 */
void i2c_init(void);
/**
 * @brief Enable I²C hardware (and Timer4 if used).
 */
void i2c_enable(void);
/**
 * @brief Disable I²C hardware (and Timer4 if used).
 */
void i2c_disable(void);
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
void i2c_write_byte_to_address_blocking(uint8_t address,uint8_t data);
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
void i2c_write_bytes_to_address_blocking(uint8_t address,uint8_t*data,uint8_t length);

#ifdef __cplusplus
    }
#endif // __cplusplus

#endif // _I2C_H_