#ifndef _TAK_READ_TEMP_H
#define _TAK_READ_TEMP_H

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

void taskReadTemp(void*pvParameters);

#endif // _TAK_READ_TEMP_H
