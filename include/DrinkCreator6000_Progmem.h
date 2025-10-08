/**
 * @file DrinkCreator6000_Progmem.h
 * @brief Constant strings stored in flash memory (PROGMEM) for diagnostics and display.
 *
 * This file contains declarations of constant strings used for:
 * - diagnostics and reports sent over UART,
 * - LCD display messages,
 * - FreeRTOS task names,
 * - and other symbols used for data formatting.
 *
 * All strings are stored in flash memory to save RAM on the microcontroller.
 */

#ifndef _DRINK_CREATOR_6000_PROGMEM_H_
    #define _DRINK_CREATOR_6000_PROGMEM_H_

#include <avr/pgmspace.h>
/** @name Headers and footers for error reports and system startup messages
 *  @{
 */
extern const char msg_lastError_header[]            PROGMEM; /**< Header text for last error report */
extern const char msg_lastError_footer[]            PROGMEM; /**< Footer text for last error report */
extern const char msg_lastError_failureAfter[]      PROGMEM; /**< Error report postfix showing failure reason */

extern const char msg_lastBootup_header[]           PROGMEM; /**< Header text for last boot-up report */
/** @} */

/** @name Headers and footers for RAM memory dumps
 *  @{
 */
extern const char msg_ramDump_header1[]             PROGMEM; /**< General RAM dump header part 1 */
extern const char msg_ramDump_header2[]             PROGMEM; /**< General RAM dump header part 2 */
extern const char msg_ramDump_data_header[]         PROGMEM; /**< Header for initialized data section */
extern const char msg_ramDump_data_footer[]         PROGMEM; /**< Footer for initialized data section */
extern const char msg_ramDump_bss_header[]          PROGMEM; /**< Header for BSS section */
extern const char msg_ramDump_bss_footer[]          PROGMEM; /**< Footer for BSS section */
extern const char msg_ramDump_tdat_header[]         PROGMEM; /**< Header for task data section */
extern const char msg_ramDump_tdat_footer[]         PROGMEM; /**< Footer for task data section */
extern const char msg_ramDump_heap_header[]         PROGMEM; /**< Header for heap section */
extern const char msg_ramDump_heap_footer[]         PROGMEM; /**< Footer for heap section */
extern const char msg_ramDump_stack_header[]        PROGMEM; /**< Header for stack section */
extern const char msg_ramDump_stack_footer[]        PROGMEM; /**< Footer for stack section */
extern const char msg_ramDump_free_header[]         PROGMEM; /**< Header for free RAM section */
extern const char msg_ramDump_free_footer[]         PROGMEM; /**< Footer for free RAM section */
extern const char msg_ramDump_addr_header[]         PROGMEM; /**< Header for memory address listings */
extern const char msg_ramDump_seperator[]           PROGMEM; /**< Separator string for memory output */
/** @} */

/** @name Diagnostic messages for the UART system debugger
 *  @{
 */
extern const char msg_serialSystemDebugger_header1[]       PROGMEM; /**< First line of UART system debugger header */
extern const char msg_serialSystemDebugger_header2[]       PROGMEM; /**< Second line of UART system debugger header */
extern const char msg_serialSystemDebugger_footer[]        PROGMEM; /**< Footer for UART system debugger output */
extern const char msg_serialSystemDebugger_taskReady[]     PROGMEM; /**< Task status: Ready */
extern const char msg_serialSystemDebugger_taskRunning[]   PROGMEM; /**< Task status: Running */
extern const char msg_serialSystemDebugger_taskBlocked[]   PROGMEM; /**< Task status: Blocked */
extern const char msg_serialSystemDebugger_taskSuspended[] PROGMEM; /**< Task status: Suspended */
extern const char msg_serialSystemDebugger_taskDeleted[]   PROGMEM; /**< Task status: Deleted */
extern const char msg_serialSystemDebugger_taskInvalid[]   PROGMEM; /**< Task status: Invalid */
/** @} */

/** @name Messages and headers for error handling and other system states
 *  @{
 */
extern const char msg_errorHandler_header[]           PROGMEM; /**< Header for error handler output */

extern const char msg_X_Marker[]                      PROGMEM; /**< Marker string: 'X' */
extern const char msg_HASH_Marker[]                   PROGMEM; /**< Marker string: '#' */

extern const char msg_UartReady[]                     PROGMEM; /**< Message indicating UART is initialized */
extern const char msg_IOInitialized[]                 PROGMEM; /**< Message indicating I/O initialization completed */
extern const char msg_EEPROMReady[]                   PROGMEM; /**< Message indicating EEPROM is ready */
extern const char msg_RamSizeInitialized[]            PROGMEM;
extern const char msg_MemoryInitialized[]             PROGMEM; /**< Message indicating memory subsystem is initialized */
extern const char msg_I2CReady[]                      PROGMEM; /**< Message indicating hardware components are ready */
extern const char msg_lcdReady[]                      PROGMEM; /**< Message indicating lcd is ready */
extern const char msg_mcpReady[]                      PROGMEM; /**< Message indicating mcp is ready */
extern const char msg_InterruptsAttached[]            PROGMEM; /**< Message indicating interrupts are attached */
extern const char msg_NormalStartUp[]                 PROGMEM; /**< Message indicating normal system startup */
extern const char msg_FaultStartUp[]                  PROGMEM; /**< Message indicating system startup with faults */
/** @} */

/** @name FreeRTOS task names stored in PROGMEM
 *  @{
 */
extern const char taskErrorHandler_name[]             PROGMEM; /**< Task name: Error Handler */
extern const char taskStackDebug_name[]               PROGMEM; /**< Task name: Stack Debug */
extern const char taskMain_name[]                     PROGMEM; /**< Task name: Main */
extern const char taskReadInput_name[]                PROGMEM; /**< Task name: Read Input */
extern const char taskSerialInput_name[]              PROGMEM; /**< Task name: Serial Input */
extern const char taskUpdateScreen_name[]             PROGMEM; /**< Task name: Update Screen */
extern const char taskReadTemp_name[]                 PROGMEM; /**< Task name: Read Temperature */
extern const char taskRegTemp_name[]                  PROGMEM; /**< Task name: Regulate Temperature */
extern const char taskSelectDrink_name[]              PROGMEM; /**< Task name: Select Drink */
extern const char taskOrderDrink_name[]               PROGMEM; /**< Task name: Order Drink */
extern const char taskShowInfo_name[]                 PROGMEM; /**< Task name: Show Info */
extern const char taskTestHW_name[]                   PROGMEM; /**< Task name: Test hardware */

extern const char* const taskNames[]                  PROGMEM; /**< Array of all task name strings */
/** @} */

/** @name Cleaning chemicals used to clean the machine stored in PROGMEM
 *  @{
 */

extern const char cleaningCycle1_name[] PROGMEM;
extern const char cleaningCycle2_name[] PROGMEM;
extern const char cleaningCycle3_name[] PROGMEM;
extern const char cleaningCycle4_name[] PROGMEM;
extern const char cleaningCycle5_name[] PROGMEM;

extern const char*const cleaningCycles_names[] PROGMEM;

/** @} */

#endif // _DRINK_CREATOR_6000_PROGMEM_H_