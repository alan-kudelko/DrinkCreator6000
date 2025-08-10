#include "DrinkCreator6000_Config.h"
#include "DrinkCreator6000_Init.h"
#include "DrinkCreator6000_EEPROM.h"

#include "DrinkCreator6000_Tasks.h"

#include <util/delay.h>
#include <uart.h>
#include <i2c.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include <stdio.h>
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

void printI2C_status(){
      uart_puts_blocking("Stan ring buffera (is empty):");
      uart_put_hex_blocking(i2c_tx_buffer_is_empty());
      uart_putc_blocking('\n');

      uart_puts_blocking("Status FSM:");
      uart_put_hex_blocking(i2c_get_status());
      uart_putc_blocking('\n');

      uart_puts_blocking("head:");
      uart_put_hex_blocking(i2c_tx_buffer_head);
      uart_putc_blocking('\n');

      uart_puts_blocking("tail:");
      uart_put_hex_blocking(i2c_tx_buffer_tail);
      uart_putc_blocking('\n');
}

#define LCD_ADDR       0x27 // 7-bit
#define LCD_BACKLIGHT  0x08
#define LCD_ENABLE     0x04
#define LCD_RS         0x01

// ---------------- LCD low-level ----------------

static void lcd_write4(uint8_t nibble) {
    i2c_write_byte_blocking(LCD_ADDR, nibble | LCD_BACKLIGHT);
    _delay_us(150);
    i2c_write_byte_blocking(LCD_ADDR, nibble | LCD_BACKLIGHT | LCD_ENABLE);
    _delay_us(1000);
    i2c_write_byte_blocking(LCD_ADDR, nibble | LCD_BACKLIGHT);
    _delay_us(150);
}

static void lcd_send(uint8_t value, uint8_t mode) {
    uint8_t high = (value & 0xF0) | mode;
    uint8_t low  = ((value << 4) & 0xF0) | mode;
    lcd_write4(high);
    lcd_write4(low);
    _delay_us(150);
}

static void lcd_cmd(uint8_t cmd) {
    lcd_send(cmd, 0);
    if (cmd == 0x01 || cmd == 0x02) {
        _delay_ms(20);
    }
}

static void lcd_data(uint8_t data) {
    lcd_send(data, LCD_RS);
}

// ---------------- LCD init ----------------

static void lcd_init(void) {
    _delay_ms(50);

    lcd_write4(0x30);
    _delay_ms(5);
    lcd_write4(0x30);
    _delay_us(300);
    lcd_write4(0x30);
    _delay_us(300);
    lcd_write4(0x20); // 4-bit
    _delay_us(150);

    lcd_cmd(0x28); // 4-bit, 2 line, 5x8
    lcd_cmd(0x08); // display off
    lcd_cmd(0x01); // clear
    _delay_ms(2);
    lcd_cmd(0x06); // entry mode
    lcd_cmd(0x0C); // display on, cursor off
}

void send_hello_world() {
    // Tablica z danymi: najpierw adres z zapisem, potem znaki 'Hello world'
    uint8_t data[] = {
        (LCD_ADDR << 1) | WRITE_FLAG, // adres + write
        'H', 'e', 'l', 'l', 'o', ' ',
        'w', 'o', 'r', 'l', 'd'
    };
    uint8_t length = sizeof(data);

    i2c_write_bytes_blocking(0x20, data + 1, length - 1);
}

void test_lcd_sequence(void) {
    // Inicjalizacja LCD 4-bit
    printI2C_status();
    lcd_write4(0x30);  // tryb 8-bit, 1 raz
    _delay_ms(2000);
    printI2C_status();
    lcd_write4(0x30);  // tryb 8-bit, 2 raz
    _delay_ms(2000);
    printI2C_status();
    lcd_write4(0x30);  // tryb 8-bit, 3 raz
    _delay_ms(2000);
    printI2C_status();
    lcd_write4(0x20);  // tryb 4-bit
    _delay_ms(2000);
    printI2C_status();
    lcd_cmd(0x28);     // funkcja: 4-bit, 2 linie, 5x8 font
    _delay_ms(2000);
    printI2C_status();
    lcd_cmd(0x08);     // wyłącz wyświetlacz
    _delay_ms(2000);
    printI2C_status();
    lcd_cmd(0x01);     // wyczyść ekran
    _delay_ms(2000);
    printI2C_status();
    lcd_cmd(0x06);     // tryb wejścia: przesuwaj kursor w prawo
    _delay_ms(2000);
    printI2C_status();
    lcd_cmd(0x0C);     // włącz wyświetlacz, bez kursora
    _delay_ms(2000);

    // Wyświetlamy napis "Hello world" po kolei, 2 sekundy na znak
    send_hello_world();
    printI2C_status();
    _delay_ms(2000);
        printI2C_status();
    uart_puts_blocking("Liczba bledow transmisji i2c: ");
    uart_put_hex_blocking(i2c_tx_error_counter);
    uart_putc_blocking('\n');
}



int main(void){
    // After vTaskStartScheduler(), SP will change dynamically depending on the active task.
    // So we treat this saved SP as the top of the main stack (pre-RTOS).  
    EEPROMUpdateBootups(&bootupsCount);
    EEPROMGetLastStartupError(&lastSystemError);

    if(lastSystemError.confirmed){
     // uart_puts_P_blocking(msg_NormalStartUp);
      normalStart();
    }
    else{
      //uart_puts_P_blocking(msg_FaultStartUp);
      faultStart();
    }
    uart_putc_blocking('\n');

    //lastBootup_dump(&bootupsCount);  
    //lastError_dump(&lastSystemError);

    __stack_ptr=(uint8_t*)SP;
    ram_dump();


    if(true){
      test_lcd_sequence();
      
      while(true){

      }
    }


    _delay_ms(100);

    uint8_t c='A';
    
  lcd_init();

    while(true){
      uart_puts_blocking("Stan ring buffera (is empty):");
      uart_put_hex_blocking(i2c_tx_buffer_is_empty());
      uart_putc_blocking('\n');

      uart_puts_blocking("Status FSM:");
      uart_put_hex_blocking(i2c_get_status());
      uart_putc_blocking('\n');

      uart_puts_blocking("head:");
      uart_put_hex_blocking(i2c_tx_buffer_head);
      uart_putc_blocking('\n');

      uart_puts_blocking("tail:");
      uart_put_hex_blocking(i2c_tx_buffer_tail);
      uart_putc_blocking('\n');

      //_delay_ms(300);
                      lcd_data(0x01); // Clear display
        lcd_data((uint8_t)c);
        c++;
        if(c=='Z'+1)
          c='A';
          
      _delay_ms(1000);
    }


    //vTaskStartScheduler();
    // calibrate max value of idleCounterPerSecond
    // calibrateIdleLoop(); architecture should be changed to main setup task for this to work
    //vTaskStartScheduler();
}