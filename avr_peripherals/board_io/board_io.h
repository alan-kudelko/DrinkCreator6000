/**
 * @file board_io.h
 * @brief High-level control interface for circulation, cooling, fans, pumps and shift register.
 *
 * This header provides functions to control various hardware components such as circulation pumps,
 * Peltier cells, enclosure fans, and HC595 shift registers. It also abstracts reading keyboard input
 * and temperature readiness signals.
 *
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */
#ifndef BOARD_IO_H_
	#define BOARD_IO_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
	extern "C"{
#endif // __cplusplus

/**
 * @brief Turns on the liquid circulation system.
 */
void circulation_on(void);

/**
 * @brief Turns off the liquid circulation system.
 */
void circulation_off(void);

/**
 * @brief Enables Peltier cooling cells.
 */
void cooler_on(void);

/**
 * @brief Disables Peltier cooling cells.
 */
void cooler_off(void);

/**
 * @brief Turns on fans for the enclosure and radiator.
 */
void fans_on(void);


/**
 * @brief Turns off fans for the enclosure and radiator.
 */
void fans_off(void);

/**
 * @brief Enables the shift register output (HC595).
 */
void pumps_enable(void);

/**
 * @brief Disables the shift register output (HC595).
 */
void pumps_disable(void);

/**
 * @brief Reads the state of the keyboard GPIO pin.
 *
 * @return keyboard interrupt state (false if activated, true if not)
 */
bool read_keyboard_int(void);

/**
 * @brief Reads the temperature readiness GPIO pin state.
 *
 * @return temperature ready state (false if ready, true if not)
 */
bool is_temp_ready(void);

/**
 * @brief Sends a byte to the HC595 shift register.
 *
 * @param value Byte to send to the shift register.
 */
void shiftOut(uint8_t value);

#ifdef __cplusplus
	}
#endif // __cplusplus
	

#endif // BOARD_IO_H_