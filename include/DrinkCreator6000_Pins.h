/**
 * @file DrinkCreator6000_Pins.h
 * @brief IO Mapping: Pin definitions and button mappings
 *
 * This header defines the mapping between microcontroller ports and
 * the hardware components (buttons, sensors, actuators) used in the project.
 * 
 * The pins are grouped by their respective ports.
 */
#ifndef _DRINK_CREATOR6000_PINS_H_
    #define _DRINK_CREATOR6000_PINS_H_

/** @name Port B Pin Definitions
 *  @{
 */
#define KEYBOARD_INT_PIN       PB0 /**< Keyboard interrupt input pin */

#define PELTIER2_PIN           PB4 /**< Second Peltier module control pin */
#define BUZZER_PIN             PB5 /**< Buzzer control pin */
/** @} */

/** @name Port D Pin Definitions
 *  @{
 */
#define I2C_SCL_PIN            PD0 /**< I2C Clock line */
#define I2C_SDA_PIN            PD1 /**< I2C Data line */
/** @} */

/** @name Port E Pin Definitions
 *  @{
 */
#define UART0_RX_PIN           PE0 /**< UART0 Receive pin */
#define UART0_TX_PIN           PE1 /**< UART0 Transmit pin */
#define DS_PIN                 PC0 /**< Data Strobe or related sensor pin */
#define ST_PIN                 PC1 /**< Shift register STrobe pin */
#define SH_PIN                 PC2 /**< Shift register Shift pin */
#define CIRCULATION_PUMP_PIN   PE5 /**< Circulation pump control pin */
#define FANS_PIN               PE6 /**< Fans control pin */
#define PELTIER1_PIN           PE7 /**< First Peltier module control pin */
/** @} */

/** @note Check for pin conflicts on PCB before finalizing the design */

#endif // _DRINK_CREATOR6000_PINS_H_