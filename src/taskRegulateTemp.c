#include <taskRegulateTemp.h>
#include <DrinkCreator6000_Config_C.h>

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
        //PORTD|=(1<<PD4|1<<PD5);
        vTaskDelay(pdMS_TO_TICKS(TASK_REGULATE_TEMP_REFRESH_RATE));    
    }
}
