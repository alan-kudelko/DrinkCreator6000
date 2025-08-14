#include <FreeRTOS.h>
#include <task.h>
#include <avr/io.h>

void vApplicationIdleHook(void){
    /* This function is called by the FreeRTOS kernel on each tick interrupt. */
}
void vApplicationTickHook(void){
    //PORTH^=(1<<PH4);
}