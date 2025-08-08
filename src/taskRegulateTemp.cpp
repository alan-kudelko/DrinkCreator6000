#include "taskRegulateTemp.h"

void taskRegulateTemp(void*pvParameters){
  for(;;){
    if(currentTemperature>=setTemperature+temperatureHysteresis){
      //digitalWrite(Pelt1Pin,HIGH); 
      //digitalWrite(Pelt2Pin,HIGH);
    }
    else if(currentTemperature<=setTemperature-temperatureHysteresis){
      //digitalWrite(Pelt1Pin,LOW);
      //digitalWrite(Pelt2Pin,LOW);
    }
    vTaskDelay(pdMS_TO_TICKS(TASK_REGULATE_TEMP_REFRESH_RATE));
  }
}

