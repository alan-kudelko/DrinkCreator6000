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
#include <buzzer.h>

void taskMain(void*pvParameters){
    #define HEART_BEAT_DELAY_MS 1500
    uint32_t notification=0;
    // Default startup task
    xTaskNotify(taskHandles[global_FSM[UI_Context.currentTask]],1,eSetValueWithOverwrite);
  
    for(;;){
        if(xTaskNotifyWait(0,0,&notification,pdMS_TO_TICKS(HEART_BEAT_DELAY_MS))>0){
            // Logic to handle different notifications
            if((notification&0x01)==0x01){
                // Change task
                if(UI_Context.currentTask>=UI_TASKS_COUNT){
                    UI_Context.currentTask=0;
                }
                xTaskNotify(taskHandles[global_FSM[UI_Context.currentTask]],1,eSetValueWithOverwrite);
            }
        }
        else{
            activateBuzzer(0);
            vTaskDelay(pdMS_TO_TICKS(25));
            deactivateBuzzer();
            // No heart beat received, should never happen
            // Probably task is stuck
            // Should trigger error handler
            
        }
    }
}
