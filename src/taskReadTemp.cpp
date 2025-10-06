#include <taskReadTemp.h>
#include <DrinkCreator6000_Config_C.h>

void taskReadTemp(void*pvParameters){
    for(;;){
        vTaskDelay(pdMS_TO_TICKS(TASK_READ_TEMP_REFRESH_RATE));
    }
}
