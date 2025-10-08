#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include <DrinkCreator6000_Config.h>
#include <DrinkCreator6000_Init.h>
#include <DrinkCreator6000_EEPROM.h>
#include <DrinkCreator6000_Tasks.h>
#include <DrinkCreator6000_Progmem.h>
#include <DrinkCreator6000_RamStats.h>
#include <DrinkCreator6000_Pins.h>
#include <buzzer.h>
#include <board_io.h>

#include <systemDebug.h>
#include <uart.h>
#include <i2c.h>

void clearPumps(void){
    uint8_t flag=0x01;
    for(int i=0;i<8;i++){
        shiftOut(flag);
        flag<<=1;
        _delay_ms(10000);
    }

    flag=0x01;

    for(int i=0;i<8;i++){
        shiftOut(flag);
        _delay_ms(10000);

        shiftOut(0x00);
        _delay_ms(10000);

        shiftOut(flag);
        _delay_ms(10000);

        flag<<=1;
    }
    shiftOut(0x00);

    activateBuzzer(0);
    _delay_ms(15000);
    deactivateBuzzer();
}

void startupScreen(){
    sScreenData screenData{};  
    sprintf(screenData.lines[0],"%s","Drink Creator 6000");
    sprintf(screenData.lines[1],"%s","Initializing...");
    sprintf(screenData.lines[2],"%12s %2d %%","[----------]",0);
    sprintf(screenData.lines[3],"%s","Please wait");

    for(int k=0;k<11;k++){
        memset((void*)(screenData.lines[2]+1),0xFF,k);
	    sprintf(screenData.lines[2]+13,"%3u %%",10*k);
        for(int i=0;i<LCD_HEIGHT;i++){
            lcd.setCursor_blocking(0,i);
            for(int j=0;j<LCD_WIDTH;j++){
                if(screenData.lines[i][j]==0)
                    lcd.write_blocking(' ');
                else if(screenData.lines[i][j]==E_LOADING_BAR)
                    lcd.write_blocking(0);
                else if(screenData.lines[i][j]==18)
                    lcd.write_blocking(1);
                else if(screenData.lines[i][j]==19)
                    lcd.write_blocking(2);
                else if(screenData.lines[i][j]==20)
                    lcd.write_blocking(3);
                else if(screenData.lines[i][j]==21)
                    lcd.write_blocking(4);
                else if(screenData.lines[i][j]==22)
                    lcd.write_blocking(5);
                else if(screenData.lines[i][j]==23)
                    lcd.write_blocking(6);
                else if(screenData.lines[i][j]==24)
                            lcd.write_blocking(7);
                else
                    lcd.write_blocking(screenData.lines[i][j]);
            }
        }
        _delay_ms(200);
    }
}

