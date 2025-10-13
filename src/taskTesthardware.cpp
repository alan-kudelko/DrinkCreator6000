#include <stdio.h>
#include <string.h>

#include <DrinkCreator6000_Config_C.h>
#include <timers.h>
#include <portable.h>
#include <portmacro.h>
#include <board_io.h>

#include <taskTestHardware.h>

void testHardware_TestPumps_Menu_N(sScreenData*screenData,volatile sUIContext*UI_context,uint32_t*notification,uint8_t*mask){
    uint8_t i=0;
    if(UI_context->currentSubMenu>8){
        UI_context->currentSubMenu=7;
    }

    if(UI_context->currentSubMenu>7){
        UI_context->currentSubMenu=0;
    }
    if((*notification&2)==2){
        xTaskNotify(taskHandles[TASK_TEST_HARDWARE],1,eSetValueWithOverwrite);
        *mask^=(1<<UI_context->currentSubMenu);
    }

    strncpy(screenData->lines[0],"HW Testing Mode",20);
    strncpy(screenData->lines[1],"Pumps Test Menu",20);
    strncpy(screenData->lines[2],"Pump ID: 01234567",20);
    strncpy(screenData->lines[3],"Status: ",20);

    for(i=0;i<8;i++){
        screenData->lines[3][i+9]=(((*mask)&(1<<i))>0)+'0';
    }
    screenData->lcdCursorX=UI_context->currentSubMenu+9;
    screenData->lcdCursorY=2;
    screenData->lcdCursorBlink=1;
}

void testHardware_TestMisc_Menu_N(sScreenData*screenData,volatile sUIContext*UI_context,uint32_t*notification,uint8_t*mask){
    uint8_t i=0;
    if(UI_context->currentSubMenu>2){
        UI_context->currentSubMenu=0;
    }

    if(UI_context->currentSubMenu>1){
        UI_context->currentSubMenu=0;
    }
    if((*notification&2)==2){
        xTaskNotify(taskHandles[TASK_TEST_HARDWARE],1,eSetValueWithOverwrite);
        *mask^=(1<<UI_context->currentSubMenu);
    }

    strncpy(screenData->lines[0],"HW Testing Mode",20);
    strncpy(screenData->lines[1],"Misc Test Menu",20);
    strncpy(screenData->lines[2],"Cooler",20);
    strncpy(screenData->lines[3],"Status: ",20);

    screenData->lcdCursorX=0;
    screenData->lcdCursorY=UI_context->currentSubMenu+2;
    screenData->lcdCursorBlink=1;
}

void taskTestHardware(void*pvParameters){
    uint8_t pumpsMask=0x00;
    uint8_t hardwareMask=0x00;

    // 0 cooler -> peltiers + pump
    // 1 fans
    uint32_t f_run=0;

    sScreenData screenData{};

    for(;;){
        if(xTaskNotifyWait(0,0,&f_run,0)>0){
            if((f_run&1)==0){
                pumpsMask=0x00;
                hardwareMask=0x00;
                pumps_disable();
                fans_off();
                cooler_off();
                circulation_off();
                shiftOut(pumpsMask);
                // Turn off task
                // Reset hardware
            }
            else{
                
            }
        }
        if((f_run&1)==1){
            if(UI_Context.currentMenu>3){
                UI_Context.currentMenu=0;
            }
            switch(UI_Context.currentMenu){
                case 0:
                    testHardware_TestPumps_Menu_N(&screenData,&UI_Context,&f_run,&pumpsMask);
                    pumps_enable();
                    shiftOut(pumpsMask);
                break;
                case 1:
                    testHardware_TestMisc_Menu_N(&screenData,&UI_Context,&f_run,&hardwareMask);
                    pumpsMask=0x00;
                    pumps_disable();
                    shiftOut(pumpsMask);
                    // Testing fans and peltiers
                break;
                case 2:
                screenData.lines[0][0]='Z';
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
