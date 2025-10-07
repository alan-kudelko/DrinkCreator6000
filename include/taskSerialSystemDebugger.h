/**
 * @file taskSerialSystemDebugger.h
 * @brief Task and utilities for serial system debugging, task diagnostics and RAM usage reporting.
 *
 */

#ifndef TASK_SERIAL_SYSTEM_DEBUGGER_H_
    #define TASK_SERIAL_SYSTEM_DEBUGGER_H_

#ifdef __cplusplus
    extern "C"{
#endif // __cplusplus

/**
 * @brief FreeRTOS task for serial system debugging.
 *
 * This task can periodically output system RAM usage information over UART
 * or respond to requests from other tasks or commands.
 *
 * @param pvParameters Pointer to FreeRTOS task parameters (unused).
 */
void taskSerialSystemDebugger(void*pvParameters);

#ifdef __cplusplus
    }
#endif // __cplusplus

#endif // TASK_SERIAL_SYSTEM_DEBUGGER_H_