void normalStart(){
    char buffer[configMAX_TASK_NAME_LEN]={0};

    strncpy_P(buffer,taskErrorHandler_name,configMAX_TASK_NAME_LEN);

    taskHandles[TASK_ERROR_HANDLER]  =xTaskCreateStatic(taskErrorHandler        ,buffer,TASK_ERROR_HANDLER_STACK_SIZE          ,NULL,3,errorHandlerStack        ,&errorHandlerTCB);         // 0
    taskHandles[TASK_SERIAL_DEBUGGER]=xTaskCreateStatic(taskSerialSystemDebugger,"STACK DEBUG"  ,TASK_SERIAL_SYSTEM_DEBUGGER_STACK_SIZE ,NULL,1,serialSystemDebuggerStack,&serialSystemDebuggerTCB); // 1
    strncpy_P(buffer,taskMain_name,configMAX_TASK_NAME_LEN);
    taskHandles[TASK_MAIN]           =xTaskCreateStatic(taskMain                ,buffer,TASK_MAIN_STACK_SIZE                   ,NULL,1,mainStack                ,&mainTCB);                 // 2
    strncpy_P(buffer,taskReadInput_name,configMAX_TASK_NAME_LEN);
    taskHandles[TASK_READ_INPUT]     =xTaskCreateStatic(taskReadInput           ,buffer,TASK_READ_INPUT_STACK_SIZE             ,NULL,2,readInputStack           ,&readInputTCB);            // 3
    strncpy_P(buffer,taskSerialInput_name,configMAX_TASK_NAME_LEN);
    taskHandles[TASK_SERIAL_INPUT]   =xTaskCreateStatic(taskSerialInput         ,buffer,TASK_SERIAL_INPUT_STACK_SIZE           ,NULL,2,serialInputStack         ,&serialInputTCB);          // 4
    strncpy_P(buffer,taskUpdateScreen_name,configMAX_TASK_NAME_LEN);
    taskHandles[TASK_UPDATE_SCREEN]  =xTaskCreateStatic(taskUpdateScreen        ,buffer,TASK_UPDATE_SCREEN_STACK_SIZE          ,NULL,1,updateScreenStack        ,&updateScreenTCB);         // 5
    strncpy_P(buffer,taskReadInput_name,configMAX_TASK_NAME_LEN);
    taskHandles[TASK_READ_TEMP]      =xTaskCreateStatic(taskReadTemp            ,buffer,TASK_READ_TEMP_STACK_SIZE              ,NULL,1,readTempStack            ,&readTempTCB);             // 6
    strncpy_P(buffer,taskRegTemp_name,configMAX_TASK_NAME_LEN);
    taskHandles[TASK_REGULATE_TEMP]  =xTaskCreateStatic(taskRegulateTemp        ,buffer,TASK_REGULATE_TEMP_STACK_SIZE          ,NULL,1,regulateTempStack        ,&regulateTempTCB);         // 7
    strncpy_P(buffer,taskSelectDrink_name,configMAX_TASK_NAME_LEN);
    taskHandles[TASK_SELECT_DRINK]   =xTaskCreateStatic(taskSelectDrink         ,buffer,TASK_SELECT_DRINK_STACK_SIZE           ,NULL,1,selectDrinkStack         ,&selectDrinkTCB);          // 8
    strncpy_P(buffer,taskOrderDrink_name,configMAX_TASK_NAME_LEN);
    taskHandles[TASK_ORDER_DRINK]    =xTaskCreateStatic(taskOrderDrink          ,buffer,TASK_ORDER_DRINK_STACK_SIZE            ,NULL,1,orderDrinkStack          ,&orderDrinkTCB);           // 9
    strncpy_P(buffer,taskShowInfo_name,configMAX_TASK_NAME_LEN);
    taskHandles[TASK_SHOW_SYS_INFO]  =xTaskCreateStatic(taskShowSystemInfo      ,buffer,TASK_SHOW_SYSTEM_INFO_STACK_SIZE       ,NULL,1,showSystemInfoStack      ,&showSystemInfoTCB);       // 10
    taskHandles[TASK_TEST_HARDWARE]  =xTaskCreateStatic(taskTestHardware        ,"TEST HW"      ,TASK_TEST_HARDWARE_STACK_SIZE          ,NULL,1,testHardwareStack        ,&testHardwareTCB);         // 11    
  
    UI_Context.currentTask=DRINK_SELECT;
    UI_Context.currentMenu=0;
    UI_Context.currentSubMenu=0;
}
void faultStart(){
    // After fault operating mode
    // UI_Context should switch to last error menu
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
    taskHandles[TASK_TEST_HARDWARE]  =xTaskCreateStatic(taskTestHardware        ,"TEST HW"      ,TASK_TEST_HARDWARE_STACK_SIZE          ,NULL,1,testHardwareStack        ,&testHardwareTCB);         // 11
    UI_Context.currentTask=SHOW_INFO;
    UI_Context.currentMenu=0;
    UI_Context.currentSubMenu=0;
}
extern "C" void vApplicationStackOverflowHook(TaskHandle_t xTask,char*pcTaskName){
    xQueueSend(qErrorId,&xTask,pdMS_TO_TICKS(50));
    //Wake up higher prority tasks
}

extern void softwareReset(void);

int main(void){
    // After vTaskStartScheduler(), SP will change dynamically depending on the active task.
    // So we treat this saved SP as the top of the main stack (pre-RTOS).
    activateBuzzer(0);
    _delay_ms(33);
    deactivateBuzzer();
    _delay_ms(50);
    activateBuzzer(0);
    _delay_ms(33);
    deactivateBuzzer();

    //circulation_on();

    //fans_on();

    //cooler_on();

    EEPROMUpdateBootups(&bootupsCount);
    EEPROMGetLastStartupError(&lastSystemError);

    if(lastSystemError.confirmed){
        uart_puts_P_blocking(msg_NormalStartUp);
        startupScreen();
        normalStart();
    }
    else{
        uart_puts_P_blocking(msg_FaultStartUp);
        startupScreen();
        faultStart();
    }
    uart_putc_blocking('\n');

    lastBootup_dump(&bootupsCount);  
    lastError_dump(&lastSystemError);

    __stack_ptr=(uint8_t*)SP;
        sei();
    ram_dump();

    vTaskStartScheduler();
    return 0;
}