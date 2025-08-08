#ifndef _TAK_REGULATE_TEMP_H_
#define _TAK_REGULATE_TEMP_H_

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
// Temperature regulation task:
// Uses global temperature variables to control Peltier elements
// by switching their pins HIGH or LOW based on hysteresis thresholds.

void taskRegulateTemp(void*pvParameters);

#endif // _TAK_REGULATE_TEMP_H_
