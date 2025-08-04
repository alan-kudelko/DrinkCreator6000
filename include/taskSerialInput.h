#ifndef _TASK_SERIAL_INPUT_H_
#define _TASK_SERIAL_INPUT_H_

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

void taskSerialInput(void*pvParameters);

#endif // _TASK_SERIAL_INPUT_H_
