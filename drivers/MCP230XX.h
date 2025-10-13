/**
 * @file MCP230XX.h
 * @brief Class interface and register definitions for MCP23008 I²C GPIO expanders.
 *
 * This header combines low-level register address definitions with a high-level
 * C++ class that provides convenient access to MCP23x08 (8-bit)
 * I²C GPIO expander.
 *
 * The class is built on top of a custom I²C driver and supports both blocking and
 * non-blocking operations, making it suitable for real-time environments such as FreeRTOS.
 *
 * Provided features:
 *  - Symbolic register addresses and bit mask macros for all device functions
 *  - Object-oriented abstraction for initialization and configuration
 *  - Pin direction, polarity inversion, and pull-up resistor control
 *  - GPIO read/write methods at both port and pin level
 *  - Support for interrupt configuration and handling
 *
 * Supported devices:
 *  - MCP23008 (single 8-bit port, 8 GPIOs)
 *
 * Typical applications:
 *  - Expanding MCU I/O lines for LEDs, buttons, relays, sensors
 *  - Interrupt-driven input monitoring
 *  - Driving displays and peripheral devices
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */

#ifndef _MCP230XX_H_
	#define _MCP230XX_H_

#include <stdint.h>

/**
 * @name MCP23008 Register Addresses
 * @brief Register map for the MCP23008 I/O Expander.
 *
 * The MCP23008 features 11 control and status registers that manage the
 * configuration, input/output behavior, and interrupt handling for all 8 GPIO pins.
 * Each register can be accessed via the I²C interface using the following addresses.
 *
 * @note All registers are 8-bit wide.
 *
 * @{
 */

#define MCP_IODIR   0x00  /**< I/O Direction Register — sets each pin as input (1) or output (0). */
#define MCP_IPOL    0x01  /**< Input Polarity Register — inverts the input logic if set. */
#define MCP_GPINTEN 0x02  /**< Interrupt-on-Change Enable — enables interrupt for selected pins. */
#define MCP_DEFVAL  0x03  /**< Default Compare Register — defines default values for interrupt comparison. */
#define MCP_INTCON  0x04  /**< Interrupt Control Register — configures interrupt generation mode. */
#define MCP_IOCON   0x05  /**< I/O Configuration Register — sets addressing, interrupt, and output behavior. */
#define MCP_GPPU    0x06  /**< Pull-Up Resistor Configuration — enables weak pull-ups on input pins. */
#define MCP_INTF    0x07  /**< Interrupt Flag Register — indicates which pins triggered an interrupt. */
#define MCP_INTCAP  0x08  /**< Interrupt Capture Register — stores the GPIO state at the time of interrupt. */
#define MCP_GPIO    0x09  /**< General Purpose I/O Register — used to read pin states or write output values. */
#define MCP_OLAT    0x0A  /**< Output Latch Register — reflects the last written output value. */

/** @} */

/**
 * @name IODIR Register Bitfields
 * @brief Controls I/O pin direction for each GPIO pin.
 *
 * Each bit configures the direction of the corresponding I/O pin:
 * - 0 = Output
 * - 1 = Input (default after reset)
 *
 * @{
 */
#define IODIR_IO0 0   /**< Direction control for pin IO0. */
#define IODIR_IO1 1   /**< Direction control for pin IO1. */
#define IODIR_IO2 2   /**< Direction control for pin IO2. */
#define IODIR_IO3 3   /**< Direction control for pin IO3. */
#define IODIR_IO4 4   /**< Direction control for pin IO4. */
#define IODIR_IO5 5   /**< Direction control for pin IO5. */
#define IODIR_IO6 6   /**< Direction control for pin IO6. */
#define IODIR_IO7 7   /**< Direction control for pin IO7. */
/** @} */

/**
 * @name IPOL Register Bitfields
 * @brief Input polarity inversion for each GPIO pin.
 *
 * Each bit inverts the polarity of the corresponding input pin:
 * - 0 = Normal polarity (default)
 * - 1 = Inverted polarity
 *
 * @{
 */
#define IPOL_IP0 0   /**< Polarity inversion control for pin IO0. */
#define IPOL_IP1 1   /**< Polarity inversion control for pin IO1. */
#define IPOL_IP2 2   /**< Polarity inversion control for pin IO2. */
#define IPOL_IP3 3   /**< Polarity inversion control for pin IO3. */
#define IPOL_IP4 4   /**< Polarity inversion control for pin IO4. */
#define IPOL_IP5 5   /**< Polarity inversion control for pin IO5. */
#define IPOL_IP6 6   /**< Polarity inversion control for pin IO6. */
#define IPOL_IP7 7   /**< Polarity inversion control for pin IO7. */
/** @} */

