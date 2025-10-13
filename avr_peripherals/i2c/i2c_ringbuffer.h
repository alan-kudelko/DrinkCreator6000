/**
 * @file i2c_ringbuffer.h
 * @brief Non-blocking I2C (TWI) Master driver for AVR (ATmega2560) using a TX ring buffer.
 *
 * This module implements non-blocking I2C communication in Master mode
 * with a transmit ring buffer that stores data packets (address + data).
 * It allows continuous sending of multiple packets without blocking the CPU.
 * The driver is RTOS-independent and suitable for real-time applications.
 *
 * @note TX buffer size can be configured via macros.
 * @note Communication is handled via interrupts:
 *       - Timer4 Compare A (`TIMER4_COMPA_vect`) starts I2C transmission when buffer is non-empty.
 *       - TWI interrupt (`TWI_vect`) continues sending data packets and handles STOP conditions.
 * @note Timing between consecutive data bytes is managed internally by the ISR.
 * @note Timer4 is configured with prescaler 64 to generate ~10 µs ticks for I2C scheduling.
 *
 * This implementation provides full low-level control over the bus while
 * minimizing CPU blocking, suitable for high-frequency or multitasking scenarios.
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */

#ifndef _I2C_RING_BUFFER_H_
    #define _I2C_RING_BUFFER_H_

#include <stdint.h>
#include <stdbool.h>

#include <i2c_common.h>

#define READ_FLAG             1     /**< Flag indicating read operation */
#define WRITE_FLAG            0     /**< Flag indicating write operation */
#define LAST_PACKAGE          1     /**< Flag indicating last package */
#define NOT_LAST_PACKAGE      0     /**< Flag indicating not last package */
#define DATA_PACKAGE          1     /**< Flag indicating data package */   
#define ADDRESS_PACKAGE       0     /**< Flag indicating address package */

#define RW_BIT 0
#define LP_BIT 1
#define DA_BIT 2

// By default package is address package with write flag and not last package

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
 * T_{interrupt} = (39 + 1) * 4\mu s = 160\mu s
 * @f]
 */
#define TIMER4_SENDING_CYCLE 39
/**
 * @name I2C TWI Master Transmitter status codes
 * @{
 */
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
    uint8_t flags; /**< Package flags */
}I2C_Data_t;


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
 * @brief Current state of the I²C finite state machine (FSM).
 *
 * Represents the current step of the I²C driver’s FSM during transmission.
 * The FSM is updated in the `TWI_vect` interrupt handler
 * in response to TWI status codes.
 */
extern volatile uint8_t i2c_state;
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

// ========================
// Core I2C Functions using buffer
// ========================

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

/**
 * @brief Initialize I²C (TWI) hardware and Timer4 support.
 *
 * Configures SDA/SCL pins, sets TWI speed, and sets up Timer4
 * for polling cycles after STOP condition (used in blocking ring buffer mode).
 */
void i2c_init(void);
/**
 * @brief Enable I²C hardware and Timer4.
 */
void i2c_enable(void);
/**
 * @brief Disable I²C hardware and Timer4 if used.
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

// ========================
// Non blocking I2C Functions using buffer
// ========================

/**
 * @brief Enqueues a single data byte to be sent to the specified I²C slave address in non-blocking mode.
 *
 * This function attempts to add the address and data byte to the TX buffer. If the buffer is full,
 * the function returns immediately with a status code indicating failure.
 * Intended for use with RTOS or cooperative multitasking to avoid blocking the CPU.
 *
 * @param address 7-bit I²C slave address.
 * @param data    Single byte to be transmitted.
 * @return uint8_t Returns 0 if the data was successfully queued, 1 if the TX buffer is full.
 */
uint8_t i2c_write_byte_to_address_non_blocking(uint8_t address,uint8_t data);

/**
 * @brief Enqueues multiple data bytes to be sent to the specified I²C slave address in non-blocking mode.
 *
 * This function attempts to add the address and multiple data bytes to the TX buffer.
 * If there is not enough free space in the buffer, it returns immediately with a failure status.
 * Suitable for RTOS-based or cooperative multitasking environments to avoid blocking the CPU.
 *
 * @param address 7-bit I²C slave address.
 * @param data    Pointer to the array of bytes to be transmitted.
 * @param length  Number of bytes to enqueue.
 * @return uint8_t Returns 0 if all bytes were successfully queued, 1 if the TX buffer did not have enough space.
 */
uint8_t i2c_write_bytes_to_address_non_blocking(uint8_t address,uint8_t*data,uint8_t length);


#ifdef __cplusplus
    }
#endif // __cplusplus

#endif // _I2C_RING_BUFFER_H_