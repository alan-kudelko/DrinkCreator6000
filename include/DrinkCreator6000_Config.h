#ifndef _DRINK_CREATOR6000_CONFIG_H_
  #define _DRINK_CREATOR6000_CONFIG_H_

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <timers.h>
#include <FreeRTOSConfig.h>
#include <portable.h>
#include <portmacro.h>
#include <semphr.h>
#include <stdint.h>
#include <stdbool.h>

#include <DrinkCreator6000_DataTypes.h>
#include <DrinkCreator6000_Config_C.h>
#include <DrinkCreator6000_Pins.h>

//////////////////////////////////////////////////////////////////
// IO Mapping:
// Pin definitions and button mappings  
// Address of MCP23017 I²C expander


#define E_GREEN_BUTTON 1
#define E_LWHITE_BUTTON 2
#define E_RWHITE_BUTTON 4
#define E_BLUE_BUTTON 8
#define E_RED_BUTTON 16

#define MCP_ADDR 0x20

/** @brief Stack size (in words) for Error Handler task (ID 0). */
#define TASK_ERROR_HANDLER_STACK_SIZE 230           //0
/** @brief Stack size (in words) for Serial System Debugger task (ID 1). */
#define TASK_SERIAL_SYSTEM_DEBUGGER_STACK_SIZE 260  //1
/** @brief Stack size (in words) for Main task (ID 2). */
#define TASK_MAIN_STACK_SIZE 150                    //2
/** @brief Stack size (in words) for Read Input task (ID 3). */
#define TASK_READ_INPUT_STACK_SIZE 180              //3
/** @brief Stack size (in words) for Serial Input task (ID 4). */
#define TASK_SERIAL_INPUT_STACK_SIZE 180            //4
/** @brief Stack size (in words) for Update Screen task (ID 5). */
#define TASK_UPDATE_SCREEN_STACK_SIZE 250           //5
/** @brief Stack size (in words) for Read Temperature task (ID 6). */
#define TASK_READ_TEMP_STACK_SIZE 120               //6
/** @brief Stack size (in words) for Regulate Temperature task (ID 7). */
#define TASK_REGULATE_TEMP_STACK_SIZE 120           //7
/** @brief Stack size (in words) for Select Drink task (ID 8). */
#define TASK_SELECT_DRINK_STACK_SIZE 230            //8
/** @brief Stack size (in words) for Order Drink task (ID 9). */
#define TASK_ORDER_DRINK_STACK_SIZE 256             //9
/** @brief Stack size (in words) for Show System Info task (ID 10). */
#define TASK_SHOW_SYSTEM_INFO_STACK_SIZE 300        //10
/** 
 * @brief Stack size (in words) for Welcome Screen task (ID 11). 
 * Tuned with 48 words in reserve. 160 words caused overflow.
 */
#define TASK_WELCOME_SCREEN_STACK_SIZE 210          //11
/** @brief Stack size (in words) for Test Hardware task (ID 12). */
#define TASK_TEST_HARDWARE_STACK_SIZE 150             //12
/** 
 * @brief Stack size (in words) for Welcome Screen task (ID 11). 
 * Tuned with 48 words in reserve. 160 words caused overflow.
 */
#define TASK_WELCOME_SCREEN_STACK_SIZE 210          //11
/** @brief Stack size (in words) for Test Hardware task (ID 12). */
#define TASK_TEST_HARDWARE_STACK_SIZE 150             //12
// Stack size - will need "tuning" in final release

#define TASK_ERROR_HANDLER 0
#define TASK_SERIAL_DEBUGGER 1
#define TASK_MAIN 2
#define TASK_READ_INPUT 3
#define TASK_SERIAL_INPUT 4
#define TASK_UPDATE_SCREEN 5
#define TASK_READ_TEMP 6
#define TASK_REGULATE_TEMP 7
#define TASK_SELECT_DRINK 8
#define TASK_ORDER_DRINK 9
#define TASK_SHOW_SYS_INFO 10
#define TASK_WELCOME_SCREEN 11
#define TASK_TEST_HARDWAER 12
// Task identifiers

