#ifndef _DRINK_CREATOR6000_CONFIG_H_
#define _DRINK_CREATOR6000_CONFIG_H_

#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <LiquidCrystal_I2C.h>
#include "CustomDataTypes.h"

//////////////////////////////////////////////////////////////////
// IO Mapping:
// Pin definitions and button mappings
// Address of MCP23017 I²C expander

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
//////////////////////////////////////////////////////////////////
// Task data:
// Stack sizes for all tasks and their IDs
// Guard zones for stack overflow protection
// Task refresh rates
enum{TASK_ERROR_HANDLER_STACK_SIZE=256};     //0
enum{TASK_STACK_DEBUGGER_STACK_SIZE=256};    //1
enum{TASK_MAIN_STACK_SIZE=256};              //2
enum{UPDATE_SCREEN_STACK_SIZE=232};          //3
enum{TASK_REGULATE_TEMP_STACK_SIZE=150};     //4
enum{TASK_READ_INPUT_STACK_SIZE=150};        //5
enum{TASK_SELECT_DRINK_STACK_SIZE=256};      //6
enum{TASK_ORDER_DRINK_STACK_SIZE=320};       //7
enum{TASK_SHOW_INFO_STACK_SIZE=384};         //8
enum{TASK_SHOW_LAST_ERROR_STACK_SIZE=320};   //9
enum{TASK_KEYBOARD_SIM_STACK_SIZE=150};      //10
enum{TASK_WELCOME_STACK_SIZE=150};           //11
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
  TASK_SHOW_LAST_ERROR=9,
  TASK_KEYBOARD_SIM=10,
  TASK_WELCOME=11
};
// Task identifiers
enum{GUARD_ZONE_SIZE=4};
// Guard zone size between task stacks	 
enum{TASK_N=11};
// Task count
enum{
  TASK_ERROR_HANDLER_REFRESH_RATE=500,
  TASK_STACK_DEBUGGER_REFRESH_RATE=2000,
  TASK_MAIN_REFRESH_RATE=500,
  TASK_UPDATE_SCREEN_REFRESH_RATE=300,
  TASK_REGULATE_TEMP_REFRESH_RATE=5000,
  TASK_READ_INPUT_REFRESH_RATE=100,
  TASK_SELECT_DRINK_REFRESH_RATE=1000,
  TASK_ORDER_DRINK_REFRESH_RATE=1000,
  TASK_SHOW_INFO_REFRESH_RATE=1500,
  TASK_SHOW_LAST_ERROR_REFRESH_RATE=1000,
  TASK_KEYBOARD_SIM_REFRESH_RATE=1000,
  TASK_WELCOME_REFRESH_RATE=500
};
// Task refresh rates in ms
extern StaticTask_t errorHandlerTCB;                                   //0
extern StaticTask_t stackDebuggerTCB;                                  //1
extern StaticTask_t mainTCB;                                           //2
extern StaticTask_t updateScreenTCB;                                   //3
extern StaticTask_t regulateTempTCB;                                   //4
extern StaticTask_t readInputTCB;                                      //5
extern StaticTask_t selectDrinkTCB;                                    //6
extern StaticTask_t orderDrinkTCB;                                     //7
extern StaticTask_t showInfoTCB;                                       //8
extern StaticTask_t showLastErrorTCB;                                  //9
extern StaticTask_t keyboardSimTCB;                                    //10
extern StaticTask_t welcomeTCB;                                        //11
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

extern StackType_t showLastErrorStack[TASK_SHOW_LAST_ERROR_STACK_SIZE];//9
extern StackType_t guardZone9[GUARD_ZONE_SIZE];

extern StackType_t keyboardSimStack[TASK_KEYBOARD_SIM_STACK_SIZE];     //10
extern StackType_t guardZone10[GUARD_ZONE_SIZE];

