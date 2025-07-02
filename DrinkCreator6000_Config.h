#ifndef _DRINK_CREATOR6000_CONFIG_H_
#define _DRINK_CREATOR6000_CONFIG_H_

#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <LiquidCrystal_I2C.h>
#include "CustomDataTypes.h"

////////////////////////////////////////////////////////////////// IO MAPPING
//enum {DSPin=PIN_PC0, STPin=PIN_PC1, SHPin=PIN_PC2,OEnable=PIN_PC3,INTPin=PIN_PD2,THERMOMETER_PIN=PIN_PD3,Pelt1Pin=PIN_PD4,Pelt2Pin=PIN_PD5};

enum{INTPin=19,Pelt1Pin=9,Pelt2Pin=8};

enum{
  E_GREEN_BUTTON=1,
  E_LWHITE_BUTTON=2,
  E_RWHITE_BUTTON=4,
  E_BLUE_BUTTON=8,
  E_RED_BUTTON=16
};

enum{
  MCP_ADDR=0x20
};
// Keyboard keys mapping
////////////////////////////////////////////////////////////////// TASK DATA
enum{TASK_ERROR_HANDLER_STACK_SIZE=200};     //0
enum{TASK_STACK_DEBUGGER_STACK_SIZE=200};    //1
enum{TASK_MAIN_STACK_SIZE=256};              //2
enum{UPDATE_SCREEN_STACK_SIZE=232};          //3
enum{TASK_REGULATE_TEMP_STACK_SIZE=128};     //4
enum{TASK_READ_INPUT_STACK_SIZE=150};        //5
enum{TASK_SELECT_DRINK_STACK_SIZE=256};      //6
enum{TASK_ORDER_DRINK_STACK_SIZE=320};       //7
enum{TASK_SHOW_INFO_STACK_SIZE=256};         //8
enum{TASK_SHOW_TEMP_STACK_SIZE=192};         //9
enum{TASK_SHOW_LAST_ERROR_STACK_SIZE=256};   //10
enum{TASK_KEYBOARD_SIM_STACK_SIZE=192};      //11
// Stack size - will need "tuning" in last release
enum{
  TASK_ERROR_HANDLER=0,
  TASK_STACK_DEBUGGER=1,
  TASK_MAIN=2,
  TASK_UPDATE_SCREEN=3,
  TASK_REGULATE_TEMP=4,
  TASK_READ_INPUT=5,
  TASK_SELECT_DRINK=6,
  TASK_ORDER_DRINK=7,
  TASK_SHOW_INFO=8,
  TASK_SHOW_TEMP=9,
  TASK_SHOW_LAST_ERROR=10,
  TASK_KEYBOARD_SIM=11
};
// Task identifiers
enum{GUARD_ZONE_SIZE=16};
// Guard zone between task stacks	 
enum{TASK_N=12};
// Task count
extern StaticTask_t errorHandlerTCB;                                   //0
extern StaticTask_t stackDebuggerTCB;                                  //1
extern StaticTask_t mainTCB;                                           //2
extern StaticTask_t updateScreenTCB;                                   //3
extern StaticTask_t regulateTempTCB;                                   //4
extern StaticTask_t readInputTCB;                                      //5
extern StaticTask_t selectDrinkTCB;                                    //6
extern StaticTask_t orderDrinkTCB;                                     //7
extern StaticTask_t showInfoTCB;                                       //8
extern StaticTask_t showTempTCB;                                       //9
extern StaticTask_t showLastErrorTCB;                                  //10
extern StaticTask_t keyboardSimTCB;                                    //11
// Task control blocks
extern TaskHandle_t taskHandles[TASK_N];
// Task handles, assigned in the same order as identifiers
extern StackType_t errorHandlerStack[TASK_ERROR_HANDLER_STACK_SIZE];   //0
extern StackType_t guardZone0[GUARD_ZONE_SIZE];

extern StackType_t stackDebuggerStack[TASK_STACK_DEBUGGER_STACK_SIZE]; //1
extern StackType_t guardZone1[GUARD_ZONE_SIZE];

extern StackType_t mainStack[TASK_MAIN_STACK_SIZE];                    //2
extern StackType_t guardZone2[GUARD_ZONE_SIZE];

extern StackType_t updateScreenStack[UPDATE_SCREEN_STACK_SIZE];        //3
extern StackType_t guardZone3[GUARD_ZONE_SIZE];

extern StackType_t regulateTempStack[TASK_REGULATE_TEMP_STACK_SIZE];   //4
extern StackType_t guardZone4[GUARD_ZONE_SIZE];

extern StackType_t readInputStack[TASK_READ_INPUT_STACK_SIZE];         //5
extern StackType_t guardZone5[GUARD_ZONE_SIZE];

extern StackType_t selectDrinkStack[TASK_SELECT_DRINK_STACK_SIZE];     //6
extern StackType_t guardZone6[GUARD_ZONE_SIZE];

