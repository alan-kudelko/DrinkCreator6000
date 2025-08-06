#ifndef _DRINK_CREATOR6000_PINS_H_
    #define _DRINK_CREATOR6000_PINS_H_

// This project is intended to run on the ATmega2560 in a TQFP-64 package.
// Therefore, all pin definitions are based on the ATmega2560 pinout.
// However, development and testing are currently being performed on an ATmega2561,
// so the actual pin mappings may correspond to the ATmega2561 where applicable.

//////////////////////////////////////////////////////////////////
// IO Mapping:
// Pin definitions and button mappings  
// Address of MCP23017 I²C expander

// Port B
#define KEYBOARD_INT_PIN       PB0

#define PELTIER2_PIN           PB4
#define BUZZER_PIN             PB5

// Port D
#define I2C_SCL_PIN            PD0
#define I2C_SDA_PIN            PD1

// Port E
#define UART0_RX_PIN           PE0
#define UART0_TX_PIN           PE1
#define DS_PIN                 PE2
#define ST_PIN                 PE3
#define SH_PIN                 PE4
#define CIRCULATION_PUMP_PIN   PE5
#define FANS_PIN               PE6
#define PELTIER1_PIN           PE7

//Check for conflicts on PCB

#endif // _DRINK_CREATOR6000_PINS_H_