extern StackType_t welcomeStack[TASK_WELCOME_STACK_SIZE];              //11
extern StackType_t guardZone11[GUARD_ZONE_SIZE];
// Task stacks and guard zones
//////////////////////////////////////////////////////////////////
// Screen data:
// LCD dimensions and I2C address
enum{
  LCD_WIDTH=20,
  LCD_HEIGHT=4,
  LCD_ADDR=0x27
};
enum{
  E_LOADING_BAR=17
};
// LCD custom characters
enum{
  SHOW_INFO_MENUS_COUNT=4
};
// Show info submenu count
enum{
  WELCOME_SCREEN=0,
  DRINK_SELECT=1,
  DRINK_ORDER=2,
  SHOW_INFO=3,
  SHOW_LAST_ERROR=4
};
// Screen identifiers and navigation overview
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
                   | 4 Show Last      |  (scrollable)
                   | Error            |
                   +------------------+
                              |
                              v
                   +------------------+
                   | 5 Test pumps     |  (scrollable)
                   |                  |
                   +------------------+
				   
				   
    Legend:
    - (scrollable) marks screens that support scrolling
    - (editable) marks screen that supports editing
*/
extern LiquidCrystal_I2C lcd;
//////////////////////////////////////////////////////////////////
// Flow control:
// Queue sizes and buffers for inter-task communication
enum{
  SCREEN_QUEUE_BUFFER_COUNT=6,
  KEYBOARD_QUEUE_BUFFER_COUNT=2,
  DRINK_ID_QUEUE_BUFFER_COUNT=2,
  SHOW_INFO_QUEUE_BUFFER_COUNT=2,
  ERROR_ID_QUEUE_BUFFER_COUNT=1,
  LAST_ERROR_ID_QUEUE_BUFFER_COUNT=1
};
extern uint8_t screenQueueBuffer[];
extern uint8_t keyboardQueueBuffer[];
extern uint8_t drinkIdQueueBuffer[];
extern uint8_t showInfoQueueBuffer[];
extern uint8_t errorIdQueueBuffer[];
extern uint8_t lastErrorIdQueueBuffer[];

extern StaticQueue_t screenQueueStructBuffer;
extern StaticQueue_t keyboardQueueStructBuffer;
extern StaticQueue_t drinkIdQueueStructBuffer;
extern StaticQueue_t showInfoQueueStructBuffer;
extern StaticQueue_t errorIdQueueStructBuffer;
extern StaticQueue_t lastErrorIdQueueStructBuffer;

extern StaticSemaphore_t semReadDataBuffer;
extern StaticSemaphore_t muxI2CLockBuffer;
extern StaticSemaphore_t muxSerialLockBuffer;

extern QueueHandle_t qScreenData;      // Consumed by taskUpdateScreen
extern QueueHandle_t qKeyboardData;    // Consumed by taskMain
extern QueueHandle_t qDrinkId;         // Change name to qSelectDrinkId, Consumed by taskSelectDrink
extern QueueHandle_t qOrderDrinkId;    // Consumed only by taskOrderDrink
extern QueueHandle_t qShowInfoId; // Consumed by taskShowInfo 
extern QueueHandle_t qErrorId; // Consumed by taskErrorHandler
extern QueueHandle_t qLastErrorId; // Consumed by taskLastError
// Queue handles
extern SemaphoreHandle_t sem_ReadData;
extern SemaphoreHandle_t mux_I2CLock;
extern SemaphoreHandle_t mux_SerialLock;
// Semaphores and Mutexes handles
//////////////////////////////////////////////////////////////////
// Global variables:
// System error, flags, counters and temperature parameters
extern sSystemError lastSystemError;
extern uint8_t f_errorConfirmed;
extern uint16_t bootupsCount;

extern float currentTemperature;
extern float setTemperature;
extern float temperatureHysteresis;

extern volatile bool f_enableISR;
//////////////////////////////////////////////////////////////////
// Drink data:
// Drink definitions, ingredients, and pump efficiencies
extern const sDrinkData drink[];
//
extern const char ingredients[][LCD_WIDTH-4-4];
//
extern const uint8_t pumpsEff[];
// Pumps efficiency in ml/min
//////////////////////////////////////////////////////////////////
// RAM-related symbols and global memory variables:
extern uint16_t __data_start;
extern uint16_t __data_end;

extern uint16_t __bss_start;
extern uint16_t __bss_end;

extern uint16_t __heap_start;
extern void*__brkval;

extern void* heap_end;
extern uint8_t*stack_ptr;

extern uint16_t heap_size;
extern uint16_t stack_size;

extern uint16_t ram_total_free;
extern uint16_t ram_in_use;
extern const uint16_t ram_size;

#endif // _DRINK_CREATOR6000_CONFIG_H_