#define GUARD_ZONE_SIZE 32
#define MEMORY_FILL_PATTERN 0xAA
// Guard zone size between task stacks	 
//enum{TASK_N=12};
// Task count
#define TASK_ERROR_HANDLER_REFRESH_RATE 500     // 0
#define TASK_SERIAL_DEBUGGER_REFRESH_RATE 2000  // 1
#define TASK_MAIN_REFRESH_RATE 500              // 2
#define TASK_READ_INPUT_REFRESH_RATE 100        // 3  
#define TASK_SERIAL_INPUT_REFRESH_RATE 200      // 4
#define TASK_UPDATE_SCREEN_REFRESH_RATE 300     // 5
#define TASK_READ_TEMP_REFRESH_RATE 2000        // 6
#define TASK_REGULATE_TEMP_REFRESH_RATE 5000    // 7
#define TASK_SELECT_DRINK_REFRESH_RATE 1000     // 8
#define TASK_ORDER_DRINK_REFRESH_RATE 500       // 9
#define TASK_SHOW_SYSTEM_INFO_REFRESH_RATE 600  // 10
#define TASK_WELCOME_SCREEN_REFRESH_RATE 500     // 11

#define TASK_WELCOME_TICKS_TO_CLOSE 10
// Task refresh rates in ms

/**
 * @brief Task control blocks (TCB) for all FreeRTOS tasks.
 * 
 * Each variable represents the static task control block for a specific task.
 * These TCBs are used by the FreeRTOS kernel to manage task states and context.
 * The order corresponds to the task IDs used throughout the system.
 */
extern StaticTask_t errorHandlerTCB;                                ///< Task Control Block for Error Handler task (ID 0)
extern StaticTask_t serialSystemDebuggerTCB;                        ///< Task Control Block for Serial System Debugger task (ID 1)
extern StaticTask_t mainTCB;                                        ///< Task Control Block for Main task (ID 2)
extern StaticTask_t readInputTCB;                                   ///< Task Control Block for Read Input task (ID 3)
extern StaticTask_t serialInputTCB;                                 ///< Task Control Block for Serial Input task (ID 4)
extern StaticTask_t updateScreenTCB;                                ///< Task Control Block for Update Screen task (ID 5)
extern StaticTask_t readTempTCB;                                    ///< Task Control Block for Read Temperature task (ID 6)
extern StaticTask_t regulateTempTCB;                                ///< Task Control Block for Regulate Temperature task (ID 7)
extern StaticTask_t selectDrinkTCB;                                 ///< Task Control Block for Select Drink task (ID 8)
extern StaticTask_t orderDrinkTCB;                                  ///< Task Control Block for Order Drink task (ID 9)
extern StaticTask_t showSystemInfoTCB;                              ///< Task Control Block for Show System Info task (ID 10)
extern StaticTask_t welcomeScreenTCB;                               ///< Task Control Block for Welcome Screen task (ID 11)
/**
 * @brief Array of task handles.
 * 
 * This array holds the TaskHandle_t values for all created tasks,
 * indexed by task ID corresponding to the order above.
 */
extern TaskHandle_t taskHandles[TASK_N];                            ///< Array of task handles for all tasks
/** 
 * @brief Guard zones used for detecting stack overflow for each task.
 * 
 * These are memory regions placed before each task stack to detect
 * stack overflows by monitoring if the guard zone contents are overwritten.
 */
extern volatile StackType_t guardZone0[]; ///< Guard zone before Error Handler task stack
extern StackType_t errorHandlerStack[];   ///< Stack for Error Handler task (ID 0)

extern volatile StackType_t guardZone1[]; ///< Guard zone before Serial System Debugger task stack
extern StackType_t serialSystemDebuggerStack[]; ///< Stack for Serial System Debugger task (ID 1)

extern volatile StackType_t guardZone2[]; ///< Guard zone before Main task stack
extern StackType_t mainStack[];            ///< Stack for Main task (ID 2)

extern volatile StackType_t guardZone3[]; ///< Guard zone before Read Input task stack
extern StackType_t readInputStack[];       ///< Stack for Read Input task (ID 3)

extern volatile StackType_t guardZone4[]; ///< Guard zone before Serial Input task stack
extern StackType_t serialInputStack[];     ///< Stack for Serial Input task (ID 4)

extern volatile StackType_t guardZone5[]; ///< Guard zone before Update Screen task stack
extern StackType_t updateScreenStack[];    ///< Stack for Update Screen task (ID 5)

