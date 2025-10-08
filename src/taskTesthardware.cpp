#include <stdio.h>
#include <string.h>

#include <DrinkCreator6000_Config_C.h>
#include <timers.h>
#include <portable.h>
#include <portmacro.h>

#include <taskTestHardware.h>

void testPumps(uint8_t*mask){

}

void taskTestHardware(void*pvParameters){
    uint32_t f_run=0;
    uint8_t mask=0x00;

    sScreenData screenData{};

    for(;;){
        if(xTaskNotifyWait(0,0,&f_run,0)>0){
            if((f_run&1)==0){
                // Turn off task
                // Reset hardware
            }
            else{
                
            }
        }
        if(f_run==1){
            if(UI_Context.currentMenu>3){
                UI_Context.currentMenu=0;
            }
            switch(UI_Context.currentMenu){
                case 0:
                screenData.lines[0][0]='A';
                    // Testing pumps
                break;
                case 1:
                screenData.lines[0][0]='B';
                    // Testing fans and peltiers
                break;
                case 2:
                screenData.lines[0][0]='C';
                    // Machine cleaning
                break;
                default:
                1;
                // Error handling
            }
            xQueueSend(qScreenData,&screenData,pdMS_TO_TICKS(50));
        }
        vTaskDelay(pdMS_TO_TICKS(300));
    }
}
