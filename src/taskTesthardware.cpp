#include <stdio.h>
#include <string.h>

#include <DrinkCreator6000_Config_C.h>
#include <DrinkCreator6000_Progmem.h>
#include <timers.h>
#include <portable.h>
#include <portmacro.h>
#include <board_io.h>

#include <taskTestHardware.h>

uint8_t taskTestHardware_ProcessContext(uint8_t*keyboardData,volatile struct sUIContext*UI_context){
    if((*keyboardData&E_LWHITE_BUTTON)==E_LWHITE_BUTTON){
        UI_context->currentSubMenu--;
        switch(UI_context->currentMenu){
            default:
                UI_context->currentSubMenu=0;
        }
        return 0;
    }
    else if((*keyboardData&E_RWHITE_BUTTON)){
        switch(UI_context->currentMenu){
            default:
                UI_context->currentSubMenu=0;
        }
        return 0;
    }
    else if((*keyboardData&E_BLUE_BUTTON)==E_BLUE_BUTTON){
        UI_context->currentMenu++;
        if(UI_context->currentMenu>2){
            taskENTER_CRITICAL();
            UI_context->currentTask=0;
            UI_context->currentMenu=0;
            UI_context->currentSubMenu=0;
            taskEXIT_CRITICAL();
            return 1; // Change task
        }
    }
    else if((*keyboardData&E_RED_BUTTON)==E_RED_BUTTON){
        taskENTER_CRITICAL();
        UI_context->currentMenu--;
        UI_context->currentSubMenu=0;
        taskEXIT_CRITICAL();
        if(UI_context->currentMenu>2){
            taskENTER_CRITICAL();
            UI_context->currentTask--;
            UI_context->currentMenu=0;
            taskEXIT_CRITICAL();
            return 1; // Change task
        }
        return 0;
    }
    

    return 0;
}


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

    strncpy(screenData->lines[0],"HW Testing Mode",LCD_WIDTH);
    strncpy(screenData->lines[1],"Pumps Test Menu",LCD_WIDTH);
    strncpy(screenData->lines[2],"Pump ID: 01234567",LCD_WIDTH);
    strncpy(screenData->lines[3],"Status: ",LCD_WIDTH);

    for(i=0;i<8;i++){
        screenData->lines[3][i+9]=(((*mask)&(1<<i))>0)+'0';
    }
    screenData->lcdCursorX=UI_context->currentSubMenu+9;
    screenData->lcdCursorY=2;
    screenData->lcdCursorBlink=1;
}

void testHardware_TestMisc_Menu_N(sScreenData*screenData,volatile sUIContext*UI_context,uint32_t*notification,uint8_t*mask){
    uint8_t i=0;
    if(UI_context->currentSubMenu>3){
        UI_context->currentSubMenu=0;
    }

    if(UI_context->currentSubMenu>2){
        UI_context->currentSubMenu=2;
    }
    if((*notification&2)==2){
        xTaskNotify(taskHandles[TASK_TEST_HARDWARE],1,eSetValueWithOverwrite);
        *mask^=(1<<UI_context->currentSubMenu);
    }

    strncpy(screenData->lines[0],"HW Testing Mode",LCD_WIDTH);
    strncpy(screenData->lines[1],"Misc Test Menu",LCD_WIDTH);
    strncpy(screenData->lines[2],"Cooler",LCD_WIDTH);
    strncpy(screenData->lines[3],"Status: ",LCD_WIDTH);

    screenData->lcdCursorX=0;
    screenData->lcdCursorY=UI_context->currentSubMenu+2;
    screenData->lcdCursorBlink=1;
}
// Willb be moved to seperate task
void testHardware_TestCleaning_Menu_N(sScreenData*screenData,volatile sUIContext*UI_context,uint32_t*notification,uint8_t*mask){
    // Cleaning FSM should be here
    if(UI_context->currentSubMenu>10){
        UI_context->currentSubMenu=0;
    }
    if(UI_context->currentSubMenu>9){
        UI_context->currentSubMenu=0;
    }

    strncpy(screenData->lines[0],"Cleaning program",LCD_WIDTH);
    strncpy(screenData->lines[1]," /",LCD_WIDTH);
    screenData->lines[1][0]='0'+(UI_context->currentSubMenu);
    screenData->lines[1][2]='0'+CLEANING_CYCLES;
    strncpy_P(screenData->lines[2],cleaningCycles_names[UI_context->currentSubMenu],LCD_WIDTH);
    strncpy(screenData->lines[3],"Press Green to start",LCD_WIDTH);
}

void taskTestHardware(void*pvParameters){
    // I need FSM for cleaning process
    sHardwareData hardwareData={0};
    uint8_t pumpsMask=0x00;
    uint8_t hardwareMask=0x00;
    uint8_t changeTask=0; // 1 if there is a change of task, 0 if not
    uint8_t keyboardData=0;

    // 0 cooler -> peltiers + pump
    // 1 fans
    uint32_t f_run=0;

    sScreenData screenData{};

    for(;;){
        if(xTaskNotifyWait(0,0,&f_run,0)>0){
            if(f_run==0){
                hardwareData.hardware=HARDWARE_COOLER|HARDWARE_FANS|HARDWARE_CIRCULATION;
                hardwareData.mask=0x00;
                xQueueSend(qHardwareControl,&hardwareData,pdMS_TO_TICKS(100));
                
                hardwareData.hardware=HARDWARE_PUMPS;
                hardwareData.mask=0x00;
                xQueueSend(qHardwareControl,&hardwareData,pdMS_TO_TICKS(100));

                // Should be in separate function
                // Stop all hardware after stopping this task
            }
            else{
                
            }
        }
        if(f_run==1){
            switch(UI_Context.currentMenu){
                case 0:
                    testHardware_TestPumps_Menu_N(&screenData,&UI_Context,&f_run,&pumpsMask);
                    hardwareData.hardware=HARDWARE_PUMPS;
                    hardwareData.mask=pumpsMask;
                    xQueueSend(qHardwareControl,&hardwareData,pdMS_TO_TICKS(100));
                break;
                case 1:
                    testHardware_TestMisc_Menu_N(&screenData,&UI_Context,&f_run,&hardwareMask);
                    hardwareData.hardware=HARDWARE_PUMPS;
                    hardwareData.mask=0x00;
                    xQueueSend(qHardwareControl,&hardwareData,pdMS_TO_TICKS(100));
                    // Testing fans and peltiers
                break;
                default:
                1;
                // Error handling
            }
            if(xQueueReceive(qKeyboardData,&keyboardData,0)==pdTRUE){
                changeTask=taskTestHardware_ProcessContext(&keyboardData,&UI_Context);
                if(changeTask==1){
                    xTaskNotify(taskHandles[TASK_TEST_HARDWARE],0,eSetValueWithOverwrite); // Notify self to terminate
                }
                else{
                    xTaskNotify(taskHandles[TASK_TEST_HARDWARE],1,eSetValueWithOverwrite); // Notify self to process keyboard data
                }
            
            }
            xTaskNotify(taskHandles[TASK_MAIN],changeTask,eSetValueWithOverwrite); // Notify main to change context
            xQueueSend(qScreenData,&screenData,pdMS_TO_TICKS(50));
        }
        vTaskDelay(pdMS_TO_TICKS(300));
    }
}
