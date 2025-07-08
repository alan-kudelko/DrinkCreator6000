#include "taskErrorHandler.h"

void taskErrorHandler(void*pvParameters){
  uint8_t i=1;
  uint32_t runTimeFromMillis=0;
  sSystemError lastError{};
  bool f_run=false;
  TaskHandle_t overflowedTask{};
  
  for(;;){
	// add mechanism for checking for guard zones corruption
	// array of pointer to guard zones, to check this in the loop
    if(xQueueReceive(qErrorId,&overflowedTask,pdMS_TO_TICKS(50))==pdPASS){
      f_run=true;
	  
      for(;i<TASK_N;i++)
        vTaskSuspend(taskHandles[i]);    
	
      sprintf(lastError.errorText,"Stack overflow in task: %s",pcTaskGetName(overflowedTask));
      runTimeFromMillis=millis()/1000;
      lastError.days=runTimeFromMillis/3600/24;
      lastError.hours=runTimeFromMillis/3600%24;
      lastError.minutes=runTimeFromMillis/60%60;
      lastError.seconds=runTimeFromMillis%60;
      lastError.confirmed=false;
	  
	  Serial.println(F("[####################]====SYSTEM CRITICAL====[##]"));
      Serial.println((const char*)lastError.errorText);
      Serial.println(F("[####################]====SYSTEM CRITICAL====[##]"));
      Serial.println("");
	  
      //EEPROMUpdateLastStartupError(&lastError);
	  
	  //stopAllDevices();	  
    }
    if(f_run){
	  // activate speaker
	  // Delay and systems restart
	  // IF error occured, all devices should be safely shut down
    }
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}