#include "taskErrorHandler.h"
#include <avr/wdt.h>  
#include <uart.h>

void EEPROMUpdateLastStartupError(sSystemError*errorStruct);

void stopPumps(){
  digitalWrite(STPin,LOW);
  shiftOut(DSPin,SHPin,0,LSBFIRST);
  digitalWrite(STPin,HIGH);  
}
void stopCooler(){
  digitalWrite(Pelt1Pin,LOW);
  digitalWrite(Pelt1Pin,LOW);
  digitalWrite(FansPin,LOW);
}
// This won't work for a while, I need ISP programmer to set High Fuse bits
// See Atmega2560 datasheet
void softwareReset(){
  wdt_enable(WDTO_15MS);
  while(true);
}
void generateErrorInfo(sSystemError*lastError){
  uint32_t currentRunTimeMS=0;
  currentRunTimeMS=xTaskGetTickCount()*portTICK_PERIOD_MS;
  lastError->days=currentRunTimeMS/3600/24;
  lastError->hours=currentRunTimeMS/3600%24;
  lastError->minutes=currentRunTimeMS/60%60;
  lastError->seconds=currentRunTimeMS%60;
  lastError->confirmed=false;
}
void checkGuardZones(uint8_t*guardZoneId){
  uint8_t i=0;
  uint8_t j=0;
  for(;i<TASK_N;i++){
    for(;j<GUARD_ZONE_SIZE;j++){
      if(*((uint8_t*)(guardZones[i]+j))!=MEMORY_FILL_PATTERN){
        *guardZoneId=i;
        break;
      }
    }
  }
}
void displayCorruptedGuardZone(uint8_t*guardZoneId){
  uint8_t i=0;
  char buffer[3]{};
  for(;i<GUARD_ZONE_SIZE;i++){
    uart_puts("0X");
    snprintf(buffer,sizeof(buffer),"%02X",*((uint8_t*)(guardZones[*guardZoneId]+i)));
    uart_puts(buffer);
    uart_putc(' ');
  }
  uart_putc('\n');
}
void taskErrorHandler(void*pvParameters){
  // Convert code to be MISRA C 2025 compliant
  uint8_t i=1;
  uint8_t guardZoneId=0;
  sSystemError lastError{};
  bool f_errorOccured=false;
  
  for(;;){
    if(!f_errorOccured){
      checkGuardZones(&guardZoneId);
      if(guardZoneId){
        for(;i<TASK_N;i++)
          vTaskSuspend(taskHandles[i]);            
        f_errorOccured=true;
        sprintf(lastError.errorText,"Guard zone %d corrupted in task: %s",guardZoneId,TaskNames[guardZoneId]);
      }
//      if(xQueueReceive(qErrorId,&overflowedTask,pdMS_TO_TICKS(50))==pdPASS){
//        f_errorOccured=true;
//        Serial.println("No cos przyszlo");
//        for(;i<TASK_N;i++)
//          vTaskSuspend(taskHandles[i]);
//      }
    }
    if(f_errorOccured){
	    // activate speaker
	    // Delay and systems restart
	    // If error occured, all devices should be safely shut down
     
      //stopAllDevices(); //pumps stop, peltier stop etc.
      for(i=2;i<TASK_N;i++)
          vTaskSuspend(taskHandles[i]);  

      stopCooler();
      stopPumps();
      
      displayCorruptedGuardZone(&guardZoneId);
      
      generateErrorInfo(&lastError);
      lastError.taskId=guardZoneId;        
      EEPROMUpdateLastStartupError(&lastError);
       
      uart_puts("[####################]====SYSTEM CRITICAL====[##]\n");
      uart_puts((const char*)lastError.errorText); uart_putc('\n');
      uart_puts("[####################]====SYSTEM CRITICAL====[##]\n");
      uart_putc('\n');
      
      for(i=0;i<5;i++){
        vTaskDelay(pdMS_TO_TICKS(1000));
      }
      // reset MCU
    }
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}
