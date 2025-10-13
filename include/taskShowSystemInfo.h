/**
 * @file taskShowSystemInfo.h
 * @brief Task for displaying system information on the user interface.
 *
 * This task periodically gathers and presents key system metrics, including:
 * - Firmware version and system uptime
 * - RAM usage and per-module memory consumption
 * - Refrigerator temperature and configured setpoints
 * - Last system error
 * - Author of the project
 * - Status of FreeRTOS tasks
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */
#ifndef _TASK_SHOW_SYSTEM_INFO_H_
    #define _TASK_SHOW_SYSTEM_INFO_H_

#include <DrinkCreator6000_DataTypes.h>

void showInfo_Firmware_Sub_0(sScreenData*screenData);

void showInfo_Firmware_Sub_1(sScreenData*screenData);

void showInfo_Temp_Sub_0(sScreenData*screenData);

void showInfo_Memory_Sub_N(sScreenData*screenData,sUIContext*UI_Context);

void showInfo_Task_Sub_N(sScreenData*screenData,sUIContext*UI_Context);

void showInfo_Error_Sub_N(sScreenData*screenData,sUIContext*UI_Context);

void showInfo_ConfError_Sub_0(sScreenData*screenData,sUIContext*UI_Context);

void taskShowSystemInfo(void*pvParameters);

#endif // _TASK_SHOW_SYSTEM_INFO_H_
