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
#define BUZZER_PIN             PB5 /**< Buzzer control pin */
#define TEMPDATA_RDY           PB6 /**< ATmega328p can send thermometer data pin */
/** @} */

/** @name Port C Pin Definitions
 *  @{
 */
#define DS_PIN                 PC0 /**< Data Strobe or related sensor pin */
#define ST_PIN                 PC1 /**< Shift register STrobe pin */
#define SH_PIN                 PC2 /**< Shift register Shift pin */
#define OE_PIN                 PC3 /**< Shift register Output Enable pin */
/** @} */

/** @name Port D Pin Definitions
 *  @{
 */
#define I2C_SCL_PIN            PD0 /**< I2C Clock line */
#define I2C_SDA_PIN            PD1 /**< I2C Data line */
#define KEYBOARD_INT_PIN       PD2 /**< Keyboard interrupt input pin */
#define KEYBOARD_RESET_PIN     PD3 /**< MCP23008 ic reset pin */
#define PELTIER1_PIN           PD4 /**< First Peltier module control pin */
#define PELTIER2_PIN           PD5 /**< Second Peltier module control pin */
#define LED_RING_DI            PD6 /**< LED ring data in pin */
#define LED_RING DO            PD7 /**< LED ring data out pin */
/** @} */

/** @name Port E Pin Definitions
 *  @{
 */
#define UART0_RX_PIN           PE0 /**< UART0 Receive pin */
#define UART0_TX_PIN           PE1 /**< UART0 Transmit pin */

#define FANS_PIN               PE3 /**< Fans control pin */
#define CIRCULATION_PUMP_PIN   PE4 /**< Circulation pump control pin */
/** @} */

#endif // _DRINK_CREATOR6000_PINS_H_