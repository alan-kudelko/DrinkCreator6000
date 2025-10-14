#include <stdio.h>
#include <string.h>

#include <DrinkCreator6000_Config_C.h>
#include <timers.h>
#include <portable.h>
#include <portmacro.h>
#include <board_io.h>

#include <taskHardwareControl.h>

void taskHardwareControl(void*pvParameters){
    uint8_t pumpsMask=0x00;
    uint8_t hardwareMask=0x00;

    // 0 cooler -> peltiers + pump
    // 1 fans
    uint32_t f_run=0;


    for(;;){


        vTaskDelay(pdMS_TO_TICKS(TASK_HARDWARE_CONTROL_REFRESH_RATE));
    }
}
