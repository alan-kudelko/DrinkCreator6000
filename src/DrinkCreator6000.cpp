#include "DrinkCreator6000_Config.h"
#include "DrinkCreator6000_Init.h"
#include "DrinkCreator6000_EEPROM.h"

#include "DrinkCreator6000_Tasks.h"

#include <Wire.h>
#include <util/delay.h>

void lastError_dump(sSystemError*lastError){
  char buffer[70]{};
  Serial.println(F("[XXXXX]================[LAST ERROR DUMP]=================[XXXXX]"));
    sprintf(buffer,"[XXXXX]%50s[XXXXX]",lastError->errorText);
  Serial.println(buffer);
    sprintf(buffer,"[XXXXX]Failure after:           %3d days %2d h %2d min %2d s[XXXXX]",lastError->days,lastError->hours,lastError->minutes,lastError->seconds);
  Serial.println(buffer);
  Serial.println(F("[XXXXX]================[LAST ERROR DUMP]=================[XXXXX]"));
  Serial.println(F(""));
}
void lastBootup_dump(uint16_t*bootup){
  Serial.print(F("[#####]Bootups count: "));
  Serial.print(*bootup);
  Serial.println(F("[#####]"));
  Serial.println(F(""));
}
void normalStart(){
  taskHandles[TASK_ERROR_HANDLER]  =xTaskCreateStatic(taskErrorHandler        ,"ERROR HANDLER",TASK_ERROR_HANDLER_STACK_SIZE          ,NULL,3,errorHandlerStack        ,&errorHandlerTCB);         // 0
  taskHandles[TASK_SERIAL_DEBUGGER]=xTaskCreateStatic(taskSerialSystemDebugger,"STACK DEBUG"  ,TASK_SERIAL_SYSTEM_DEBUGGER_STACK_SIZE ,NULL,1,serialSystemDebuggerStack,&serialSystemDebuggerTCB); // 1
  //taskHandles[TASK_MAIN]           =xTaskCreateStatic(taskMain                ,"MAIN"         ,TASK_MAIN_STACK_SIZE                   ,NULL,1,mainStack                ,&mainTCB);                 // 2
  //taskHandles[TASK_READ_INPUT]     =xTaskCreateStatic(taskReadInput           ,"READ INPUT"   ,TASK_READ_INPUT_STACK_SIZE             ,NULL,2,readInputStack           ,&readInputTCB);            // 3
  //taskHandles[TASK_SERIAL_INPUT]   =xTaskCreateStatic(taskSerialInput         ,"SERIAL INPUT" ,TASK_SERIAL_INPUT_STACK_SIZE           ,NULL,2,serialInputStack         ,&serialInputTCB);          // 4
  taskHandles[TASK_UPDATE_SCREEN]  =xTaskCreateStatic(taskUpdateScreen        ,"UPDATE SCREEN",TASK_UPDATE_SCREEN_STACK_SIZE          ,NULL,1,updateScreenStack        ,&updateScreenTCB);         // 5
  //taskHandles[TASK_READ_TEMP]      =xTaskCreateStatic(taskReadTemp            ,"READ TEMP"    ,TASK_READ_TEMP_STACK_SIZE              ,NULL,1,readTempStack            ,&readTempTCB);             // 6
  //taskHandles[TASK_REGULATE_TEMP]  =xTaskCreateStatic(taskRegulateTemp        ,"REGULATE TEMP",TASK_REGULATE_TEMP_STACK_SIZE          ,NULL,1,regulateTempStack        ,&regulateTempTCB);         // 7
  taskHandles[TASK_SELECT_DRINK]   =xTaskCreateStatic(taskSelectDrink         ,"SELECT DRINK" ,TASK_SELECT_DRINK_STACK_SIZE           ,NULL,1,selectDrinkStack         ,&selectDrinkTCB);          // 8
  taskHandles[TASK_ORDER_DRINK]    =xTaskCreateStatic(taskOrderDrink          ,"ORDER DRINK"  ,TASK_ORDER_DRINK_STACK_SIZE            ,NULL,1,orderDrinkStack          ,&orderDrinkTCB);           // 9
  //taskHandles[TASK_SHOW_SYS_INFO]  =xTaskCreateStatic(taskShowSystemInfo      ,"SHOW INFO"    ,TASK_SHOW_SYSTEM_INFO_STACK_SIZE       ,NULL,1,showSystemInfoStack      ,&showSystemInfoTCB);       // 10
  taskHandles[TASK_WELCOME_SCREEN] =xTaskCreateStatic(taskWelcomeScreen       ,"WELCOME"      ,TASK_WELCOME_SCREEN_STACK_SIZE         ,NULL,1,welcomeScreenStack       ,&welcomeScreenTCB);        // 11  
}
void faultStart(){
  // After fault operating mode
  // UI_Context should switch to last error menu
  // Task in which fault was detected shouldn't start up
  taskHandles[TASK_ERROR_HANDLER]  =xTaskCreateStatic(taskErrorHandler        ,"ERROR HANDLER",TASK_ERROR_HANDLER_STACK_SIZE          ,NULL,3,errorHandlerStack        ,&errorHandlerTCB);         // 0
  taskHandles[TASK_SERIAL_DEBUGGER]=xTaskCreateStatic(taskSerialSystemDebugger,"STACK DEBUG"  ,TASK_SERIAL_SYSTEM_DEBUGGER_STACK_SIZE ,NULL,1,serialSystemDebuggerStack,&serialSystemDebuggerTCB); // 1
  //taskHandles[TASK_MAIN]           =xTaskCreateStatic(taskMain                ,"MAIN"         ,TASK_MAIN_STACK_SIZE                   ,NULL,1,mainStack                ,&mainTCB);                 // 2
  //taskHandles[TASK_READ_INPUT]     =xTaskCreateStatic(taskReadInput           ,"READ INPUT"   ,TASK_READ_INPUT_STACK_SIZE             ,NULL,2,readInputStack           ,&readInputTCB);            // 3
  //taskHandles[TASK_SERIAL_INPUT]   =xTaskCreateStatic(taskSerialInput         ,"SERIAL INPUT" ,TASK_SERIAL_INPUT_STACK_SIZE           ,NULL,2,serialInputStack         ,&serialInputTCB);          // 4
  taskHandles[TASK_UPDATE_SCREEN]  =xTaskCreateStatic(taskUpdateScreen        ,"UPDATE SCREEN",TASK_UPDATE_SCREEN_STACK_SIZE          ,NULL,1,updateScreenStack        ,&updateScreenTCB);         // 5
  //taskHandles[TASK_READ_TEMP]      =xTaskCreateStatic(taskReadTemp            ,"READ TEMP"    ,TASK_READ_TEMP_STACK_SIZE              ,NULL,1,readTempStack            ,&readTempTCB);             // 6
  //taskHandles[TASK_REGULATE_TEMP]  =xTaskCreateStatic(taskRegulateTemp        ,"REGULATE TEMP",TASK_REGULATE_TEMP_STACK_SIZE          ,NULL,1,regulateTempStack        ,&regulateTempTCB);         // 7
  taskHandles[TASK_SELECT_DRINK]   =xTaskCreateStatic(taskSelectDrink         ,"SELECT DRINK" ,TASK_SELECT_DRINK_STACK_SIZE           ,NULL,1,selectDrinkStack         ,&selectDrinkTCB);          // 8
  taskHandles[TASK_ORDER_DRINK]    =xTaskCreateStatic(taskOrderDrink          ,"ORDER DRINK"  ,TASK_ORDER_DRINK_STACK_SIZE            ,NULL,1,orderDrinkStack          ,&orderDrinkTCB);           // 9
  //taskHandles[TASK_SHOW_SYS_INFO]  =xTaskCreateStatic(taskShowSystemInfo      ,"SHOW INFO"    ,TASK_SHOW_SYSTEM_INFO_STACK_SIZE       ,NULL,1,showSystemInfoStack      ,&showSystemInfoTCB);       // 10
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

#define BAUD 9600
#define UBRR_VALUE ((F_CPU / (16UL * BAUD)) - 1)

void uart_init(void) {
    UBRR0H = (uint8_t)(UBRR_VALUE >> 8);
    UBRR0L = (uint8_t)UBRR_VALUE;
    UCSR0B = (1 << TXEN0); // Enable transmitter
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit data
}

void uart_putc(char c) {
    while (!(UCSR0A & (1 << UDRE0))); // Wait for empty transmit buffer
    UDR0 = c;
}

void uart_puts(const char* s) {
    while (*s) uart_putc(*s++);
}

int main(void){
    uart_init();
    sei();  // Enable global interrupts if needed

    uart_puts("Start!\r\n");

    while (1) {
        uart_puts("Hello Baremetal!\r\n");
        _delay_ms(500);
    }
 // while(!eeprom_is_ready());

  //EEPROMUpdateBootups(&bootupsCount);
  //EEPROMGetLastStartupError(&lastSystemError);
  
  //initializeIO();
  //initializeMemory();
  //initializeHardware();

  
  //attachInterrupt(digitalPinToInterrupt(INTPin),setInputFlag,FALLING);
  // For now I will try method without interrupt
  
  //lastBootup_dump(&bootupsCount);  
  //lastError_dump(&lastSystemError);
  __stack_ptr=(uint8_t*)SP;
  
  // After vTaskStartScheduler(), SP will change dynamically depending on the active task.
  // So we treat this saved SP as the top of the main stack (pre-RTOS).  
  
  ram_dump();  
  
 // if(lastSystemError.confirmed)
  //  normalStart();
  //else
  //  faultStart();
  // calibrate max value of idleCounterPerSecond
  // calibrateIdleLoop(); architecture should be changed to main setup task for this to work
  //vTaskStartScheduler();
  Serial.print("Werdon");
}