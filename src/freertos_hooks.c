#include <FreeRTOS.h>
#include <task.h>
#include <avr/io.h>

void vApplicationTickHook(void){
    // Diagnostics of the system's clock
    // PORTC^=(1<<PC5);
}