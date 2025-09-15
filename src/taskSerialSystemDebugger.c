#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include <taskSerialSystemDebugger.h>
#include <uart.h>
#include <DrinkCreator6000_Progmem.h>

#include <DrinkCreator6000_RamStats.h>

void ram_dump(void){
    /* buffer is initialized with NUL characters to avoid undefined content
      * Rule: MISRA C 2025 Dir 9.2 – all automatic variables must be initialized */
    char buffer[RAM_DUMP_BUFFER_LENGTH]={'\0'};
    /* Variable to store return value of snprintf
     * MISRA C 2025 17.x – return value of functions that may fail must be checked */
    int32_t snprintf_ret_val=0;

    updateMemoryUsage();

    /* Output headers stored in PROGMEM (MISRA 8.13 – use const for read-only data) */
    uart_puts_P_blocking(msg_ramDump_header1);
    uart_puts_P_blocking(msg_ramDump_header2);

    snprintf_ret_val=snprintf(buffer,sizeof(buffer),RAM_DUMP_ADDR_FMT,(unsigned int)(uintptr_t)&__data_start);
    if((snprintf_ret_val<0)||(snprintf_ret_val>=(int32_t)sizeof(buffer))){
        buffer[0]='\0';
    }
    uart_puts_P_blocking(msg_ramDump_data_header); 
    uart_puts_blocking(buffer);

    snprintf(buffer,sizeof(buffer),RAM_DUMP_ADDR_FMT,(uint16_t)&__data_end);
    uart_puts_P_blocking(msg_ramDump_addr_header);
    uart_puts_blocking(buffer);

    snprintf(buffer,sizeof(buffer),RAM_DUMP_SIZE_FMT,(uint16_t)((uint16_t)&__data_end-(uint16_t)&__data_start));
    uart_puts_P_blocking(msg_ramDump_seperator);
    uart_puts_blocking(buffer);
    uart_puts_P_blocking(msg_ramDump_data_footer);

  snprintf(buffer,sizeof(buffer),RAM_DUMP_ADDR_FMT,(uint16_t)&__bss_start);
  uart_puts_P_blocking(msg_ramDump_bss_header); uart_puts_blocking(buffer);
  snprintf(buffer,sizeof(buffer),RAM_DUMP_ADDR_FMT,(uint16_t)&__bss_end);
  uart_puts_P_blocking(msg_ramDump_addr_header); uart_puts_blocking(buffer);
  snprintf(buffer,sizeof(buffer),RAM_DUMP_SIZE_FMT,(uint16_t)((uint16_t)&__bss_end-(uint16_t)&__bss_start));
  uart_puts_P_blocking(msg_ramDump_seperator); uart_puts_blocking(buffer); uart_puts_P_blocking(msg_ramDump_bss_footer);

  snprintf(buffer,sizeof(buffer),RAM_DUMP_ADDR_FMT,(uint16_t)&__tdat_start);
  uart_puts_P_blocking(msg_ramDump_tdat_header); uart_puts_blocking(buffer);
  snprintf(buffer,sizeof(buffer),RAM_DUMP_ADDR_FMT,(uint16_t)&__tdat_end);
  uart_puts_P_blocking(msg_ramDump_addr_header); uart_puts_blocking(buffer);
  snprintf(buffer,sizeof(buffer),RAM_DUMP_SIZE_FMT,(uint16_t)((uint16_t)&__tdat_end-(uint16_t)&__tdat_start));
  uart_puts_P_blocking(msg_ramDump_seperator); uart_puts_blocking(buffer); uart_puts_P_blocking(msg_ramDump_tdat_footer);  

  snprintf(buffer,sizeof(buffer),RAM_DUMP_ADDR_FMT,(uint16_t)&__heap_start);
  uart_puts_P_blocking(msg_ramDump_heap_header); uart_puts_blocking(buffer);
  snprintf(buffer,sizeof(buffer),RAM_DUMP_ADDR_FMT,(uint16_t)__heap_end);
  uart_puts_P_blocking(msg_ramDump_addr_header); uart_puts_blocking(buffer);
  snprintf(buffer,sizeof(buffer),RAM_DUMP_SIZE_FMT,__heap_size);
  uart_puts_P_blocking(msg_ramDump_seperator); uart_puts_blocking(buffer); uart_puts_P_blocking(msg_ramDump_heap_footer);

  snprintf(buffer,sizeof(buffer),RAM_DUMP_ADDR_FMT,(uint16_t)__stack_ptr);
  uart_puts_P_blocking(msg_ramDump_stack_header); uart_puts_blocking(buffer);
  snprintf(buffer,sizeof(buffer),RAM_DUMP_ADDR_FMT,RAMEND);
  uart_puts_P_blocking(msg_ramDump_addr_header); uart_puts_blocking(buffer);
  snprintf(buffer,sizeof(buffer),RAM_DUMP_SIZE_FMT,__stack_size);
  uart_puts_P_blocking(msg_ramDump_seperator); uart_puts_blocking(buffer); uart_puts_P_blocking(msg_ramDump_stack_footer);

  uart_puts_P_blocking(msg_ramDump_free_header);
  snprintf(buffer,sizeof(buffer),RAM_DUMP_SIZE_FMT,ram_total_free);
  uart_puts_blocking(buffer);
  uart_puts_P_blocking(msg_ramDump_free_footer);

  uart_puts_P_blocking(msg_ramDump_header1);
}
void taskSerialSystemDebugger(void*pvParameters){
  char buffer[configMAX_TASK_NAME_LEN+1]={0};
  uint8_t i=0;
  uint8_t nameLength={0};
  TaskStatus_t taskStatus={0};
  for(;;){
      uart_putc_blocking('\n');
      uart_puts_P_blocking(msg_serialSystemDebugger_header1);
      uart_puts_P_blocking(msg_serialSystemDebugger_header2);
      for(i=0;i<TASK_N;i++){
        memset((void*)buffer,0,sizeof(buffer));
        if(taskHandles[i]==NULL)
          continue;
        vTaskGetInfo(taskHandles[i],&taskStatus,pdTRUE,eInvalid);
        sprintf(buffer,"%s",taskStatus.pcTaskName);
        nameLength=strlen(taskStatus.pcTaskName);
        memset(buffer+nameLength,' ',sizeof(buffer)-nameLength);
        buffer[configMAX_TASK_NAME_LEN-1]='\0';
        uart_putc_blocking('['); uart_puts_blocking(buffer); uart_putc_blocking(']');
        sprintf(buffer,"%5d",taskStatus.usStackHighWaterMark);
        uart_puts_blocking(buffer); uart_putc_blocking('|');
        switch(taskStatus.eCurrentState){
          case eReady:
            uart_puts_P_blocking(msg_serialSystemDebugger_taskReady);
          break;
          case eRunning:
            uart_puts_P_blocking(msg_serialSystemDebugger_taskRunning);
          break;
          case eBlocked:
            uart_puts_P_blocking(msg_serialSystemDebugger_taskBlocked);
            break;
          case eSuspended:
            uart_puts_P_blocking(msg_serialSystemDebugger_taskSuspended);
          break;
          case eDeleted:
            uart_puts_P_blocking(msg_serialSystemDebugger_taskDeleted);
            break;
          default:
            uart_puts_P_blocking(msg_serialSystemDebugger_taskInvalid);
          }
        snprintf(buffer,sizeof(buffer),"%d",taskStatus.uxCurrentPriority);
        uart_puts_blocking("| "); uart_puts_blocking(buffer);
        sprintf(buffer,"%2d",i);
        uart_putc_blocking('['); uart_puts_blocking(buffer); uart_puts_blocking("]\n");
      }
    uart_puts_P_blocking(msg_serialSystemDebugger_footer);
	  uart_putc_blocking('\n');

    updateMemoryUsage();
	  ram_dump();
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}
