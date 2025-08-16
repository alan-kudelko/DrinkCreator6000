#include <stddef.h>
#include <avr/io.h>
#include <avr/iom2560.h>

#include <DrinkCreator6000_RamStats.h>

uint16_t __tdat_size=0;

void* __heap_end=NULL;
uint8_t*__stack_ptr=NULL;

uint16_t __heap_size=0;
uint16_t __stack_size=0;

uint16_t ram_total_free=0;
uint16_t ram_in_use=0;
uint16_t ram_size=(uint16_t)RAMEND;

void initRamSize(void){
    ram_size=(uint16_t)RAMEND-(uint16_t)&__data_start+1;
}

void updateMemoryUsage(void){
    __heap_end=__brkval?__brkval:(void*)&__heap_start;
    __heap_size=(uint16_t)__heap_end-(uint16_t)&__heap_start;
    __stack_size=(uint16_t)RAMEND-(uint16_t)__stack_ptr;
    __tdat_size=(uint16_t)&__tdat_end-(uint16_t)&__tdat_start;
  
    ram_total_free=(uint16_t)__stack_ptr-(uint16_t)__heap_end;
    ram_in_use=(uint16_t)ram_size-(uint16_t)ram_total_free;
}