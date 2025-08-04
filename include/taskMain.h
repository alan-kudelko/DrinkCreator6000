#ifndef _TASK_MAIN_H_
#define _TASK_MAIN_H_

#include <Arduino.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <timers.h>
#include <FreeRTOSConfig.h>
#include <portable.h>
#include <portmacro.h>
#include <semphr.h>

#include "DrinkCreator6000_Config.h"

void taskMain_ProcessScrollButtons(uint8_t*keyboardInput,volatile sUIContext*UI_Context);

void taskMain_ProcessContext_Task_WelcomeScreen(uint8_t*keyboardInput,volatile sUIContext*UI_Context);

void taskMain_ProcessContext_taskSelectDrink(uint8_t*keyboardInput,volatile sUIContext*UI_Context);

void taskMain_ProcessContext_taskOrderDrink(uint8_t*keyboardInput,volatile sUIContext*UI_Context);

void taskMain_ProcessContext_taskShowSystemInfo(uint8_t*keyboardInput,volatile sUIContext*UI_Context);

void taskMain(void*pvParameters);

#endif // _TASK_MAIN_H_
