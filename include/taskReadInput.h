/**
 * @file taskReadInput.h
 * @brief Task responsible for reading input data from MCP23017 via I2C.
 *
 * This task reads keyboard/input data from the MCP23017 I2C I/O expander
 * and sends the data to the main task through the external queue 
 * `qKeyboardData`.
 */
#ifndef _TASK_READ_INPUT_H_
    #define _TASK_READ_INPUT_H_

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <timers.h>
#include <FreeRTOSConfig.h>
#include <portable.h>
#include <portmacro.h>
#include <semphr.h>

#include <DrinkCreator6000_Config.h>
/**
 * @brief Reads input data from MCP23017 and enqueues to main task.
 *
 * @param pvParameters Task parameters (unused).
 */
void taskReadInput(void*pvParameters);

#endif // _TASK_READ_INPUT_H_