extern volatile StackType_t guardZone6[]; ///< Guard zone before Read Temperature task stack
extern StackType_t readTempStack[];        ///< Stack for Read Temperature task (ID 6)

extern volatile StackType_t guardZone7[]; ///< Guard zone before Regulate Temperature task stack
extern StackType_t regulateTempStack[];    ///< Stack for Regulate Temperature task (ID 7)

extern volatile StackType_t guardZone8[]; ///< Guard zone before Select Drink task stack
extern StackType_t selectDrinkStack[];     ///< Stack for Select Drink task (ID 8)

extern volatile StackType_t guardZone9[]; ///< Guard zone before Order Drink task stack
extern StackType_t orderDrinkStack[];      ///< Stack for Order Drink task (ID 9)

extern volatile StackType_t guardZone10[]; ///< Guard zone before Show System Info task stack
extern StackType_t showSystemInfoStack[];  ///< Stack for Show System Info task (ID 10)

extern volatile StackType_t guardZone11[]; ///< Guard zone before Welcome Screen task stack
extern StackType_t welcomeScreenStack[];   ///< Stack for Welcome Screen task (ID 11)
/**
 * @brief Array of pointers to all guard zones.
 * 
 * Used by the stack overflow detection routine to check each task's guard zone.
 */
extern volatile StackType_t* guardZones[]; ///< Array of pointers to guard zones for all tasks


// Task stacks and guard zones
//////////////////////////////////////////////////////////////////
// Screen data:
// LCD dimensions and I2C address

#define LCD_WIDTH 20
#define LCD_HEIGHT 4
#define LCD_ADDR 0x27

#define E_LOADING_BAR 17
// LCD custom characters

#define SHOW_INFO_MENUS_COUNT 4
// Show info submenu count

#define WELCOME_SCREEN 0
#define DRINK_SELECT 1
#define DRINK_ORDER 2
#define SHOW_INFO 3
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
//extern LiquidCrystal_I2C lcd;
//////////////////////////////////////////////////////////////////
// Flow control:
// Queue sizes and buffers for inter-task communication

#define SCREEN_QUEUE_BUFFER_COUNT 2
#define KEYBOARD_QUEUE_BUFFER_COUNT 2
#define ERROR_ID_QUEUE_BUFFER_COUNT 1

extern uint8_t screenQueueBuffer[];
extern uint8_t keyboardQueueBuffer[];
extern uint8_t errorIdQueueBuffer[];

extern StaticQueue_t screenQueueStructBuffer;
extern StaticQueue_t keyboardQueueStructBuffer;
extern StaticQueue_t errorIdQueueStructBuffer;

extern StaticSemaphore_t semReadDataBuffer;
extern StaticSemaphore_t muxI2CLockBuffer;
extern StaticSemaphore_t muxSerialLockBuffer;

extern QueueHandle_t qScreenData;      // Consumed by taskUpdateScreen
extern QueueHandle_t qKeyboardData;    // Consumed by taskMain
extern QueueHandle_t qErrorId;
// Queue handles
extern SemaphoreHandle_t sem_ReadData;
extern SemaphoreHandle_t mux_I2CLock;
extern SemaphoreHandle_t mux_SerialLock;
// Semaphores and Mutexes handles

extern volatile struct sUIContext UI_Context;
//////////////////////////////////////////////////////////////////
// Global variables:
// System error, flags, counters and temperature parameters

extern struct sSystemError lastSystemError;
extern uint8_t f_errorConfirmed;
extern uint16_t bootupsCount;

extern float currentTemperature;
extern float setTemperature;
extern float temperatureHysteresis;

extern volatile bool f_enableISR;
//////////////////////////////////////////////////////////////////
// Drink data:
// Drink definitions, ingredients, and pump efficiencies

#define DRINK_COUNT 10

extern const struct sDrinkData drink[];
//
extern const char ingredients[][LCD_WIDTH-4-4];
//
extern const uint8_t pumpsEff[];
// Pumps efficiency in ml/min
// Idle task counter

#define INTERVAL_TICKS 1000 // CPU usage update interval

extern volatile uint32_t idleCounter;
extern volatile uint32_t idleCounterLast;
extern volatile uint32_t idleCounterPerSecond;
extern volatile uint32_t tickCount;
extern volatile uint32_t idleCalib;

#endif // _DRINK_CREATOR6000_CONFIG_H_
