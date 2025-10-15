/**
 * @file taskSelectDrink.c
 * @brief Implementation of taskSelectDrink.h
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */

#include <string.h>
#include <stdio.h>

#include <taskSelectDrink.h>
#include <DrinkCreator6000_Config_C.h>

uint8_t taskSelectDrink_ProcessContext(uint8_t*keyboardData,volatile struct sUIContext*UI_context){
    if((*keyboardData&E_LWHITE_BUTTON)==E_LWHITE_BUTTON){
        UI_context->currentSubMenu--;

        if(UI_context->currentSubMenu==DRINK_COUNT){
            UI_context->currentSubMenu=DRINK_COUNT-1;
        }
        return 0;
    }
    else if((*keyboardData&E_RWHITE_BUTTON)==E_RWHITE_BUTTON){
        UI_context->currentSubMenu++;
        if(UI_context->currentSubMenu>=DRINK_COUNT){
            UI_context->currentSubMenu=0;
        }
        return 0;
    }
    else if((*keyboardData&E_BLUE_BUTTON)==E_BLUE_BUTTON){
        taskENTER_CRITICAL();
        UI_context->currentTask+=2;
        UI_context->currentMenu=0;
        UI_context->currentSubMenu=0;
        taskEXIT_CRITICAL();
        return 1; // Change task
    }
    else if((*keyboardData&E_GREEN_BUTTON)==E_GREEN_BUTTON){
        taskENTER_CRITICAL();
        UI_context->currentTask++;
        UI_context->currentMenu=0;
        //UI_context->currentSubMenu=0;
        taskEXIT_CRITICAL();
        return 1; // Change task
    }    
    else{
        return 0; // Invalid input
    }
}

void taskSelectDrink(void*pvParameters){ 
    uint32_t f_run=0;
    uint32_t changeTask=0; // 1 if there is a change of task, 0 if not
    uint8_t keyboardData=0;
  
    uint8_t currentScroll=0;
    uint8_t i=0;
    struct sScreenData screenData={0};
    uint16_t usedIngredients[8][2]={0};
    uint8_t noIngredients=0;
    // Used for storing only existing drink values for display
  
    for(;;){
        if(xTaskNotifyWait(0,0,&f_run,pdMS_TO_TICKS(TASK_SELECT_DRINK_REFRESH_RATE))>0){
	          if(f_run==1){
                changeTask=0;
                currentScroll=0;
                noIngredients=0;
        
                memset(&screenData,0,sizeof(screenData));
                sprintf(screenData.lines[0],"[%2d]%s",UI_Context.currentSubMenu+1,drink[UI_Context.currentSubMenu].drinkName);		  
                for(i=0;i<8;i++){
                    if(drink[UI_Context.currentSubMenu].ingredients[i]>0){
                        usedIngredients[noIngredients][0]=drink[UI_Context.currentSubMenu].ingredients[i];
                        usedIngredients[noIngredients][1]=i;
                        noIngredients++;
                    }
                }
            }
        }
        if(f_run==1){
            memset(screenData.lines[1],0,sizeof(screenData.lines[0])*3);
            // If there are more ingredients than lines, lines need to be scrolled
            if(noIngredients>LCD_HEIGHT-1){
                for(i=0;i<3;i++){
                    snprintf(screenData.lines[i+1],14,"%s",ingredients[usedIngredients[i+currentScroll][1]]);
                    snprintf(screenData.lines[i+1]+14,6,"%3dml",usedIngredients[i+currentScroll][0]);
                }
                currentScroll++;
                if(currentScroll>(noIngredients-(LCD_HEIGHT-1))){
                    currentScroll=0;
                }
            }
            else{
                for(i=0;i<noIngredients;i++){
                    snprintf(screenData.lines[i+1],14,"%s",ingredients[usedIngredients[i][1]]);
                    snprintf(screenData.lines[i+1]+14,6,"%3dml",usedIngredients[i][0]);
                }
            }

            xQueueSend(qScreenData,&screenData,pdMS_TO_TICKS(50));

            // Read data from keyboard
            if(xQueueReceive(qKeyboardData,&keyboardData,0)==pdTRUE){
                changeTask=taskSelectDrink_ProcessContext(&keyboardData,&UI_Context);
                if(changeTask==1){
                    xTaskNotify(taskHandles[TASK_SELECT_DRINK],0,eSetValueWithOverwrite); // Notify self to terminate
                }
                else{
                    xTaskNotify(taskHandles[TASK_SELECT_DRINK],1,eSetValueWithOverwrite); // Notify self to process keyboard data
                }
            }
            xTaskNotify(taskHandles[TASK_MAIN],changeTask,eSetValueWithOverwrite); // Notify main to change context
        }
    }
}
