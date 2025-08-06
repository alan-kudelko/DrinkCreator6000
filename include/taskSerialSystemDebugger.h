#ifndef _TASK_SERIAL_SYSTEM_DEBUGGER_H_
#define _TASK_SERIAL_SYSTEM_DEBUGGER_H_

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

extern "C" void updateMemoryUsage();
void ram_dump();
void taskSerialSystemDebugger(void*pvParameters);

#endif // _TASK_SERIAL_SYSTEM_DEBUGGER_H_
