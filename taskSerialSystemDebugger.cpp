#include "taskSerialSystemDebugger.h"

void updateMemoryUsage(){
  heap_end=__brkval?__brkval:(void*)&__heap_start;
  heap_size=(uint16_t)heap_end-(uint16_t)&__heap_start;
  stack_size=(uint16_t)RAMEND-(uint16_t)stack_ptr;
  
  ram_total_free=(uint16_t)stack_ptr-(uint16_t)heap_end;
  ram_in_use=uint16_t(ram_size)-uint16_t(ram_total_free);
}
void ram_dump(){
  char buffer[6]{};
  updateMemoryUsage();
  
  Serial.println(F("[#####]====[MEMORY STATUS]====[#####]"));
  Serial.println(F("[     ] START |  END  | SIZE  [     ]"));

  sprintf(buffer,"%04X",(uint16_t)&__tdat_start);
  Serial.print(F("[.TDAT]0x")); Serial.print(buffer);
  sprintf(buffer,"%04X",(uint16_t)&__tdat_end);
  Serial.print(F("|0x")); Serial.print(buffer);
  sprintf(buffer,"%4d",(uint16_t)((uint16_t)&__tdat_end-(uint16_t)&__tdat_start));
  Serial.print(F("|")); Serial.print(buffer); Serial.println(F(" B[.TDAT]"));

  sprintf(buffer,"%04X",(uint16_t)&__data_start);
  Serial.print(F("[.DATA]0x")); Serial.print(buffer);
  sprintf(buffer,"%04X",(uint16_t)&__data_end);
  Serial.print(F("|0x")); Serial.print(buffer);
  sprintf(buffer,"%4d",(uint16_t)((uint16_t)&__data_end-(uint16_t)&__data_start));
  Serial.print(F("|")); Serial.print(buffer); Serial.println(F(" B[.DATA]"));

  sprintf(buffer,"%04X",(uint16_t)&__bss_start);
  Serial.print(F("[.BSS ]0x")); Serial.print(buffer);
  sprintf(buffer,"%04X",(uint16_t)&__bss_end);
  Serial.print(F("|0x")); Serial.print(buffer);
  sprintf(buffer,"%4d",(uint16_t)((uint16_t)&__bss_end-(uint16_t)&__bss_start));
  Serial.print(F("|")); Serial.print(buffer); Serial.println(F(" B[.BSS ]"));

  sprintf(buffer,"%04X",(uint16_t)&__heap_start);
  Serial.print(F("[HEAP ]0x")); Serial.print(buffer);
  sprintf(buffer,"%04X",(uint16_t)heap_end);
  Serial.print(F("|0x")); Serial.print(buffer);
  sprintf(buffer,"%4d",heap_size);
  Serial.print(F("|")); Serial.print(buffer); Serial.println(F(" B[HEAP ]"));

  sprintf(buffer,"%04X",(uint16_t)stack_ptr);
  Serial.print(F("[STACK]0x")); Serial.print(buffer);
  sprintf(buffer,"%04X",RAMEND);
  Serial.print(F("|0x")); Serial.print(buffer);
  sprintf(buffer,"%4d",stack_size);
  Serial.print(F("|")); Serial.print(buffer); Serial.println(F(" B[STACK]"));

  Serial.print(F("[FREE ]-------------|"));
  sprintf(buffer, "%4d",ram_total_free);
  Serial.print(buffer);
  Serial.println(F(" B[FREE ]"));

  Serial.println(F("[#####]====[MEMORY STATUS]====[#####]"));
}
void taskSerialSystemDebugger(void*pvParameters){
  char buffer[21]{};
  byte i=0;
  uint8_t nameLength{};
  TaskStatus_t taskStatus{};
  for(;;){
      Serial.println();
      Serial.println(F("[####################]====TASK STATUS===[##]"));
      Serial.println(F("[     TASK NAME      ]STACK|  STATE  |PR[ID]"));
      for(i=0;i<TASK_N;i++){
        memset(buffer,0,sizeof(buffer));
        if(taskHandles[i]==nullptr)
          continue;
        vTaskGetInfo(taskHandles[i],&taskStatus,pdTRUE,eInvalid);
        sprintf(buffer,"%s",taskStatus.pcTaskName);
        nameLength=strlen(taskStatus.pcTaskName);
        memset(buffer+nameLength,' ',sizeof(buffer)-nameLength);
        buffer[20]='\0';
        Serial.print(F("[")); Serial.print(buffer); Serial.print(F("]"));
        sprintf(buffer,"%5d",taskStatus.uxStackHighWaterMark);
        Serial.print(buffer); Serial.print(F("|"));
        switch(taskStatus.eCurrentState){
          case eReady:
            Serial.print(F("Ready    "));
          break;
          case eRunning:
            Serial.print(F("Running  "));
          break;
          case eBlocked:
            Serial.print(F("Blocked  "));
            break;
          case eSuspended:
            Serial.print(F("Suspended"));
          break;
          case eDeleted:
            Serial.print(F("Deleted  "));
            break;
          default:
            Serial.print(F("Unknown  "));
        }
        Serial.print(F("| ")); Serial.print(taskStatus.uxCurrentPriority);
        sprintf(buffer,"%2d",i);
        Serial.print(F("[")); Serial.print(buffer); Serial.println(F("]"));
      }
    Serial.println(F("[####################]====TASK STATUS===[##]"));
	Serial.println();

    updateMemoryUsage();
	ram_dump();
    // Stack, heap etc
	/*
    Serial.print("stack_ptr: ");
    Serial.println(uint16_t(stack_ptr),HEX);

    Serial.print("heap_end: ");
    Serial.println(uint16_t(heap_end),HEX);

    
    Serial.print("stack_size: ");
    Serial.println(uint16_t(stack_size),HEX);
    
    Serial.print("heap_size: ");
    Serial.println(uint16_t(heap_size),HEX);

    Serial.print("ram_total_free: ");
    Serial.println(uint16_t(ram_total_free));
    
    Serial.print("ram_in_use: ");
    Serial.println(uint16_t(ram_in_use));

    Serial.print("ram_size: ");
    Serial.println(uint16_t(ram_size));

    Serial.print("__data_start: ");
    Serial.println(uint16_t(&__data_start),HEX);

    Serial.print("__data_end: ");
    Serial.println(uint16_t(&__data_end),HEX);
	*/
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}
