#include <DrinkCreator6000_Progmem.h>

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

// Messages for last error dump
const char msg_lastError_header[]      PROGMEM="[XXXXX]================[LAST ERROR DUMP]=================[XXXXX]\n";
const char msg_lastError_footer[]      PROGMEM="[XXXXX]================[LAST ERROR DUMP]=================[XXXXX]\n";
const char msg_lastError_failureAfter[]PROGMEM="[XXXXX]Failure after:           ";

// Messages for last bootup dump
const char msg_lastBootup_header[]PROGMEM="[#####]====[MEMORY STATUS]====[#####]\n";

// Memory dump messages
const char msg_ramDump_header1[]     PROGMEM="[#####]====[MEMORY STATUS]====[#####]\n";
const char msg_ramDump_header2[]     PROGMEM="[     ] START |  END  | SIZE  [     ]\n";
const char msg_ramDump_data_header[] PROGMEM="[.DATA] 0x";
const char msg_ramDump_data_footer[] PROGMEM=" B[.DATA]\n";
const char msg_ramDump_bss_header[]  PROGMEM="[.BSS ] 0x";
const char msg_ramDump_bss_footer[]  PROGMEM=" B[.BSS ]\n";
const char msg_ramDump_tdat_header[] PROGMEM="[.TDAT] 0x";
const char msg_ramDump_tdat_footer[] PROGMEM=" B[.TDAT]\n";
const char msg_ramDump_heap_header[] PROGMEM="[HEAP ] 0x";
const char msg_ramDump_heap_footer[] PROGMEM=" B[HEAP ]\n";
const char msg_ramDump_stack_header[]PROGMEM="[STACK] 0x";
const char msg_ramDump_stack_footer[]PROGMEM=" B[STACK]\n";
const char msg_ramDump_free_header[] PROGMEM="[FREE ]---------------| ";
const char msg_ramDump_free_footer[] PROGMEM=" B[FREE ]\n";
const char msg_ramDump_addr_header[] PROGMEM="| 0x";
const char msg_ramDump_seperator[]   PROGMEM="| ";

// Serial system debugger messages
const char msg_serialSystemDebugger_header1[] PROGMEM="[####################]====TASK STATUS===[##]\n";
const char msg_serialSystemDebugger_header2[] PROGMEM="[     TASK NAME      ]STACK|  STATE  |PR[ID]\n";
const char msg_serialSystemDebugger_footer[]  PROGMEM="[####################]=================[##]\n";
const char msg_serialSystemDebugger_taskReady[]    PROGMEM="Ready    ";
const char msg_serialSystemDebugger_taskRunning[]  PROGMEM="Running  ";
const char msg_serialSystemDebugger_taskBlocked[]  PROGMEM="Blocked  ";
const char msg_serialSystemDebugger_taskSuspended[]PROGMEM="Suspended";
const char msg_serialSystemDebugger_taskDeleted[]  PROGMEM="Deleted  ";
const char msg_serialSystemDebugger_taskInvalid[]  PROGMEM="Invalid  ";

// Error handler messages
const char msg_errorHandler_header[] PROGMEM="[####################]====SYSTEM CRITICAL====[##]\n";

// Markers for formatting
const char msg_X_Marker[]   PROGMEM="[XXXXX]";
const char msg_HASH_Marker[]PROGMEM="[#####]";
// Messages for initialization and startup
const char msg_UartReady[]         PROGMEM="[  1  ] UART is ready!       [#####]\n";
const char msg_EEPROMReady[]       PROGMEM="[  2  ] EEPROM is ready!     [#####]\n";
const char msg_IOInitialized[]     PROGMEM="[  3  ] IO initialized       [#####]\n";
const char msg_MemoryInitialized[] PROGMEM="[  4  ] Memory initialized   [#####]\n";
const char msg_HardwareReady[]     PROGMEM="[  5  ] Hardware is ready!   [#####]\n";
const char msg_InterruptsAttached[]PROGMEM="[  6  ] Interrupts attached! [#####]\n";
const char msg_NormalStartUp[]     PROGMEM="[  7  ] Start up ...         [#####]\n";
const char msg_FaultStartUp[]      PROGMEM="[  7  ] Fault start up ...   [#####]\n";

#ifdef __cplusplus
}
#endif // __cplusplus