#ifndef _DRINK_CREATOR_6000_TASKS_H_
#define _DRINK_CREATOR_6000_TASKS_H_

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

#include "taskErrorHandler.h"          // 0
#include "taskSerialSystemDebugger.h"  // 1
#include "taskMain.h"                  // 2
#include "taskReadInput.h"             // 3
#include "taskSerialInput.h"           // 4
#include "taskUpdateScreen.h"          // 5
#include "taskReadTemp.h"              // 6
#include "taskRegulateTemp.h"          // 7
#include "taskSelectDrink.h"           // 8
#include "taskOrderDrink.h"            // 9
#include "taskShowSystemInfo.h"        // 10
#include "taskWelcomeScreen.h"         // 11

#endif // _DRINK_CREATOR_6000_TASKS_H_