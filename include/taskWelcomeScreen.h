#ifndef _TASK_WELCOME_SCREEN_H_
#define _TASK_WELCOME_SCREEN_H_

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

void taskWelcomeScreen(void*pvParameters);

#endif // _TASK_WELCOME_SCREEN_H_