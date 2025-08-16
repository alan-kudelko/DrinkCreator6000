/**
 * @file taskOrderDrink.h
 * @brief Task definitions and interfaces for controlling drink orders.
 *
 * This header declares the FreeRTOS task responsible for managing drink
 * preparation, including:
 *   - Sequencing pumps according to the selected recipe
 *   - Controlling pump activation duration (pouring time)
 *   - Coordinating task synchronization with other system modules
 */
#ifndef _TASK_ORDER_DRINK_H_
    #define _TASK_ORDER_DRINK_H_

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

void taskOrderDrink(void*pvParameters);

#ifdef __cplusplus
    }
#endif // __cplusplus

#endif // _TASK_ORDER_DRINK_H_