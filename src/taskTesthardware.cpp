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
            mask=0x00;
        }
        if(f_run==1){
            if(UI_Context.currentMenu>3){
                UI_Context.currentMenu=0;
            }
            switch(UI_Context.currentMenu){

            }
            screenData.lines[0][0]='A';
            xQueueSend(qScreenData,&screenData,pdMS_TO_TICKS(50));
        }
        vTaskDelay(pdMS_TO_TICKS(300));
    }
}
