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
enum{TASK_ERROR_HANDLER_STACK_SIZE=256};           //0
enum{TASK_SERIAL_SYSTEM_DEBUGGER_STACK_SIZE=270};  //1
enum{TASK_MAIN_STACK_SIZE=200};                    //2
enum{TASK_READ_INPUT_STACK_SIZE=150};              //3
enum{TASK_SERIAL_INPUT_STACK_SIZE=150};            //4
enum{TASK_UPDATE_SCREEN_STACK_SIZE=250};           //5
enum{TASK_READ_TEMP_STACK_SIZE=180};               //6
enum{TASK_REGULATE_TEMP_STACK_SIZE=180};           //7
enum{TASK_SELECT_DRINK_STACK_SIZE=270};            //8
enum{TASK_ORDER_DRINK_STACK_SIZE=320};             //9
enum{TASK_SHOW_SYSTEM_INFO_STACK_SIZE=300};        //10
enum{TASK_WELCOME_SCREEN_STACK_SIZE=177};          //11 // Tuned, 48 words in reserve //160 words causes overflow
enum{TASK_TEST_HARDWARE_STACK_SIZE=215};           //12
// Stack size - will need "tuning" in last release
enum{
  TASK_ERROR_HANDLER=0,
  TASK_SERIAL_DEBUGGER=1,
  TASK_MAIN=2,
  TASK_READ_INPUT=3,
  TASK_SERIAL_INPUT=4,
  TASK_UPDATE_SCREEN=5,
  TASK_READ_TEMP=6,
  TASK_REGULATE_TEMP=7,
  TASK_SELECT_DRINK=8,
  TASK_ORDER_DRINK=9,
  TASK_SHOW_SYS_INFO=10,
  TASK_WELCOME_SCREEN=11,
  TASK_TEST_HARDWAER=12
};
extern const char*TaskNames[];
// Task identifiers
enum{GUARD_ZONE_SIZE=32};
enum{MEMORY_FILL_PATTERN=0xAA};
// Guard zone size between task stacks	 
enum{TASK_N=12};
// Task count
enum{
  TASK_ERROR_HANDLER_REFRESH_RATE=500,     // 0
  TASK_SERIAL_DEBUGGER_REFRESH_RATE=2000,  // 1
  TASK_MAIN_REFRESH_RATE=500,              // 2
  TASK_READ_INPUT_REFRESH_RATE=100,        // 3  
  TASK_SERIAL_INPUT_REFRESH_RATE=200,      // 4
  TASK_UPDATE_SCREEN_REFRESH_RATE=300,     // 5
  TASK_READ_TEMP_REFRESH_RATE=2000,        // 6
  TASK_REGULATE_TEMP_REFRESH_RATE=5000,    // 7
  TASK_SELECT_DRINK_REFRESH_RATE=1000,     // 8
  TASK_ORDER_DRINK_REFRESH_RATE=1000,      // 9
  TASK_SHOW_SYSTEM_INFO_REFRESH_RATE=600,  // 10
  TASK_WELCOME_SCREEN_REFRESH_RATE=500     // 11
};
enum{TASK_WELCOME_TICKS_TO_CLOSE=10};
// Task refresh rates in ms
extern StaticTask_t errorHandlerTCB;                                //0
extern StaticTask_t serialSystemDebuggerTCB;                        //1
extern StaticTask_t mainTCB;                                        //2
extern StaticTask_t readInputTCB;                                   //3
extern StaticTask_t serialInputTCB;                                 //4
extern StaticTask_t updateScreenTCB;                                //5
extern StaticTask_t readTempTCB;                                    //6
extern StaticTask_t regulateTempTCB;                                //7
extern StaticTask_t selectDrinkTCB;                                 //8
extern StaticTask_t orderDrinkTCB;                                  //9
extern StaticTask_t showSystemInfoTCB;                              //10
extern StaticTask_t welcomeScreenTCB;                               //11
// Task control blocks
extern TaskHandle_t taskHandles[TASK_N];
// Task handles, assigned in the same order as identifiers
extern volatile StackType_t guardZone0[];
extern StackType_t errorHandlerStack[];          //0

extern volatile StackType_t guardZone1[];
extern StackType_t serialSystemDebuggerStack[];  //1

extern volatile StackType_t guardZone2[];
extern StackType_t mainStack[];                  //2

extern volatile StackType_t guardZone3[];
extern StackType_t readInputStack[];             //3

extern volatile StackType_t guardZone4[];
extern StackType_t serialInputStack[];           //4

extern volatile StackType_t guardZone5[];
extern StackType_t updateScreenStack[];          //5

extern volatile StackType_t guardZone6[];
extern StackType_t readTempStack[];              //6

extern volatile StackType_t guardZone7[];
extern StackType_t regulateTempStack[];          //7

extern volatile StackType_t guardZone8[];
extern StackType_t selectDrinkStack[];           //8

extern volatile StackType_t guardZone9[];
extern StackType_t orderDrinkStack[];            //9

extern volatile StackType_t guardZone10[];
extern StackType_t showSystemInfoStack[];        //10

extern volatile StackType_t guardZone11[];
extern StackType_t welcomeScreenStack[];         //11

extern StackType_t*guardZones[]; // Used in stack overflow detection
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
  SHOW_INFO=3
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
  ERROR_ID_QUEUE_BUFFER_COUNT=1
};
extern uint8_t screenQueueBuffer[];
extern uint8_t keyboardQueueBuffer[];
extern uint8_t drinkIdQueueBuffer[];
extern uint8_t showInfoQueueBuffer[];
extern uint8_t errorIdQueueBuffer[];

extern StaticQueue_t screenQueueStructBuffer;
extern StaticQueue_t keyboardQueueStructBuffer;
extern StaticQueue_t drinkIdQueueStructBuffer;
extern StaticQueue_t showInfoQueueStructBuffer;
extern StaticQueue_t errorIdQueueStructBuffer;

extern StaticSemaphore_t semReadDataBuffer;
extern StaticSemaphore_t muxI2CLockBuffer;
extern StaticSemaphore_t muxSerialLockBuffer;

extern QueueHandle_t qScreenData;      // Consumed by taskUpdateScreen
extern QueueHandle_t qKeyboardData;    // Consumed by taskMain
extern QueueHandle_t qDrinkId;         // Change name to qSelectDrinkId, Consumed by taskSelectDrink
extern QueueHandle_t qOrderDrinkId;    // Consumed only by taskOrderDrink
extern QueueHandle_t qShowInfoId; // Consumed by taskShowInfo 
extern QueueHandle_t qErrorId;
// Queue handles
extern SemaphoreHandle_t sem_ReadData;
extern SemaphoreHandle_t mux_I2CLock;
extern SemaphoreHandle_t mux_SerialLock;
// Semaphores and Mutexes handles
extern volatile sUIContext UI_Context;
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
enum{DRINK_COUNT=20};
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

extern uint16_t __tdat_start;
extern uint16_t __tdat_end;
extern uint16_t __tdat_size;

extern uint16_t __heap_start;
extern void*__brkval;

extern void* __heap_end;
extern uint8_t*__stack_ptr;

extern uint16_t __heap_size;
extern uint16_t __stack_size;

extern uint16_t ram_total_free;
extern uint16_t ram_in_use;
extern const uint16_t ram_size;

#endif // _DRINK_CREATOR6000_CONFIG_H_
