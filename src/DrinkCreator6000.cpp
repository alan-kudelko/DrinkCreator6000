#include "DrinkCreator6000_Config.h"
#include "DrinkCreator6000_Init.h"
#include "DrinkCreator6000_EEPROM.h"

#include "DrinkCreator6000_Tasks.h"

#include <Wire.h>
#include <util/delay.h>
#include <uart.h>
#include <i2c.h>
#include <avr/pgmspace.h>
#include <DrinkCreator6000_Progmem.h>
#include <DrinkCreator6000_RamStats.h>

#define LAST_ERROR_BUFFER_SIZE 51
#define LAST_BOOTUP_BUFFER_SIZE 6

void lastError_dump(const sSystemError*lastError){
  /* MISRA C 2025 Rule 21.6: snprintf is used with bounds checking */

  char buffer[LAST_ERROR_BUFFER_SIZE]={0};

  uart_puts_P_blocking(msg_lastError_header);

  uart_puts_P_blocking(msg_X_Marker);
  snprintf(buffer,sizeof(buffer),"%-50s",lastError->errorText);

  uart_puts_blocking(buffer);
  uart_puts_P_blocking(msg_X_Marker);
  uart_putc_blocking('\n');

  uart_puts_P_blocking(msg_lastError_failureAfter);
  snprintf(buffer,sizeof(buffer),"%3d days %2d h %2d min %2d s",lastError->days,lastError->hours,lastError->minutes,lastError->seconds);
  uart_puts_blocking(buffer);
  uart_puts_P_blocking(msg_X_Marker);
  uart_putc_blocking('\n');

  uart_puts_P_blocking(msg_lastError_header);
}
void lastBootup_dump(const uint16_t*bootup){
  /* MISRA C 2025 Rule 21.6: snprintf is used with bounds checking */
  char buffer[LAST_BOOTUP_BUFFER_SIZE]={0};

  uart_puts_P_blocking(msg_lastBootup_header);
  snprintf(buffer,sizeof(buffer),"%-5d",*bootup);
  uart_puts_blocking(buffer);
  uart_putc_blocking(' ');
  uart_puts_P_blocking(msg_HASH_Marker);
  uart_putc_blocking('\n');
}
void normalStart(){
  taskHandles[TASK_ERROR_HANDLER]  =xTaskCreateStatic(taskErrorHandler        ,"ERROR HANDLER",TASK_ERROR_HANDLER_STACK_SIZE          ,NULL,3,errorHandlerStack        ,&errorHandlerTCB);         // 0
  taskHandles[TASK_SERIAL_DEBUGGER]=xTaskCreateStatic(taskSerialSystemDebugger,"STACK DEBUG"  ,TASK_SERIAL_SYSTEM_DEBUGGER_STACK_SIZE ,NULL,1,serialSystemDebuggerStack,&serialSystemDebuggerTCB); // 1
  taskHandles[TASK_MAIN]           =xTaskCreateStatic(taskMain                ,"MAIN"         ,TASK_MAIN_STACK_SIZE                   ,NULL,1,mainStack                ,&mainTCB);                 // 2
  taskHandles[TASK_READ_INPUT]     =xTaskCreateStatic(taskReadInput           ,"READ INPUT"   ,TASK_READ_INPUT_STACK_SIZE             ,NULL,2,readInputStack           ,&readInputTCB);            // 3
  taskHandles[TASK_SERIAL_INPUT]   =xTaskCreateStatic(taskSerialInput         ,"SERIAL INPUT" ,TASK_SERIAL_INPUT_STACK_SIZE           ,NULL,2,serialInputStack         ,&serialInputTCB);          // 4
  taskHandles[TASK_UPDATE_SCREEN]  =xTaskCreateStatic(taskUpdateScreen        ,"UPDATE SCREEN",TASK_UPDATE_SCREEN_STACK_SIZE          ,NULL,1,updateScreenStack        ,&updateScreenTCB);         // 5
  taskHandles[TASK_READ_TEMP]      =xTaskCreateStatic(taskReadTemp            ,"READ TEMP"    ,TASK_READ_TEMP_STACK_SIZE              ,NULL,1,readTempStack            ,&readTempTCB);             // 6
  taskHandles[TASK_REGULATE_TEMP]  =xTaskCreateStatic(taskRegulateTemp        ,"REGULATE TEMP",TASK_REGULATE_TEMP_STACK_SIZE          ,NULL,1,regulateTempStack        ,&regulateTempTCB);         // 7
  taskHandles[TASK_SELECT_DRINK]   =xTaskCreateStatic(taskSelectDrink         ,"SELECT DRINK" ,TASK_SELECT_DRINK_STACK_SIZE           ,NULL,1,selectDrinkStack         ,&selectDrinkTCB);          // 8
  taskHandles[TASK_ORDER_DRINK]    =xTaskCreateStatic(taskOrderDrink          ,"ORDER DRINK"  ,TASK_ORDER_DRINK_STACK_SIZE            ,NULL,1,orderDrinkStack          ,&orderDrinkTCB);           // 9
  taskHandles[TASK_SHOW_SYS_INFO]  =xTaskCreateStatic(taskShowSystemInfo      ,"SHOW INFO"    ,TASK_SHOW_SYSTEM_INFO_STACK_SIZE       ,NULL,1,showSystemInfoStack      ,&showSystemInfoTCB);       // 10
  taskHandles[TASK_WELCOME_SCREEN] =xTaskCreateStatic(taskWelcomeScreen       ,"WELCOME"      ,TASK_WELCOME_SCREEN_STACK_SIZE         ,NULL,1,welcomeScreenStack       ,&welcomeScreenTCB);        // 11  
}
void faultStart(){
  // After fault operating mode
  // UI_Context should switch to last error menu
  // Task in which fault was detected shouldn't start up
  taskHandles[TASK_ERROR_HANDLER]  =xTaskCreateStatic(taskErrorHandler        ,"ERROR HANDLER",TASK_ERROR_HANDLER_STACK_SIZE          ,NULL,3,errorHandlerStack        ,&errorHandlerTCB);         // 0
  taskHandles[TASK_SERIAL_DEBUGGER]=xTaskCreateStatic(taskSerialSystemDebugger,"STACK DEBUG"  ,TASK_SERIAL_SYSTEM_DEBUGGER_STACK_SIZE ,NULL,1,serialSystemDebuggerStack,&serialSystemDebuggerTCB); // 1
  taskHandles[TASK_MAIN]           =xTaskCreateStatic(taskMain                ,"MAIN"         ,TASK_MAIN_STACK_SIZE                   ,NULL,1,mainStack                ,&mainTCB);                 // 2
  taskHandles[TASK_READ_INPUT]     =xTaskCreateStatic(taskReadInput           ,"READ INPUT"   ,TASK_READ_INPUT_STACK_SIZE             ,NULL,2,readInputStack           ,&readInputTCB);            // 3
  taskHandles[TASK_SERIAL_INPUT]   =xTaskCreateStatic(taskSerialInput         ,"SERIAL INPUT" ,TASK_SERIAL_INPUT_STACK_SIZE           ,NULL,2,serialInputStack         ,&serialInputTCB);          // 4
  taskHandles[TASK_UPDATE_SCREEN]  =xTaskCreateStatic(taskUpdateScreen        ,"UPDATE SCREEN",TASK_UPDATE_SCREEN_STACK_SIZE          ,NULL,1,updateScreenStack        ,&updateScreenTCB);         // 5
  taskHandles[TASK_READ_TEMP]      =xTaskCreateStatic(taskReadTemp            ,"READ TEMP"    ,TASK_READ_TEMP_STACK_SIZE              ,NULL,1,readTempStack            ,&readTempTCB);             // 6
  taskHandles[TASK_REGULATE_TEMP]  =xTaskCreateStatic(taskRegulateTemp        ,"REGULATE TEMP",TASK_REGULATE_TEMP_STACK_SIZE          ,NULL,1,regulateTempStack        ,&regulateTempTCB);         // 7
  taskHandles[TASK_SELECT_DRINK]   =xTaskCreateStatic(taskSelectDrink         ,"SELECT DRINK" ,TASK_SELECT_DRINK_STACK_SIZE           ,NULL,1,selectDrinkStack         ,&selectDrinkTCB);          // 8
  taskHandles[TASK_ORDER_DRINK]    =xTaskCreateStatic(taskOrderDrink          ,"ORDER DRINK"  ,TASK_ORDER_DRINK_STACK_SIZE            ,NULL,1,orderDrinkStack          ,&orderDrinkTCB);           // 9
  taskHandles[TASK_SHOW_SYS_INFO]  =xTaskCreateStatic(taskShowSystemInfo      ,"SHOW INFO"    ,TASK_SHOW_SYSTEM_INFO_STACK_SIZE       ,NULL,1,showSystemInfoStack      ,&showSystemInfoTCB);       // 10
  taskHandles[TASK_WELCOME_SCREEN]=NULL;
  
    UI_Context.currentTask=SHOW_INFO;
    UI_Context.currentMenu=4;
    UI_Context.currentSubMenu=0;
  
    xTaskNotify(taskHandles[TASK_SHOW_SYS_INFO],1,eSetValueWithOverwrite);
  
  //taskHandles[TASK_WELCOME_SCREEN] =xTaskCreateStatic(taskWelcomeScreen       ,"WELCOME"      ,TASK_WELCOME_SCREEN_STACK_SIZE         ,NULL,1,welcomeScreenStack       ,&welcomeScreenTCB);        // 11   
}
extern "C" void vApplicationStackOverflowHook(TaskHandle_t xTask,char*pcTaskName){
    xQueueSend(qErrorId,&xTask,pdMS_TO_TICKS(50));
  //Wake up higher prority tasks
}
void calibrateIdleLoop(){
    vTaskStartScheduler();
    vTaskSuspendAll();
    idleCounter=0;
    vTaskDelay(pdMS_TO_TICKS(INTERVAL_TICKS));
    idleCalib=idleCounter;
    xTaskResumeAll();
}
//////////////////////////////////////////////////////////////////
// Temperature regulation task:
// Uses global temperature variables to control Peltier elements
// by switching their pins HIGH or LOW based on hysteresis thresholds.
//////////////////////////////////////////////////////////////////


int main(void){
    // After vTaskStartScheduler(), SP will change dynamically depending on the active task.
    // So we treat this saved SP as the top of the main stack (pre-RTOS).  
    while(true);
    EEPROMUpdateBootups(&bootupsCount);
    EEPROMGetLastStartupError(&lastSystemError);

    if(lastSystemError.confirmed){
      uart_puts_P_blocking(msg_NormalStartUp);
      normalStart();
    }
    else{
      uart_puts_P_blocking(msg_FaultStartUp);
      faultStart();
    }
    uart_putc_blocking('\n');

    lastBootup_dump(&bootupsCount);  
    lastError_dump(&lastSystemError);

    __stack_ptr=(uint8_t*)SP;
    ram_dump();

    _delay_ms(100);


    vTaskStartScheduler();
    // calibrate max value of idleCounterPerSecond
    // calibrateIdleLoop(); architecture should be changed to main setup task for this to work
    //vTaskStartScheduler();
}