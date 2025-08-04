#ifndef _TASK_ERROR_HANDLER_H_
#define _TASK_ERROR_HANDLER_H_

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

void taskErrorHandler(void*pvParameters);

#endif // _TASK_ERROR_HANDLER_H_
