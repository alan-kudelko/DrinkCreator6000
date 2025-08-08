#ifndef _TASK_UPDATE_SCREEN_H_
#define _TASK_UPDATE_SCREEN_H_

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <timers.h>
#include <FreeRTOSConfig.h>
#include <portable.h>
#include <portmacro.h>
#include <semphr.h>

#include "DrinkCreator6000_Config.h"

//////////////////////////////////////////////////////////////////
// Screen update task:
// Consumer of screen data queue, handles LCD display refresh,
// custom characters, and cursor control with I2C bus synchronization - mux_I2CLock

void taskUpdateScreen(void*pvParameters);

#endif // _TASK_UPDATE_SCREEN_H_
