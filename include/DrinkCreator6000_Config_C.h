#ifndef _DRINK_CREATOR6000_CONFIG_C_H_
#define _DRINK_CREATOR6000_CONFIG_C_H_


// Define the CPU frequency and the AVR model
#ifndef F_CPU
    #define F_CPU 16000000UL
#endif

#ifndef __AVR_ATmega2560__
    #define __AVR_ATmega2560__ 1
#endif
// Define the UART baud rate and calculate UBRR value
#define UART_BAUD_RATE       9600
#define UBRR_VALUE           ((F_CPU/(16UL*UART_BAUD_RATE))-1)

// Define the I2C frequency and calculate TWBR value
#define I2C_FREQ       100000UL  // 100 kHz I2C frequency
#define TWBR_VALUE     (((F_CPU/I2C_FREQ)-16UL)/2UL) // Calculate TWBR value for TWI (I2C) speed

// Task count
#define TASK_N 12

#endif // _DRINK_CREATOR6000_CONFIG_C_H_
