/**
 * @file taskMain.c
 * @brief Implementation of taskMain.h
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */

#include <stdbool.h>

#include <taskMain.h>
#include <DrinkCreator6000_Config_C.h>
#include <DrinkCreator6000_EEPROM.h>

// Each operation should update current context
// If one or more field is modified, operation should be atomic
// incrementing and decrementing submenus can be common for all the tasks
// It can be done because each task will control its own submenu values range
// and clip it if necessary
// In some cases I.E specific menus and submenus some additional control logic should be implement here

void taskMain_ProcessScrollButtons(uint8_t*keyboardInput,volatile struct sUIContext*UI_context){
    if((*keyboardInput&E_LWHITE_BUTTON)==E_LWHITE_BUTTON){
        UI_context->currentSubMenu--;
    }
    if((*keyboardInput&E_RWHITE_BUTTON)==E_RWHITE_BUTTON){
        UI_context->currentSubMenu++;
    }
}
void taskMain_ProcessContext_taskSelectDrink(uint8_t*keyboardInput,volatile struct sUIContext*UI_context){
    if((*keyboardInput&E_GREEN_BUTTON)==E_GREEN_BUTTON){
        // Ordering drink with UI_Context change and data send
        taskENTER_CRITICAL();
        UI_context->currentTask=DRINK_ORDER;
        UI_context->currentMenu=0;
        // UI_context->currentSubMenu should remain as it is used to indicate which drink should be ordered
        taskEXIT_CRITICAL();
    
        xTaskNotify(taskHandles[TASK_SELECT_DRINK],0,eSetValueWithOverwrite);
        xTaskNotify(taskHandles[TASK_ORDER_DRINK],1,eSetValueWithOverwrite);
    }
    if((*keyboardInput&(E_LWHITE_BUTTON|E_RWHITE_BUTTON))){
        taskMain_ProcessScrollButtons(keyboardInput,UI_context);
        xTaskNotify(taskHandles[TASK_SELECT_DRINK],1,eSetValueWithOverwrite);
    }
    if((*keyboardInput&E_BLUE_BUTTON)==E_BLUE_BUTTON){
        taskENTER_CRITICAL();
        UI_context->currentTask=SHOW_INFO;
        UI_context->currentMenu=0;
        UI_context->currentSubMenu=0;
        taskEXIT_CRITICAL();
    
        xTaskNotify(taskHandles[TASK_SHOW_SYS_INFO],1,eSetValueWithOverwrite);
        xTaskNotify(taskHandles[TASK_SELECT_DRINK],0,eSetValueWithOverwrite);
    }
}
void taskMain_ProcessContext_taskOrderDrink(uint8_t*keyboardInput,volatile struct sUIContext*UI_context){
    // if red button is pushed, there should be some kind of information that processed was aborted
    // of course all the pumps should be stopped
    if((*keyboardInput&E_RED_BUTTON)==E_RED_BUTTON){
        taskENTER_CRITICAL();
        UI_context->currentTask=DRINK_SELECT;
        UI_context->currentMenu=0;
        UI_context->currentSubMenu=0;
        taskEXIT_CRITICAL();
    
        xTaskNotify(taskHandles[TASK_ORDER_DRINK],2,eSetValueWithOverwrite);
        //xTaskNotify(taskHandles[TASK_SELECT_DRINK],1,eSetValueWithOverwrite); is send in taskOrderDrink
        //Might change this in the future, because main should be responsible for everything  
    }
}
void taskMain_ProcessContext_taskShowSystemInfo(uint8_t*keyboardInput,volatile struct sUIContext*UI_context){
  // UI_contect->currentTask==SHOW_INFO
    if((lastSystemError.confirmed==0)&&(UI_context->currentMenu==4)){
        if((*keyboardInput&E_GREEN_BUTTON)==E_GREEN_BUTTON){
            lastSystemError.confirmed=1;
            EEPROMUpdateLastStartupError(&lastSystemError);
      
            taskENTER_CRITICAL();
            UI_context->currentMenu=5;
            UI_context->currentSubMenu=0;
            UI_context->autoScrollEnable=0;
            taskEXIT_CRITICAL();
        }
    }
    if((*keyboardInput&E_BLUE_BUTTON)==E_BLUE_BUTTON){
        if(UI_context->currentMenu==4){
            taskENTER_CRITICAL();
            UI_context->currentTask=TEST_HARDWARE;
            UI_context->currentMenu=0;
            UI_context->currentSubMenu=0;
            UI_context->autoScrollEnable=0;
            taskEXIT_CRITICAL();
            xTaskNotify(taskHandles[TASK_SHOW_SYS_INFO],0,eSetValueWithOverwrite);
            xTaskNotify(taskHandles[TASK_TEST_HARDWARE],1,eSetValueWithOverwrite);
            return;
        }
        taskENTER_CRITICAL();
        UI_context->currentMenu++;
        UI_context->currentSubMenu=0;
        UI_context->autoScrollEnable=0;
        taskEXIT_CRITICAL();
    }
    if((*keyboardInput&E_RED_BUTTON)==E_RED_BUTTON){
        if(UI_context->currentMenu==0){
            taskENTER_CRITICAL();
            UI_context->currentTask=DRINK_SELECT;
            UI_context->currentMenu=0;
            UI_context->currentSubMenu=0;
            UI_context->autoScrollEnable=0;
            taskEXIT_CRITICAL();
      
            xTaskNotify(taskHandles[TASK_SHOW_SYS_INFO],0,eSetValueWithOverwrite);
            xTaskNotify(taskHandles[TASK_SELECT_DRINK],1,eSetValueWithOverwrite);
            return;
        }
        if(UI_context->currentMenu>0){
            taskENTER_CRITICAL();
            UI_context->currentMenu--;
            UI_context->currentSubMenu=0;
            taskEXIT_CRITICAL();
        }
    }
    taskMain_ProcessScrollButtons(keyboardInput,UI_context);
}
void taskMain_ProcessContext_taskTestHardware(uint8_t*keyboardInput,volatile struct sUIContext*UI_context){
    if(UI_context->currentMenu==0){
        if((*keyboardInput&E_GREEN_BUTTON)==E_GREEN_BUTTON){
            xTaskNotify(taskHandles[TASK_TEST_HARDWARE],11,eSetValueWithOverwrite);
        }
    }
    
    if((*keyboardInput&E_RED_BUTTON)==E_RED_BUTTON){
        if(UI_context->currentMenu==0){
            taskENTER_CRITICAL();
            UI_context->currentTask=SHOW_INFO;
            UI_context->currentMenu=4;
            UI_context->currentSubMenu=0;
            UI_context->autoScrollEnable=0;
            taskEXIT_CRITICAL();
            xTaskNotify(taskHandles[TASK_TEST_HARDWARE],0,eSetValueWithOverwrite);
            xTaskNotify(taskHandles[TASK_SHOW_SYS_INFO],1,eSetValueWithOverwrite);
            return;
        }
        else{
            taskENTER_CRITICAL();
            UI_context->currentMenu--;
            UI_context->currentSubMenu=0;
            UI_context->autoScrollEnable=0;
            taskEXIT_CRITICAL();
        }
    }
    if((*keyboardInput&E_BLUE_BUTTON)==E_BLUE_BUTTON){
        if(UI_context->currentMenu==2){
            taskENTER_CRITICAL();
            UI_context->currentTask=DRINK_SELECT;
            UI_context->currentMenu=0;
            UI_context->currentSubMenu=0;
            UI_context->autoScrollEnable=0;
            taskEXIT_CRITICAL();
            xTaskNotify(taskHandles[TASK_TEST_HARDWARE],0,eSetValueWithOverwrite);
            xTaskNotify(taskHandles[TASK_SELECT_DRINK],1,eSetValueWithOverwrite);
            return;
        }
        taskENTER_CRITICAL();
        UI_context->currentMenu++;
        UI_context->currentSubMenu=0;
        UI_context->autoScrollEnable=0;
        taskEXIT_CRITICAL();
    }
    taskMain_ProcessScrollButtons(keyboardInput,UI_context);
}

