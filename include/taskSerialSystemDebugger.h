#ifndef _TASK_SERIAL_SYSTEM_DEBUGGER_H_
    #define _TASK_SERIAL_SYSTEM_DEBUGGER_H_

extern "C" void updateMemoryUsage();
extern "C" void ram_dump();
void taskSerialSystemDebugger(void*pvParameters);

#endif // _TASK_SERIAL_SYSTEM_DEBUGGER_H_
