#ifndef _DRINK_CREATOR6000_PINS_H_

#include <DrinkCreator6000_Config_C.h>

// This project is intended to run on the ATmega2560 in a TQFP-64 package.
// Therefore, all pin definitions are based on the ATmega2560 pinout.
// However, development and testing are currently being performed on an ATmega2561,
// so the actual pin mappings may correspond to the ATmega2561 where applicable.

#ifdef __AVR_ATmega2560__

#warning "Using ATmega2560 pin definitions"


// Port B
#define KEYBOARD_INT_PIN     PB0

#define PELTIER2_PIN         PB4
#define BUZZER_PIN           PB5

#define UART0_RX_PIN         PE0
#define UART0_TX_PIN         PE1
#define DS_PIN               PE2
#define ST_PIN               PE3
#define SH_PIN               PE4
#define CIRCULATION_PUMP_PIN PE5
#define FANS_PIN             PE6
#define PELTIER1_PIN         PE7

#define I2C_SCL_PIN          PD0
#define I2C_SDA_PIN          PD1

//Check for conflicts on PCB

#endif // __atmega2560__

//////////////////////////////////////////////////////////////////

#ifdef __AVR_ATmega2561__

#warning "Using ATmega2561 pin definitions"

#define KEYBOARD_INT_PIN PB0

#endif // __atmega2561__

#endif // _DRINK_CREATOR6000_PINS_H_