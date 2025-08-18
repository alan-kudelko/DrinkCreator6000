/**
 * @file i2c_common.h
 * @brief Common I2C (TWI) definitions and macros for AVR projects.
 *
 * This header provides the necessary constants, bit masks, and configuration macros
 * used internally by the I2C driver, including:
 *  - TWI status codes
 *  - TX/RX buffer sizes
 *  - Timer configuration macros
 *  - Register and bit definitions for controlling I2C hardware
 *
 * It is included by higher-level I2C modules (e.g., `i2c.h`) to ensure consistent
 * configuration and low-level access across the project.
 *
 * @note This file does not contain function implementations; it only defines
 *       values and macros used by the I2C driver.
 */
#ifndef _I2C_COMMON_H_
    #define _I2C_COMMON_H_

#include <stdint.h>

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

/** @} */

#define USE_RING_BUFFER_FOR_I2C_OPERATIONS 1

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

#endif // _I2C_COMMON_H_