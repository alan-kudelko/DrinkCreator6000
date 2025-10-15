#include <taskRegulateTemp.h>
#include <DrinkCreator6000_Config_C.h>

void taskRegulateTemp(void*pvParameters){
    uint32_t f_run=0;

    struct sHardwareData hardwareData={HARDWARE_COOLER|HARDWARE_CIRCULATION|HARDWARE_FANS,0};
    for(;;){
        if(xTaskNotifyWait(0,0,&f_run,0)){
            if(f_run==0){
                hardwareData.mask=0;
                xQueueSend(qHardwareControl,&hardwareData,pdMS_TO_TICKS(100));
            }
        }
        if(f_run==1){
            if(currentTemperature>=setTemperature+temperatureHysteresis){
            //digitalWrite(Pelt1Pin,HIGH); 
            //digitalWrite(Pelt2Pin,HIGH);
            }
            else if(currentTemperature<=setTemperature-temperatureHysteresis){
            //digitalWrite(Pelt1Pin,LOW);
            //digitalWrite(Pelt2Pin,LOW);
            }
            // For now
            hardwareData.mask=HARDWARE_MISC_BIT;
            xQueueSend(qHardwareControl,&hardwareData,pdMS_TO_TICKS(100));
            //PORTD|=(1<<PD4|1<<PD5);
            vTaskDelay(pdMS_TO_TICKS(TASK_REGULATE_TEMP_REFRESH_RATE));
        }
        else{
            vTaskDelay(pdMS_TO_TICKS(TASK_REGULATE_TEMP_REFRESH_RATE*2));
        }
    }
}
