#ifndef _TASK_READ_INPUT_H_
#define _TASK_READ_INPUT_H_

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

#include <Wire.h>

#include "DrinkCreator6000_Config.h"

void taskReadInput(void*pvParameters);

#endif // _TASK_READ_INPUT_H_