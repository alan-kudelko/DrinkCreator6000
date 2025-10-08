/**
 * @file DrinkCreator6000_Config_C.h
 * @brief Centralized configuration header for the DrinkCreator6000 system.
 *
 * This file defines compile-time constants and declares global data structures
 * used throughout the project. It consolidates configuration of:
 * - CPU frequency and target MCU
 * - FreeRTOS tasks resources
 * - Synchronization primitives (queues, semaphores, mutexes)
 * - UI context and global variables
 * - Drink recipes and machine configuration
 * - CPU usage statistics variables
 * - Button mapping for MCP230017
 */
#ifndef _DRINK_CREATOR6000_CONFIG_C_H_
    #define _DRINK_CREATOR6000_CONFIG_C_H_

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>

#include <DrinkCreator6000_DataTypes.h>

// ========================
// CPU and MCU Configuration
// ========================

/**
 * @def F_CPU
 * @brief CPU clock frequency in Hertz.
 * 
 * Default is 8 MHz (8,000,000 Hz).
 */
#ifndef F_CPU
    #define F_CPU 8000000UL
#endif
/**
 * @def __AVR_ATmega2560__
 * @brief Define to indicate the target MCU is ATmega2560.
 */
#ifndef __AVR_ATmega2561__
    #define __AVR_ATmega2561__ 1
#endif

// ========================
// FreeRTOS Task Configuration
// ========================
// (Task handles, stack sizes, priorities, guard zones)

// Task count
#define TASK_N 12

// Task identifiers

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
#define TASK_TEST_HARDWARE 11

/** @brief Stack size (in words) for Error Handler task (ID 0). */
#define TASK_ERROR_HANDLER_STACK_SIZE 230           //0
/** @brief Stack size (in words) for Serial System Debugger task (ID 1). */
#define TASK_SERIAL_SYSTEM_DEBUGGER_STACK_SIZE 260  //1
/** @brief Stack size (in words) for Main task (ID 2). */
#define TASK_MAIN_STACK_SIZE 300                    //2
/** @brief Stack size (in words) for Read Input task (ID 3). */
#define TASK_READ_INPUT_STACK_SIZE 280              //3
/** @brief Stack size (in words) for Serial Input task (ID 4). */
#define TASK_SERIAL_INPUT_STACK_SIZE 280            //4
/** @brief Stack size (in words) for Update Screen task (ID 5). */
#define TASK_UPDATE_SCREEN_STACK_SIZE 270           //5
/** @brief Stack size (in words) for Read Temperature task (ID 6). */
#define TASK_READ_TEMP_STACK_SIZE 160               //6
/** @brief Stack size (in words) for Regulate Temperature task (ID 7). */
#define TASK_REGULATE_TEMP_STACK_SIZE 160           //7
/** @brief Stack size (in words) for Select Drink task (ID 8). */
#define TASK_SELECT_DRINK_STACK_SIZE 300            //8
/** @brief Stack size (in words) for Order Drink task (ID 9). */
#define TASK_ORDER_DRINK_STACK_SIZE 256             //9
/** @brief Stack size (in words) for Show System Info task (ID 10). */
#define TASK_SHOW_SYSTEM_INFO_STACK_SIZE 300        //10
/** @brief Stack size (in words) for Test Hardware task (ID 11). */
#define TASK_TEST_HARDWARE_STACK_SIZE 300             //11

// Tasks execution periods in ms
#define TASK_ERROR_HANDLER_REFRESH_RATE 500     // 0
#define TASK_SERIAL_DEBUGGER_REFRESH_RATE 2000  // 1
#define TASK_MAIN_REFRESH_RATE 500              // 2
#define TASK_READ_INPUT_REFRESH_RATE 100        // 3  
#define TASK_SERIAL_INPUT_REFRESH_RATE 200      // 4
#define TASK_UPDATE_SCREEN_REFRESH_RATE 200     // 5
#define TASK_READ_TEMP_REFRESH_RATE 2000        // 6
#define TASK_REGULATE_TEMP_REFRESH_RATE 5000    // 7
#define TASK_SELECT_DRINK_REFRESH_RATE 1000     // 8
#define TASK_ORDER_DRINK_REFRESH_RATE 500       // 9
#define TASK_SHOW_SYSTEM_INFO_REFRESH_RATE 600  // 10

// Timeout for closing welcome screen task
#define TASK_WELCOME_TICKS_TO_CLOSE 10

/**
 * @brief Size of the guard zone (in bytes) placed between task stacks.
 */
#define GUARD_ZONE_SIZE 32
/**
 * @brief Memory fill pattern for guard zones, used to detect overflow.
 */
#define MEMORY_FILL_PATTERN 0xAA


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
extern StaticTask_t testHardwareTCB;                                ///< Task Control Block for Welcome Screen task (ID 11)
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
extern StackType_t testHardwareStack[];   ///< Stack for Welcome Screen task (ID 11)
/**
 * @brief Array of pointers to all guard zones.
 * 
 * Used by the stack overflow detection routine to check each task's guard zone.
 */
extern volatile StackType_t*guardZones[]; ///< Array of pointers to guard zones for all tasks

// ========================
// Synchronization Objects
// ========================
// (Queues, semaphores, mutexes for inter-task communication)
#define SCREEN_QUEUE_BUFFER_COUNT 3
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

// ========================
// Global UI Context
// ========================
// (Structures holding UI state, LCD/menu state, user selections)

extern volatile struct sUIContext UI_Context;

// ========================
// Global Variables
// ========================
// (Flags, error codes, counters used across multiple modules)

extern struct sSystemError lastSystemError;
extern uint8_t f_errorConfirmed;
extern uint16_t bootupsCount;

extern float currentTemperature;
extern float setTemperature;
extern float temperatureHysteresis;

// ========================
// Drink Configuration
// ========================
// (Recipes, ingredient-to-pump mapping, maximum drink limits)

#define DRINK_COUNT 10

extern const struct sDrinkData drink[];
//
extern const char ingredients[][20-4-4];
//
extern const uint16_t pumpsEff[];
// Pumps efficiency in ml/min

// ========================
// Button Mapping
// ========================
// (Definitions mapping hardware buttons to logical actions)

#define E_GREEN_BUTTON 1
#define E_LWHITE_BUTTON 2
#define E_RWHITE_BUTTON 4
#define E_BLUE_BUTTON 8
#define E_RED_BUTTON 16

// ========================
// Hardware adddresses and configuration
// ========================

#define LCD_WIDTH 20
#define LCD_HEIGHT 4
#define LCD_ADDR 0x27

#define MCP_ADDR 0x20

#define E_LOADING_BAR 17
// LCD custom characters

#define SHOW_INFO_MENUS_COUNT 4 // later needs to be changed
// Show info submenu count

#define DRINK_SELECT 0
#define DRINK_ORDER 1
#define SHOW_INFO 2
#define TEST_HARDWARE 3

#endif // _DRINK_CREATOR6000_CONFIG_C_H_