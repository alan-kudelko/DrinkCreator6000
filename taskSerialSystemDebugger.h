#ifndef _TASK_SERIAL_SYSTEM_DEBUGGER_H_
#define _TASK_SERIAL_SYSTEM_DEBUGGER_H_

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

#include "DrinkCreator6000_Config.h"

void updateMemoryUsage();
void ram_dump();
void taskSerialSystemDebugger(void*pvParameters);

#endif // _TASK_SERIAL_SYSTEM_DEBUGGER_H_
