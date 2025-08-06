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
extern const char msg_lastError_header[]            PROGMEM;
extern const char msg_lastError_footer[]            PROGMEM;
extern const char msg_lastError_failureAfter[]      PROGMEM;

extern const char msg_lastBootup_header[]           PROGMEM;
/** @} */

/** @name Headers and footers for RAM memory dumps
 *  @{
 */
extern const char msg_ramDump_header1[]             PROGMEM;
extern const char msg_ramDump_header2[]             PROGMEM;
extern const char msg_ramDump_data_header[]         PROGMEM;
extern const char msg_ramDump_data_footer[]         PROGMEM;
extern const char msg_ramDump_bss_header[]          PROGMEM;
extern const char msg_ramDump_bss_footer[]          PROGMEM;
extern const char msg_ramDump_tdat_header[]         PROGMEM;
extern const char msg_ramDump_tdat_footer[]         PROGMEM;
extern const char msg_ramDump_heap_header[]         PROGMEM;
extern const char msg_ramDump_heap_footer[]         PROGMEM;
extern const char msg_ramDump_stack_header[]        PROGMEM;
extern const char msg_ramDump_stack_footer[]        PROGMEM;
extern const char msg_ramDump_free_header[]         PROGMEM;
extern const char msg_ramDump_free_footer[]         PROGMEM;
extern const char msg_ramDump_addr_header[]         PROGMEM;
extern const char msg_ramDump_seperator[]           PROGMEM;
/** @} */

/** @name Diagnostic messages for the UART system debugger
 *  @{
 */
extern const char msg_serialSystemDebugger_header1[]       PROGMEM;
extern const char msg_serialSystemDebugger_header2[]       PROGMEM;
extern const char msg_serialSystemDebugger_footer[]        PROGMEM;
extern const char msg_serialSystemDebugger_taskReady[]     PROGMEM;
extern const char msg_serialSystemDebugger_taskRunning[]   PROGMEM;
extern const char msg_serialSystemDebugger_taskBlocked[]   PROGMEM;
extern const char msg_serialSystemDebugger_taskSuspended[] PROGMEM;
extern const char msg_serialSystemDebugger_taskDeleted[]   PROGMEM;
extern const char msg_serialSystemDebugger_taskInvalid[]   PROGMEM;
/** @} */

/** @name Messages and headers for error handling and other system states
 *  @{
 */
extern const char msg_errorHandler_header[]           PROGMEM;

extern const char msg_X_Marker[]                      PROGMEM;
extern const char msg_HASH_Marker[]                   PROGMEM;

extern const char msg_UartReady[]                     PROGMEM;
extern const char msg_IOInitialized[]                 PROGMEM;
extern const char msg_EEPROMReady[]                   PROGMEM;
extern const char msg_MemoryInitialized[]             PROGMEM;
extern const char msg_HardwareReady[]                 PROGMEM;
extern const char msg_InterruptsAttached[]            PROGMEM;
extern const char msg_NormalStartUp[]                 PROGMEM;
extern const char msg_FaultStartUp[]                  PROGMEM;
/** @} */

/** @name FreeRTOS task names stored in PROGMEM
 *  @{
 */
extern const char taskErrorHandler_name[]             PROGMEM;
extern const char taskStackDebug_name[]               PROGMEM;
extern const char taskMain_name[]                     PROGMEM;
extern const char taskReadInput_name[]                PROGMEM;
extern const char taskSerialInput_name[]              PROGMEM;
extern const char taskUpdateScreen_name[]             PROGMEM;
extern const char taskReadTemp_name[]                 PROGMEM;
extern const char taskRegTemp_name[]                  PROGMEM;
extern const char taskSelectDrink_name[]              PROGMEM;
extern const char taskOrderDrink_name[]               PROGMEM;
extern const char taskShowInfo_name[]                 PROGMEM;
extern const char taskWelcome_name[]                  PROGMEM;

extern const char* const taskNames[]                  PROGMEM;
/** @} */

#endif // _DRINK_CREATOR_6000_PROGMEM_H_