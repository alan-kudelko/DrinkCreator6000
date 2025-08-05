#ifndef _DRINK_CREATOR6000_CONFIG_C_H_
#define _DRINK_CREATOR6000_CONFIG_C_H_

#define F_CPU                16000000UL
#define __AVR_ATmega2560__   1
#define UART_BAUD_RATE       9600
#define UBRR_VALUE           ((F_CPU/(16UL*UART_BAUD_RATE))-1)

#endif // _DRINK_CREATOR6000_CONFIG_C_H_
