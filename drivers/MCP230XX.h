/**
 * @file MCP230XX.h
 * @brief Class interface and register definitions for MCP23017/MCP23008 I²C GPIO expanders.
 *
 * This header combines low-level register address definitions with a high-level
 * C++ class that provides convenient access to MCP23x17 (16-bit) and MCP23x08 (8-bit)
 * I²C GPIO expanders.
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
 *  - MCP23017 (two 8-bit ports, 16 GPIOs)
 *  - MCP23008 (single 8-bit port, 8 GPIOs)
 *
 * Typical applications:
 *  - Expanding MCU I/O lines for LEDs, buttons, relays, sensors
 *  - Interrupt-driven input monitoring
 *  - Driving displays and peripheral devices
 */

#ifndef _MCP230XX_H_
	#define _MCP230XX_H_

#include <stdint.h>
/**
 * @name IOCON Register Bitfields
 * @brief Configuration bits for the IOCON register.
 *
 * These bits control addressing, interrupt behavior, and output modes.
 * The register is accessible at two mirrored addresses (0x0A, 0x0B).
 *
 * @{
 */
#define IOCON_BANK     7   /**< Controls register addressing scheme (1 = separate banks, 0 = sequential). */
#define IOCON_MIRROR   6   /**< INT pins mirror (1 = INTA and INTB are internally connected). */
#define IOCON_SEQOP    5   /**< Sequential operation disable (1 = disable auto-increment). */
#define IOCON_DISSLW   4   /**< Slew rate control for SDA output (1 = disabled, 0 = enabled). */
#define IOCON_HAEN     3   /**< Hardware addressing enable (MCP23S17 only, ignored on MCP23017). */
#define IOCON_ODR      2   /**< Configures INT pin as open-drain (1) or active driver (0). */
#define IOCON_INTPOL   1   /**< Interrupt polarity (1 = active-high, 0 = active-low). */
/** @} */

/**
 * @name IOCON Register Addresses
 * @brief The IOCON register can be accessed at two mirrored addresses.
 *
 * Both addresses map to the same internal register with identical bitfields.
 * Useful for auto-increment addressing modes.
 *
 * @{
 */
#define MCP_IOCONA 0x0A    /**< IOCON register address A. */
#define MCP_IOCONB 0x0B    /**< IOCON register address B. */
/** @} */
#define MCP_IODIRA   0x00
#define MCP_IODIRB   0x01
#define MCP_IPOLA    0x02
#define MCP_IPOLB    0x03
#define MCP_GPINTENA 0x04
#define MCP_GPINTENB 0x05
#define MCP_DEFVALA  0x06
#define MCP_DEFVALB  0x07
#define MCP_INTCONA  0x08
#define MCP_INTCONB  0x09
#define MCP_IOCONA   0x0A
#define MCP_IOCONB   0x0B
#define MCP_GPPUA    0x0C
#define MCP_GPPUB    0x0D
#define MCP_INTFA    0x0E
#define MCP_INTFB    0x0F
#define MCP_INTCAPA  0x10
#define MCP_INTCAPB  0x11
#define MCP_GPIOA    0x12
#define MCP_GPIOB    0x13
#define MCP_OLATA    0x14
#define MCP_OLATB    0x15
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
 * @brief Configuration options for MCP230xx I/O expanders.
 *
 * The IOCON register controls global device settings, including addressing mode,
 * interrupt behavior, and hardware options. Note that some bits (e.g., HAEN) are
 * only relevant for the MCP23S17 (SPI variant) and ignored on MCP23017.
 *
 * @{
 */
#define IOCON_BANK     7   /**< Controls register addressing scheme (0 = sequential, 1 = banked). */
#define IOCON_MIRROR   6   /**< INT pins mirror: 1 = INTA=INTB, 0 = separate pins. */
#define IOCON_SEQOP    5   /**< Sequential operation disable: 1 = sequential operations disabled. */
#define IOCON_DISSLW   4   /**< Slew rate control for SDA output (1 = disabled). */
#define IOCON_HAEN     3   /**< Hardware addressing enable (MCP23S17 only). */
#define IOCON_ODR      2   /**< INT pin as open-drain (1) or active driver (0). */
#define IOCON_INTPOL   1   /**< INT pin polarity: 1 = active-high, 0 = active-low. */
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
#define INTCAP_IC0 0   /**< Interrupt capture for IO0. */
#define INTCAP_IC1 1   /**< Interrupt capture for IO1. */
#define INTCAP_IC2 2   /**< Interrupt capture for IO2. */
#define INTCAP_IC3 3   /**< Interrupt capture for IO3. */
#define INTCAP_IC4 4   /**< Interrupt capture for IO4. */
#define INTCAP_IC5 5   /**< Interrupt capture for IO5. */
#define INTCAP_IC6 6   /**< Interrupt capture for IO6. */
#define INTCAP_IC7 7   /**< Interrupt capture for IO7. */
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

#define MCP23017 1
#define MCP23008 0

class MCP230XX{
	uint8_t _address;
	uint8_t _chip;
public:
	MCP230XX(uint8_t address,uint8_t chip);
	~MCP230XX();
	
	uint8_t read_byte_blocking(uint8_t reg);

};


#endif // _MCP230XX_H_