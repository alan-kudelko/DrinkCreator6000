/**
 * @file i2c_blocking.h
 * @brief Blocking I2C (TWI) Master operations for AVR (ATmega2560) without TX buffer.
 *
 * This header declares functions for performing I2C transactions in a blocking manner.
 * All operations wait until the TWI hardware has completed the byte transfer before returning.
 *
 * @note These functions do not use a TX buffer and therefore occupy the CPU during transmission.
 * @note Suitable for simple or low-frequency I2C operations where non-blocking behavior is not required.
 */
#ifndef _I2C_BLOCKING_H_
    #define _I2C_BLOCKING_H_

#include <i2c_common.h>

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

void i2c_init(void);
void i2c_enable(void);
void i2c_disable(void);
void i2c_write_byte_to_address_blocking(uint8_t address,uint8_t data);
void i2c_write_bytes_to_address_blocking(uint8_t address,uint8_t*data,uint8_t length);

uint8_t i2c_read_reg_from_adddress_blocking(uint8_t address,uint8_t reg);

uint8_t i2c_read_regs_from_address_blocking(uint8_t address,uint8_t reg,uint8_t*data,uint8_t count);

#ifdef __cplusplus
    }
#endif // __cplusplus

#endif // _I2C_BLOCKING_H_