/**
 * @name GPINTEN Register Bitfields
 * @brief Interrupt-on-change enable for each GPIO pin.
 *
 * Each bit controls whether the corresponding pin can trigger an interrupt-on-change:
 * - 0 = Interrupt disabled for this pin
 * - 1 = Interrupt enabled for this pin
 *
 * @{
 */
#define GPINTEN_GPINT0 0   /**< Enable interrupt-on-change for pin IO0. */
#define GPINTEN_GPINT1 1   /**< Enable interrupt-on-change for pin IO1. */
#define GPINTEN_GPINT2 2   /**< Enable interrupt-on-change for pin IO2. */
#define GPINTEN_GPINT3 3   /**< Enable interrupt-on-change for pin IO3. */
#define GPINTEN_GPINT4 4   /**< Enable interrupt-on-change for pin IO4. */
#define GPINTEN_GPINT5 5   /**< Enable interrupt-on-change for pin IO5. */
#define GPINTEN_GPINT6 6   /**< Enable interrupt-on-change for pin IO6. */
#define GPINTEN_GPINT7 7   /**< Enable interrupt-on-change for pin IO7. */
/** @} */

/**
 * @name DEFVAL Register Bitfields
 * @brief Default compare value for interrupt-on-change.
 *
 * Each bit sets the default logic value used in comparison when the corresponding pin
 * is configured for interrupt-on-change (see @ref GPINTEN and @ref INTCON):
 * - If the pin state differs from DEFVAL, an interrupt will be triggered.
 *
 * @{
 */
#define DEFVAL_DEF0 0   /**< Default compare value for pin IO0. */
#define DEFVAL_DEF1 1   /**< Default compare value for pin IO1. */
#define DEFVAL_DEF2 2   /**< Default compare value for pin IO2. */
#define DEFVAL_DEF3 3   /**< Default compare value for pin IO3. */
#define DEFVAL_DEF4 4   /**< Default compare value for pin IO4. */
#define DEFVAL_DEF5 5   /**< Default compare value for pin IO5. */
#define DEFVAL_DEF6 6   /**< Default compare value for pin IO6. */
#define DEFVAL_DEF7 7   /**< Default compare value for pin IO7. */
/** @} */

/**
 * @name INTCON Register Bitfields
 * @brief Interrupt control for MCP230xx.
 *
 * Each bit defines the source for interrupt-on-change detection for the corresponding pin:
 * - 1: Compare against the DEFVAL register
 * - 0: Compare against previous pin value
 *
 * Used in conjunction with @ref GPINTEN to determine which pins generate interrupts.
 *
 * @{
 */
#define INTCON_IOC0 0   /**< Interrupt control for pin IO0. */
#define INTCON_IOC1 1   /**< Interrupt control for pin IO1. */
#define INTCON_IOC2 2   /**< Interrupt control for pin IO2. */
#define INTCON_IOC3 3   /**< Interrupt control for pin IO3. */
#define INTCON_IOC4 4   /**< Interrupt control for pin IO4. */
#define INTCON_IOC5 5   /**< Interrupt control for pin IO5. */
#define INTCON_IOC6 6   /**< Interrupt control for pin IO6. */
#define INTCON_IOC7 7   /**< Interrupt control for pin IO7. */
/** @} */

/**
 * @name IOCON Register Bitfields
 * @brief Configuration bits for the IOCON register.
 *
 * These bits control addressing, interrupt behavior, and output modes.
 *
 * @{
 */
#define IOCON_SEQOP    5   /**< Sequential operation disable (1 = disable auto-increment). */
#define IOCON_DISSLW   4   /**< Slew rate control for SDA output (1 = disabled, 0 = enabled). */
#define IOCON_HAEN     3   /**< Hardware addressing enable (MCP23S08 only, ignored on MCP23008). */
#define IOCON_ODR      2   /**< Configures INT pin as open-drain (1) or active driver (0). */
#define IOCON_INTPOL   1   /**< Interrupt polarity (1 = active-high, 0 = active-low). */
/** @} */

/**
 * @name GPPU Register Bitfields
 * @brief Pull-up resistor configuration for MCP230xx I/O pins.
 *
 * Each bit enables or disables the internal 100kΩ pull-up resistor for the corresponding pin.
 *
 * @{
 */
