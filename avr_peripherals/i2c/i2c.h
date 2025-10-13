/**
 * @file i2c.h
 * @brief I2C (TWI) Master driver for AVR (ATmega2560) with optional TX buffer.
 *
 * This module implements I2C communication in Master mode with a choice of
 * blocking or non-blocking operations depending on whether the TX buffer
 * (ring buffer) is used.
 *
 * - Without the buffer: functions are blocking and wait for each byte to be transmitted,
 *   consuming CPU cycles until completion.
 * - With the buffer enabled: non-blocking operations are possible, allowing the CPU
 *   to perform other tasks while I2C transmission occurs in the background via interrupts.
 *
 * @note TX buffer usage is controlled via the `USE_RING_BUFFER_FOR_I2C_OPERATIONS` macro.
 * @note Timer4 Compare A (`TIMER4_COMPA_vect`) and TWI interrupt (`TWI_vect`) are used
 *       internally to manage non-blocking transmission when the buffer is enabled.
 *
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */

#ifndef _I2C_H_
    #define _I2C_H_

#include <i2c_common.h>

#if USE_RING_BUFFER_FOR_I2C_OPERATIONS==1

#include <i2c_ringbuffer.h>

#elif USE_RING_BUFFER_FOR_I2C_OPERATIONS==0

#include <i2c_blocking.h>

#endif // USE_RING_BUFFER_FOR_I2C_OPERATIONS==1

#endif // _I2C_H_