extern StackType_t orderDrinkStack[TASK_ORDER_DRINK_STACK_SIZE];       //7
extern StackType_t guardZone7[GUARD_ZONE_SIZE];

extern StackType_t showInfoStack[TASK_SHOW_INFO_STACK_SIZE];           //8
extern StackType_t guardZone8[GUARD_ZONE_SIZE];

extern StackType_t showTempStack[TASK_SHOW_TEMP_STACK_SIZE];           //9
extern StackType_t guardZone9[GUARD_ZONE_SIZE];

extern StackType_t showLastErrorStack[TASK_SHOW_LAST_ERROR_STACK_SIZE];//10
extern StackType_t guardZone10[GUARD_ZONE_SIZE];

extern StackType_t keyboardSimStack[TASK_KEYBOARD_SIM_STACK_SIZE];     //11
extern StackType_t guardZone11[GUARD_ZONE_SIZE];
//Task stacks with added guard zones
////////////////////////////////////////////////////////////////// SCREEN DATA
enum{
  LCD_WIDTH=20,
  LCD_HEIGHT=4,
  LCD_ADDR=0x27
};
// LCD's parameters
enum{
  E_LOADING_BAR=17
};
// LCD's custom character identifier (based on ascii code)
enum{
  SHOW_INFO_MENUS_COUNT=4
};
// Count of submenus in task showInfo
enum{
  WELCOME_SCREEN=0,
  DRINK_SELECT=1,
  DRINK_ORDER=2,
  SHOW_INFO=3,
  SHOW_LAST_ERROR=4
};
// Screen identifiers
/*
    Screen transition diagram:

          +-------------+
          | 0 Welcome   |
          +------+------+
                 |
                 v
          +-------------+  
          | 1 Drink     |  (scrollable)
          | Select      |
          +------+------+
             /       \
            v         v
      +-------------+  +-------------+  (scrollable)
      | 2 Drink     |  | 3 Show      |
      | Order       |  | System Info |
      +-------------+  +------+------+
                              |
                              v
                   +------------------+
                   | 4 Show Last Error|  (scrollable)
                   | Size             |
                   +------------------+

    Legend:
    - (scrollable) marks screens that support scrolling
    - (editable) marks screen that supports editing
*/
extern LiquidCrystal_I2C lcd;
////////////////////////////////////////////////////////////////// FLOW CONTROL
enum{
  SCREEN_QUEUE_BUFFER_COUNT=6,
  KEYBOARD_QUEUE_BUFFER_COUNT=2,
  DRINK_ID_QUEUE_BUFFER_COUNT=2,
  SHOW_INFO_QUEUE_BUFFER_COUNT=2,
  ERROR_ID_QUEUE_BUFFER_COUNT=1
};
// Size of each Queue
extern uint8_t screenQueueBuffer[];
extern uint8_t keyboardQueueBuffer[];
extern uint8_t drinkIdQueueBuffer[];
extern uint8_t showInfoQueueBuffer[];
extern uint8_t errorIdQueueBuffer[];
// Queues' buffers
extern StaticQueue_t screenQueueStructBuffer;
extern StaticQueue_t keyboardQueueStructBuffer;
extern StaticQueue_t drinkIdQueueStructBuffer;
extern StaticQueue_t showInfoQueueStructBuffer;
extern StaticQueue_t errorIdQueueStructBuffer;
// 
extern StaticSemaphore_t semReadDataBuffer;
extern StaticSemaphore_t muxI2CLockBuffer;
extern StaticSemaphore_t muxSerialLockBuffer;
// Semaphores' buffers
extern QueueHandle_t qScreenData;      // Consumed only by taskUpdateScreen
extern QueueHandle_t qKeyboardData;    // Consumed by taskMain and taskErrorHandler (change it to taskMain only)
extern QueueHandle_t qDrinkId; // Change name to qSelectDrinkId, Consumed by taskSelectDrink
extern QueueHandle_t qOrderDrinkId; // Consumed only by taskOrderDrink
extern QueueHandle_t qShowInfoId; // Consumed only by taskShowInfo 
extern QueueHandle_t qErrorId; // Consumed onl by taskErrorHandler
// Queue handles
extern SemaphoreHandle_t sem_ReadData;
extern SemaphoreHandle_t mux_I2CLock;
extern SemaphoreHandle_t mux_SerialLock;
// Semaphores and mutexes

////////////////////////////////////////////////////////////////// GLOBAL VARIABLES
extern sSystemError lastSystemError;
extern uint8_t f_errorConfirmed;
extern uint16_t bootupsCount;

extern float currentTemperature;
extern float setTemperature;
extern float temperatureHysteresis;

extern volatile bool f_enableISR;
////////////////////////////////////////////////////////////////// DRINK DATA
extern const sDrinkData drink[];
//
extern const char ingredients[][LCD_WIDTH-4-4];
//
extern const uint8_t pumpsEff[];
// Pumps efficiency in ml/min

#endif 
