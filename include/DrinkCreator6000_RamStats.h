#ifndef _DRINK_CREATOR6000_RAM_STATS_H_
    #define _DRINK_CREATOR6000_RAM_STATS_H_

#include <stdint.h>

//////////////////////////////////////////////////////////////////
// RAM-related symbols and global memory:

#ifdef __cplusplus
    extern "C"{
#endif  
extern uint16_t __data_start; // Provided by linker
extern uint16_t __data_end;   // Provided by linker

extern uint16_t __bss_start; // Provided by linker
extern uint16_t __bss_end;   // Provided by linker

extern uint16_t __tdat_start; // Provided by linker
extern uint16_t __tdat_end;   // Provided by linker
extern uint16_t __tdat_size;  // Calculated

extern uint16_t __heap_start; // Provided by linker
extern void*__brkval; // Managed by malloc

extern void* __heap_end; // Calculated
extern uint8_t*__stack_ptr; // Initialized by SP

extern uint16_t __heap_size; // Calculated
extern uint16_t __stack_size; // Calculated

extern uint16_t ram_total_free; // Calculated
extern uint16_t ram_in_use;     // Calculated
extern uint16_t ram_size; // Calculated

extern void initRamSize(void) __attribute__((section(".init8")));

extern void updateMemoryUsage(void);

#ifdef __cplusplus
    }
#endif

#endif // _DRINK_CREATOR6000_RAM_STATS_H_