void taskMain(void*pvParameters){
    bool f_keyboardDataReceived=false;
  
    uint8_t keyboardData=0;
    // Default startup task
    xTaskNotify(taskHandles[TASK_SELECT_DRINK],1,eSetValueWithoutOverwrite);
  
    for(;;){
        if(xQueueReceive(qKeyboardData,&keyboardData,pdMS_TO_TICKS(portMAX_DELAY))){
	        f_keyboardDataReceived=true;
        }
	    if(f_keyboardDataReceived){
            switch(UI_Context.currentTask){
		        case DRINK_SELECT:
                    taskMain_ProcessContext_taskSelectDrink(&keyboardData,&UI_Context);
		            break;
	            case DRINK_ORDER:
                    taskMain_ProcessContext_taskOrderDrink(&keyboardData,&UI_Context);
		            break;
		        case SHOW_INFO:
		            taskMain_ProcessContext_taskShowSystemInfo(&keyboardData,&UI_Context);
		            break;
                case TEST_HARDWARE:
                    taskMain_ProcessContext_taskTestHardware(&keyboardData,&UI_Context);
                default:
                1;
                // Code to handle potential errors
		        }
    
		        f_keyboardDataReceived=false;
		        // Should be executed only once after received qKeyboardData
	      }
    }
}
