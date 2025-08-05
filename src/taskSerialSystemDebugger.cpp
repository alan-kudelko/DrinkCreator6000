#include <taskSerialSystemDebugger.h>
#include <uart.h>

void updateMemoryUsage(){
  __heap_end=__brkval?__brkval:(void*)&__heap_start;
  __heap_size=(uint16_t)__heap_end-(uint16_t)&__heap_start;
  __stack_size=(uint16_t)RAMEND-(uint16_t)__stack_ptr;
  __tdat_size=(uint16_t)&__tdat_end-(uint16_t)&__tdat_start;
  
  ram_total_free=(uint16_t)__stack_ptr-(uint16_t)__heap_end;
  ram_in_use=(uint16_t)ram_size-(uint16_t)ram_total_free;
}
void ram_dump(){
  // Convert code to be MISRA C 2025 compliant
  char buffer[6]{};
  updateMemoryUsage();
  
  uart_puts("[#####]====[MEMORY STATUS]====[#####]\n");
  uart_puts("[     ] START |  END  | SIZE  [     ]\n");

  sprintf(buffer,"%04X",(uint16_t)&__data_start);
  uart_puts("[.DATA] 0x"); uart_puts(buffer);
  sprintf(buffer,"%04X",(uint16_t)&__data_end);
  uart_puts("| 0x"); uart_puts(buffer);
  sprintf(buffer,"%4d",(uint16_t)((uint16_t)&__data_end-(uint16_t)&__data_start));
  uart_puts("| "); uart_puts(buffer); uart_puts(" B[.DATA]\n");

  sprintf(buffer,"%04X",(uint16_t)&__bss_start);
  uart_puts("[.BSS ] 0x"); uart_puts(buffer);
  sprintf(buffer,"%04X",(uint16_t)&__bss_end);
  uart_puts("| 0x"); uart_puts(buffer);
  sprintf(buffer,"%4d",(uint16_t)((uint16_t)&__bss_end-(uint16_t)&__bss_start));
  uart_puts("| "); uart_puts(buffer); uart_puts(" B[.BSS ]\n");

  sprintf(buffer,"%04X",(uint16_t)&__tdat_start);
  uart_puts("[.TDAT] 0x"); uart_puts(buffer);
  sprintf(buffer,"%04X",(uint16_t)&__tdat_end);
  uart_puts("| 0x"); uart_puts(buffer);
  sprintf(buffer,"%4d",(uint16_t)((uint16_t)&__tdat_end-(uint16_t)&__tdat_start));
  uart_puts("| "); uart_puts(buffer); uart_puts(" B[.TDAT]\n");  

  sprintf(buffer,"%04X",(uint16_t)&__heap_start);
  uart_puts("[HEAP ] 0x"); uart_puts(buffer);
  sprintf(buffer,"%04X",(uint16_t)__heap_end);
  uart_puts("| 0x"); uart_puts(buffer);
  sprintf(buffer,"%4d",__heap_size);
  uart_puts("| "); uart_puts(buffer); uart_puts(" B[HEAP ]\n");

  sprintf(buffer,"%04X",(uint16_t)__stack_ptr);
  uart_puts("[STACK] 0x"); uart_puts(buffer);
  sprintf(buffer,"%04X",RAMEND);
  uart_puts("| 0x"); uart_puts(buffer);
  sprintf(buffer,"%4d",__stack_size);
  uart_puts("| "); uart_puts(buffer); uart_puts(" B[STACK]\n");

  uart_puts("[FREE ]---------------| ");
  sprintf(buffer, "%4d",ram_total_free);
  uart_puts(buffer);
  uart_puts(" B[FREE ]\n");

  uart_puts("[#####]====[MEMORY STATUS]====[#####]\n");
}
void taskSerialSystemDebugger(void*pvParameters){
  char buffer[21]{};
  byte i=0;
  uint8_t nameLength{};
  TaskStatus_t taskStatus{};
  for(;;){
      uart_putc('\n');
      uart_puts("[####################]====TASK STATUS===[##]\n");
      uart_puts("[     TASK NAME      ]STACK|  STATE  |PR[ID]\n");
      for(i=0;i<TASK_N;i++){
        memset(buffer,0,sizeof(buffer));
        if(taskHandles[i]==nullptr)
          continue;
        vTaskGetInfo(taskHandles[i],&taskStatus,pdTRUE,eInvalid);
        sprintf(buffer,"%s",taskStatus.pcTaskName);
        nameLength=strlen(taskStatus.pcTaskName);
        memset(buffer+nameLength,' ',sizeof(buffer)-nameLength);
        buffer[20]='\0';
        uart_putc('['); uart_puts(buffer); uart_putc(']');
        sprintf(buffer,"%5d",taskStatus.usStackHighWaterMark);
        uart_puts(buffer); uart_putc('|');
        switch(taskStatus.eCurrentState){
          case eReady:
            uart_puts("Ready    ");
          break;
          case eRunning:
            uart_puts("Running  ");
          break;
          case eBlocked:
            uart_puts("Blocked  ");
            break;
          case eSuspended:
            uart_puts("Suspended");
          break;
          case eDeleted:
            uart_puts("Deleted  ");
            break;
          default:
            uart_puts("Unknown  ");
        }
        uart_puts("| "); uart_putc(taskStatus.uxCurrentPriority);
        sprintf(buffer,"%2d",i);
        uart_putc('['); uart_puts(buffer); uart_puts("]\n");
      }
    uart_puts("[####################]====TASK STATUS===[##]\n");
	  uart_putc('\n');

    updateMemoryUsage();
	  ram_dump();
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}
