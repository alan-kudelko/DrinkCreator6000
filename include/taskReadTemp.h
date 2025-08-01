#ifndef _TAK_READ_TEMP_H
#define _TAK_READ_TEMP_H

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

#include "DrinkCreator6000_Config.h"

void taskReadTemp(void*pvParameters);

#endif // _TAK_READ_TEMP_H
