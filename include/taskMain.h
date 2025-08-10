/**
 * @file taskMain.h
 * @brief Main task coordinating the operation of other system tasks.
 *
 * This task manages the application context and user interface state,
 * delegating input processing to helper functions corresponding to
 * different UI screens or modes.
 */
#ifndef _TASK_MAIN_H_
    #define _TASK_MAIN_H_

#include <DrinkCreator6000_DataTypes.h>

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

/**
 * @brief Processes scroll button input and updates UI context accordingly.
 */
void taskMain_ProcessScrollButtons(uint8_t*keyboardInput,volatile struct sUIContext*UI_Context);
/**
 * @brief Processes input in the Welcome Screen context.
 */
void taskMain_ProcessContext_Task_WelcomeScreen(uint8_t*keyboardInput,volatile struct sUIContext*UI_Context);
/**
 * @brief Processes input in the Select Drink task context.
 */
void taskMain_ProcessContext_taskSelectDrink(uint8_t*keyboardInput,volatile struct sUIContext*UI_Context);
/**
 * @brief Processes input in the Order Drink task context.
 */
void taskMain_ProcessContext_taskOrderDrink(uint8_t*keyboardInput,volatile struct sUIContext*UI_Context);
/**
 * @brief Processes input in the Show System Info task context.
 */
void taskMain_ProcessContext_taskShowSystemInfo(uint8_t*keyboardInput,volatile struct sUIContext*UI_Context);
/**
 * @brief Main FreeRTOS task that coordinates system workflow.
 */
void taskMain(void*pvParameters);

#ifdef __cplusplus
    }
#endif // __cplusplus

#endif // _TASK_MAIN_H_