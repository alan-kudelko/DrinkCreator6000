#ifndef _TASK_MAIN_H_
#define _TASK_MAIN_H_

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

#include "DrinkCreator6000_Config.h"

void taskMain_ProcessContext_Task_WelcomeScreen();

void taskMain_ProcessContext_taskSelectDrink();

void taskMain_ProcessContext_taskOrderDrink();

void taskMain_ProcessContext_taskShowSystemInfo();

void taskMain(void*pvParameters);

#endif // _TASK_MAIN_H_
