#ifndef _TASK_UPDATE_SCREEN_H_
#define _TASK_UPDATE_SCREEN_H_

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

#include "DrinkCreator6000_Config.h"

//////////////////////////////////////////////////////////////////
// Screen update task:
// Consumer of screen data queue, handles LCD display refresh,
// custom characters, and cursor control with I2C bus synchronization - mux_I2CLock

void taskUpdateScreen(void*pvParameters);

#endif // _TASK_UPDATE_SCREEN_H_
