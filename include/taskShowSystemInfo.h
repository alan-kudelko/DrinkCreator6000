#ifndef _TASK_SHOW_SYSTEM_INFO_H_
#define _TASK_SHOW_SYSTEM_INFO_H_

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

void updateMemoryUsage();

void showInfo_Firmware_Sub_0(sScreenData*screenData);

void showInfo_Firmware_Sub_1(sScreenData*screenData);

void showInfo_Temp_Sub_0(sScreenData*screenData);

void showInfo_Memory_Sub_N(sScreenData*screenData,sUIContext*UI_Context);

void showInfo_Task_Sub_N(sScreenData*screenData,sUIContext*UI_Context);

void showInfo_Error_Sub_N(sScreenData*screenData,sUIContext*UI_Context);

void showInfo_ConfError_Sub_0(sScreenData*screenData,sUIContext*UI_Context);

void taskShowSystemInfo(void*pvParameters);

#endif // _TASK_SHOW_SYSTEM_INFO_H_
