#include "taskReadTemp.h"

void taskReadTemp(void*pvParameters){
  for(;;){
    vTaskDelay(pdMS_TO_TICKS(TASK_READ_TEMP_REFRESH_RATE));
  }
}

