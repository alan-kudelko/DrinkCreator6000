#include "taskErrorHandler.h"

void EEPROMUpdateLastStartupError(sSystemError*errorStruct);

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
    for(j=0;j<GUARD_ZONE_SIZE;j++){
      if(*(char*)(guardZones[i]+j)!=MEMORY_FILL_PATTERN){
        *guardZoneId=i;
        break;
      }
    }
  }
}
void taskErrorHandler(void*pvParameters){
  uint8_t i=1;
  uint32_t runTimeFromMillis=0;
  uint8_t guardZoneId=0;
  sSystemError lastError{};
  bool f_errorOccured=false;
  TaskHandle_t overflowedTask{};
  
  for(;;){
	// add mechanism for checking for guard zones corruption
	// array of pointer to guard zones, to check this in the loop
    if(!f_errorOccured){
      if(xQueueReceive(qErrorId,&overflowedTask,pdMS_TO_TICKS(50))==pdPASS){
        f_errorOccured=true;
    
        for(;i<TASK_N;i++)
          vTaskSuspend(taskHandles[i]);    
  
        sprintf(lastError.errorText,"Stack overflow in task: %s",pcTaskGetName(overflowedTask));  
      }
      //checkGuardZones(&guardZoneId); // Needs fixing
      if(guardZoneId){
        f_errorOccured=true;
        sprintf(lastError.errorText,"Guard zone corrupted in task: %d",guardZoneId);
      }
    }
    if(f_errorOccured){
	    // activate speaker
	    // Delay and systems restart
	    // If error occured, all devices should be safely shut down
     
      //stopAllDevices(); //pumps stop, peltier stop etc.
      for(i=2;i<TASK_N;i++)
          vTaskSuspend(taskHandles[i]);  
      
      generateErrorInfo(&lastError);        
      EEPROMUpdateLastStartupError(&lastError);
       
      Serial.println(F("[####################]====SYSTEM CRITICAL====[##]"));
      Serial.println((const char*)lastError.errorText);
      Serial.println(F("[####################]====SYSTEM CRITICAL====[##]"));
      Serial.println("");
        
      for(i=0;i<100;i++){
        // Buzzer
        vTaskDelay(pdMS_TO_TICKS(5000));
      }
      // reset MCU
    }
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}
