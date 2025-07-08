#ifndef _DRINK_CREATOR_6000_TASKS_H_
#define _DRINK_CREATOR_6000_TASKS_H_

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

#include "DrinkCreator6000_Config.h"

#endif // _DRINK_CREATOR_6000_TASKS_H_

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
