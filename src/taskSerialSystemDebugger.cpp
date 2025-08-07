#include <taskSerialSystemDebugger.h>
#include <uart.h>
#include <DrinkCreator6000_Progmem.h>
#include <FreeRTOS.h>

#include <DrinkCreator6000_RamStats.h>

void ram_dump(){
  // Convert code to be MISRA C 2025 compliant
  char buffer[5]={0};
  updateMemoryUsage();
  
  //uart_puts_P(msg_ramDump_header1);
  //uart_puts_P(msg_ramDump_header2);

  // snprintf(buffer,sizeof(buffer),"%04X",(uint16_t)&__data_start);
  // uart_puts_P(msg_ramDump_data_header); uart_puts(buffer);
  // snprintf(buffer,sizeof(buffer),"%04X",(uint16_t)&__data_end);
  // uart_puts_P(msg_ramDump_addr_header); uart_puts(buffer);
  // snprintf(buffer,sizeof(buffer),"%4d",(uint16_t)((uint16_t)&__data_end-(uint16_t)&__data_start));
  // uart_puts_P(msg_ramDump_seperator); uart_puts(buffer); uart_puts_P(msg_ramDump_data_footer);

  // snprintf(buffer,sizeof(buffer),"%04X",(uint16_t)&__bss_start);
  // uart_puts_P(msg_ramDump_bss_header); uart_puts(buffer);
  // snprintf(buffer,sizeof(buffer),"%04X",(uint16_t)&__bss_end);
  // uart_puts_P(msg_ramDump_addr_header); uart_puts(buffer);
  // snprintf(buffer,sizeof(buffer),"%4d",(uint16_t)((uint16_t)&__bss_end-(uint16_t)&__bss_start));
  // uart_puts_P(msg_ramDump_seperator); uart_puts(buffer); uart_puts_P(msg_ramDump_bss_footer);

  // snprintf(buffer,sizeof(buffer),"%04X",(uint16_t)&__tdat_start);
  // uart_puts_P(msg_ramDump_tdat_header); uart_puts(buffer);
  // snprintf(buffer,sizeof(buffer),"%04X",(uint16_t)&__tdat_end);
  // uart_puts_P(msg_ramDump_addr_header); uart_puts(buffer);
  // snprintf(buffer,sizeof(buffer),"%4d",(uint16_t)((uint16_t)&__tdat_end-(uint16_t)&__tdat_start));
  // uart_puts_P(msg_ramDump_seperator); uart_puts(buffer); uart_puts_P(msg_ramDump_tdat_footer);  

  // snprintf(buffer,sizeof(buffer),"%04X",(uint16_t)&__heap_start);
  // uart_puts_P(msg_ramDump_heap_header); uart_puts(buffer);
  // snprintf(buffer,sizeof(buffer),"%04X",(uint16_t)__heap_end);
  // uart_puts_P(msg_ramDump_addr_header); uart_puts(buffer);
  // snprintf(buffer,sizeof(buffer),"%4d",__heap_size);
  // uart_puts_P(msg_ramDump_seperator); uart_puts(buffer); uart_puts_P(msg_ramDump_heap_footer);

  // snprintf(buffer,sizeof(buffer),"%04X",(uint16_t)__stack_ptr);
  // uart_puts_P(msg_ramDump_stack_header); uart_puts(buffer);
  // snprintf(buffer,sizeof(buffer),"%04X",RAMEND);
  // uart_puts_P(msg_ramDump_addr_header); uart_puts(buffer);
  // snprintf(buffer,sizeof(buffer),"%4d",__stack_size);
  // uart_puts_P(msg_ramDump_seperator); uart_puts(buffer); uart_puts_P(msg_ramDump_stack_footer);

  // uart_puts_P(msg_ramDump_free_header);
  // snprintf(buffer,sizeof(buffer),"%4d",ram_total_free);
  // uart_puts(buffer);
  // uart_puts_P(msg_ramDump_free_footer);

  // uart_puts_P(msg_ramDump_header1);
}
void taskSerialSystemDebugger(void*pvParameters){
  char buffer[configMAX_TASK_NAME_LEN+1]={0};
  byte i=0;
  uint8_t nameLength{};
  TaskStatus_t taskStatus{};
  for(;;){
      //uart_putc('\n');
      //uart_puts_P(msg_serialSystemDebugger_header1);
      //uart_puts_P(msg_serialSystemDebugger_header2);
      for(i=0;i<TASK_N;i++){
        memset((void*)buffer,0,sizeof(buffer));
        if(taskHandles[i]==nullptr)
          continue;
        vTaskGetInfo(taskHandles[i],&taskStatus,pdTRUE,eInvalid);
        sprintf(buffer,"%s",taskStatus.pcTaskName);
        nameLength=strlen(taskStatus.pcTaskName);
        memset(buffer+nameLength,' ',sizeof(buffer)-nameLength);
        buffer[configMAX_TASK_NAME_LEN-1]='\0';
        //uart_putc('['); uart_puts(buffer); uart_putc(']');
        sprintf(buffer,"%5d",taskStatus.usStackHighWaterMark);
        //uart_puts(buffer); uart_putc('|');
        switch(taskStatus.eCurrentState){
          case eReady:
            //uart_puts_P(msg_serialSystemDebugger_taskReady);
          break;
          case eRunning:
           // uart_puts_P(msg_serialSystemDebugger_taskRunning);
          break;
          case eBlocked:
           // uart_puts_P(msg_serialSystemDebugger_taskBlocked);
            break;
          case eSuspended:
           // uart_puts_P(msg_serialSystemDebugger_taskSuspended);
          break;
          case eDeleted:
            //uart_puts_P(msg_serialSystemDebugger_taskDeleted);
            break;
          default:
            //uart_puts_P(msg_serialSystemDebugger_taskInvalid);
        1;
          }
        //uart_puts("| "); uart_putc(taskStatus.uxCurrentPriority+48);
        sprintf(buffer,"%2d",i);
        //uart_putc('['); uart_puts(buffer); uart_puts("]\n");
      }
    //uart_puts_P(msg_serialSystemDebugger_footer);
	  //uart_putc('\n');

    updateMemoryUsage();
	  ram_dump();
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}
