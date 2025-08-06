#include <DrinkCreator6000_Progmem.h>
#include <FreeRTOSConfig.h>
#include <DrinkCreator6000_Config_C.h>

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

// Messages for last error dump
const char msg_lastError_header[]      PROGMEM="[XXXXX]================[LAST ERROR DUMP]=================[XXXXX]\n";
const char msg_lastError_footer[]      PROGMEM="[XXXXX]================[LAST ERROR DUMP]=================[XXXXX]\n";
const char msg_lastError_failureAfter[]PROGMEM="[XXXXX]Failure after:           ";

// Messages for last bootup dump
const char msg_lastBootup_header[]PROGMEM="[#####] Bootups count: ";

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
const char msg_UartReady[]         PROGMEM="[  1  ] UART is ready!        [#####]\n";
const char msg_IOInitialized[]     PROGMEM="[  2  ] IO initialized        [#####]\n";
const char msg_EEPROMReady[]       PROGMEM="[  3  ] EEPROM is ready!      [#####]\n";
const char msg_RamSizeInitialized[]PROGMEM="[  4  ] RAM Size initialized! [#####]\n";
const char msg_MemoryInitialized[] PROGMEM="[  5  ] Memory initialized    [#####]\n";
const char msg_I2CReady[]          PROGMEM="[  6  ] I2C is ready!         [#####]\n";
const char msg_InterruptsAttached[]PROGMEM="[  7  ] Interrupts ready!     [#####]\n";
const char msg_NormalStartUp[]     PROGMEM="[  8  ] Start up ...          [#####]\n";
const char msg_FaultStartUp[]      PROGMEM="[  8  ] Fault start up ...    [#####]\n";

// 
const char taskErrorHandler_name[configMAX_TASK_NAME_LEN]           PROGMEM="ERROR HANDLER";
const char taskStackDebug_name[configMAX_TASK_NAME_LEN]             PROGMEM="STACK DEBUG";
const char taskMain_name[configMAX_TASK_NAME_LEN]                   PROGMEM="MAIN";
const char taskReadInput_name[configMAX_TASK_NAME_LEN]              PROGMEM="READ INPUT";
const char taskSerialInput_name[configMAX_TASK_NAME_LEN]            PROGMEM="SERIAL INPUT";
const char taskUpdateScreen_name[configMAX_TASK_NAME_LEN]           PROGMEM="UPDATE SCREEN";
const char taskReadTemp_name[configMAX_TASK_NAME_LEN]               PROGMEM="READ TEMP";
const char taskRegTemp_name[configMAX_TASK_NAME_LEN]                PROGMEM="REGULATE TEMP";
const char taskSelectDrink_name[configMAX_TASK_NAME_LEN]            PROGMEM="SELECT DRINK";
const char taskOrderDrink_name[configMAX_TASK_NAME_LEN]             PROGMEM="ORDER DRINK";
const char taskShowInfo_name[configMAX_TASK_NAME_LEN]               PROGMEM="SHOW INFO";
const char taskWelcome_name[configMAX_TASK_NAME_LEN]                PROGMEM="WELCOME";
const char*const taskNames[TASK_N]                                  PROGMEM={
    taskErrorHandler_name,
    taskStackDebug_name,
    taskMain_name,
    taskReadInput_name,
    taskSerialInput_name,
    taskUpdateScreen_name,
    taskReadTemp_name,
    taskRegTemp_name,
    taskSelectDrink_name,
    taskOrderDrink_name,
    taskShowInfo_name,
    taskWelcome_name
};

#ifdef __cplusplus
}
#endif // __cplusplus