#define GPPU_PU0 0   /**< Enable pull-up on IO0. */
#define GPPU_PU1 1   /**< Enable pull-up on IO1. */
#define GPPU_PU2 2   /**< Enable pull-up on IO2. */
#define GPPU_PU3 3   /**< Enable pull-up on IO3. */
#define GPPU_PU4 4   /**< Enable pull-up on IO4. */
#define GPPU_PU5 5   /**< Enable pull-up on IO5. */
#define GPPU_PU6 6   /**< Enable pull-up on IO6. */
#define GPPU_PU7 7   /**< Enable pull-up on IO7. */
/** @} */

/**
 * @name INTF Register Bitfields
 * @brief Interrupt flags for MCP230xx I/O pins.
 *
 * Each bit indicates whether an interrupt event occurred on the corresponding pin.
 * A set bit means an interrupt was triggered.
 *
 * @{
 */
#define INTF_INT0 0   /**< Interrupt flag for IO0. */
#define INTF_INT1 1   /**< Interrupt flag for IO1. */
#define INTF_INT2 2   /**< Interrupt flag for IO2. */
#define INTF_INT3 3   /**< Interrupt flag for IO3. */
#define INTF_INT4 4   /**< Interrupt flag for IO4. */
#define INTF_INT5 5   /**< Interrupt flag for IO5. */
#define INTF_INT6 6   /**< Interrupt flag for IO6. */
#define INTF_INT7 7   /**< Interrupt flag for IO7. */
/** @} */

/**
 * @name INTCAP Register Bitfields
 * @brief Interrupt capture values for MCP230xx I/O pins.
 *
 * These bits reflect the logic level of the corresponding pins at the time
 * the interrupt occurred. Useful for reading the pin state that triggered
 * the interrupt.
 *
 * @{
 */
#define INTCAP_ICP0 0   /**< Interrupt capture for IO0. */
#define INTCAP_ICP1 1   /**< Interrupt capture for IO1. */
#define INTCAP_ICP2 2   /**< Interrupt capture for IO2. */
#define INTCAP_ICP3 3   /**< Interrupt capture for IO3. */
#define INTCAP_ICP4 4   /**< Interrupt capture for IO4. */
#define INTCAP_ICP5 5   /**< Interrupt capture for IO5. */
#define INTCAP_ICP6 6   /**< Interrupt capture for IO6. */
#define INTCAP_ICP7 7   /**< Interrupt capture for IO7. */
/** @} */


/**
 * @name GPIO and Output Latch Bits
 * @brief Bit definitions for general-purpose I/O (GPIO) and output latches (OLAT) in MCP230xx.
 *
 * Each bit corresponds to a single I/O pin:
 * - GPIO: Read the current logic level of the pin.
 * - OLAT: Write the output value for the pin (only effective if configured as output).
 *
 * @{
 */
#define GPIO_GP0 0   /**< GPIO/OLAT bit for IO0. */
#define GPIO_GP1 1   /**< GPIO/OLAT bit for IO1. */
#define GPIO_GP2 2   /**< GPIO/OLAT bit for IO2. */
#define GPIO_GP3 3   /**< GPIO/OLAT bit for IO3. */
#define GPIO_GP4 4   /**< GPIO/OLAT bit for IO4. */
#define GPIO_GP5 5   /**< GPIO/OLAT bit for IO5. */
#define GPIO_GP6 6   /**< GPIO/OLAT bit for IO6. */
#define GPIO_GP7 7   /**< GPIO/OLAT bit for IO7. */
/** @} */

/**
 * @name OLAT Register Bitfields
 * @brief Bit positions for each I/O pin in the MCP23008 OLAT (Output Latch) register.
 *
 * These definitions can be used to manipulate or read the logical output state
 * of each GPIO pin through the OLAT or GPIO register.
 *
 * @note Each bit corresponds directly to one I/O pin (GP0–GP7).
 *
 * @{
 */

#define OLAT_OL0 0  /**< Output Latch bit for IO0 (GP0). */
#define OLAT_OL1 1  /**< Output Latch bit for IO1 (GP1). */
#define OLAT_OL2 2  /**< Output Latch bit for IO2 (GP2). */
#define OLAT_OL3 3  /**< Output Latch bit for IO3 (GP3). */
#define OLAT_OL4 4  /**< Output Latch bit for IO4 (GP4). */
#define OLAT_OL5 5  /**< Output Latch bit for IO5 (GP5). */
#define OLAT_OL6 6  /**< Output Latch bit for IO6 (GP6). */
#define OLAT_OL7 7  /**< Output Latch bit for IO7 (GP7). */

/** @} */

class MCP23008{
	uint8_t _address;
public:
	MCP23008(uint8_t address);
	~MCP23008();

	void init();
	
	uint8_t read_byte_blocking(uint8_t reg);

	void write_byte_blocking(uint8_t reg,uint8_t val);
};


#endif // _MCP230XX_H_