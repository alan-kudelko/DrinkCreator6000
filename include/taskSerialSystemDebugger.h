// Doxy

#ifndef TASK_SERIAL_SYSTEM_DEBUGGER_H_
    #define TASK_SERIAL_SYSTEM_DEBUGGER_H_

#define RAM_DUMP_BUFFER_LENGTH 5
#define RAM_DUMP_ADDR_FMT      "%04X"
#define RAM_DUMP_SIZE_FMT      "%4d"

#ifdef __cplusplus
    extern "C"{
#endif // __cplusplus

void updateMemoryUsage(void);

/**
 * @brief   Dumps memory usage via UART.
 * @details Prints boundaries and sizes of .data, .bss, .tdat, heap, and stack.
 * @note    Uses blocking UART output.
 */
void ram_dump(void);


void taskSerialSystemDebugger(void*pvParameters);

#ifdef __cplusplus
    }
#endif // __cplusplus

#endif // TASK_SERIAL_SYSTEM_DEBUGGER_H_
