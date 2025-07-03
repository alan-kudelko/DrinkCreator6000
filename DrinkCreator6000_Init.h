#ifndef _DRINK_CREATOR6000_INIT_H_
#define _DRINK_CREATOR6000_INIT_H_

#include "DrinkCreator6000_Config.h"

extern void setInputFlag();

extern const uint8_t fullSquare[];

//////////////////////////////////////////////////////////////////
// IO initialization: 
// Configures I/O pins and attaches interrupts related to pin events
void initializeIO(void);

//////////////////////////////////////////////////////////////////
// Memory initialization:
// Static allocation of stacks, queues, semaphores, and mutexes
void initializeMemory(void);

//////////////////////////////////////////////////////////////////
// Hardware initialization:
// Initializes Serial, LCD display, I²C keyboard (MCP23017), 
// shift register (74HC595), and temperature sensor
void initializeHardware(void);

#endif // _DRINK_CREATOR6000_INIT_H_
