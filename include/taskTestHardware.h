// Doxy

#ifndef TASK_TEST_HARDWARE_H_
    #define TASK_TEST_HARDWARE_H_

#include <stdint.h>

uint8_t taskTestHardware_ProcessContext(uint8_t*keyboardData,volatile struct sUIContext*UI_context);

void testHardware_TestPumps_Menu_N(sScreenData*screenData,volatile sUIContext*UI_context,uint32_t*notification,uint8_t*mask);

void testHardware_TestMisc_Menu_N(sScreenData*screenData,volatile sUIContext*UI_context,uint32_t*notification,uint8_t*mask);

void taskTestHardware(void*pvParameters);

#endif // TASK_TEST_HARDWARE_H_