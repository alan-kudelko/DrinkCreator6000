#include "taskErrorHandler.h"
#include <avr/wdt.h>  

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
  uint32_t runTimeFromMillis=0;
  runTimeFromMillis=millis()/1000;
  lastError->days=runTimeFromMillis/3600/24;
  lastError->hours=runTimeFromMillis/3600%24;
  lastError->minutes=runTimeFromMillis/60%60;
  lastError->seconds=runTimeFromMillis%60;
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
  for(;i<GUARD_ZONE_SIZE;i++){
    Serial.print("0X");
    Serial.print(*((uint8_t*)(guardZones[*guardZoneId]+i)),HEX);
    Serial.print(' ');
  }
  Serial.println("");
}
void taskErrorHandler(void*pvParameters){
  uint8_t i=1;
  uint32_t runTimeFromMillis=0;
  uint8_t guardZoneId=0;
  sSystemError lastError{};
  bool f_errorOccured=false;
  TaskHandle_t overflowedTask{};
  
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
       
      Serial.println(F("[####################]====SYSTEM CRITICAL====[##]"));
      Serial.println((const char*)lastError.errorText);
      Serial.println(F("[####################]====SYSTEM CRITICAL====[##]"));
      Serial.println("");
      
      for(i=0;i<5;i++){
        vTaskDelay(pdMS_TO_TICKS(1000));
      }
      // reset MCU
    }
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}
