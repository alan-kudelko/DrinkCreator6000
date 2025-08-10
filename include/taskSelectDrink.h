/**
 * @file taskSelectDrink.h
 * @brief Task responsible for selecting a drink to order.
 *
 * This task allows the user to select a drink, then sends
 * the screen update data to the `taskUpdateScreen` task
 * via the external queue `qScreenData`.
 */
#ifndef _TASK_SELECT_DRINK_H_
    #define _TASK_SELECT_DRINK_H_

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
 * @brief Handles the drink selection process.
 *
 * @param pvParameters Task parameters (unused).
 */
void taskSelectDrink(void*pvParameters);

#endif // _TASK_SELECT_DRINK_H_