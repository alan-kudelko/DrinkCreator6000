#include <stdio.h>
#include <string.h>

#include <DrinkCreator6000_Config_C.h>
#include <timers.h>
#include <portable.h>
#include <portmacro.h>
#include <board_io.h>
#include <buzzer.h>

#include <taskHardwareControl.h>

void taskHardwareControl(void*pvParameters){
    struct sHardwareData hardwareData={0};

    for(;;){
        if(xQueueReceive(qHardwareControl,&hardwareData,pdMS_TO_TICKS(TASK_HARDWARE_CONTROL_MAX_TIMEOUT_MS))==pdTRUE){
            if(hardwareData.hardware&HARDWARE_COOLER){
                if(hardwareData.mask&HARDWARE_MISC_BIT){
                    cooler_on();
                }
                else{
                    cooler_off();
                }
            }
            if(hardwareData.hardware&HARDWARE_FANS){
                if(hardwareData.mask&HARDWARE_MISC_BIT){
                    fans_on();
                }
                else{
                    fans_off();
                }
            }
            if(hardwareData.hardware&HARDWARE_CIRCULATION){
                if(hardwareData.mask&HARDWARE_MISC_BIT){
                    circulation_on();
                }
                else{
                    circulation_off();
                }
            }
            else if(hardwareData.hardware&HARDWARE_PUMPS){
                if(hardwareData.mask==0){
                    pumps_disable();
                    //shiftOut(0);
                }
                else{
                    pumps_enable();
                                        pumps_disable();
                    //shiftOut(hardwareData.mask);
                }
            }
            else{
                // Invalid hardware type - ignore
            }
        }
        else{
            // Timeout - disable all hardware
            // Additional Safety feature
            // There should be separate timeout for pumps and misc hardware
            pumps_disable();
            shiftOut(0);

            cooler_off();
            circulation_off();
            fans_off();
        }
    }
}
