#include "taskReadInput.h"

void taskReadInput(void*pvParameters){
  uint8_t keyboardInput=0;
  //TickType_t xLastWakeTime=xTaskGetTickCount();
  
  for(;;){
    if(xSemaphoreTake(sem_ReadData,pdMS_TO_TICKS(portMAX_DELAY))==pdTRUE){
      if(xSemaphoreTake(mux_I2CLock,pdMS_TO_TICKS(portMAX_DELAY))==pdTRUE){
        Wire.beginTransmission(MCP_ADDR);
        Wire.write(0x10);
        Wire.endTransmission();
        Wire.requestFrom(MCP_ADDR,1);
        
        keyboardInput=Wire.read()^0b11111111;
                
        xSemaphoreGive(mux_I2CLock);
        
        Serial.print("Dane klawa: ");
        Serial.println(keyboardInput,BIN);

        xQueueSend(qKeyboardData,&keyboardInput,pdMS_TO_TICKS(50));

        vTaskDelay(pdMS_TO_TICKS(300));
        if(xSemaphoreTake(mux_I2CLock,pdMS_TO_TICKS(portMAX_DELAY))==pdTRUE){
          Wire.beginTransmission(MCP_ADDR);
          Wire.write(0x10);
          Wire.endTransmission();
          Wire.requestFrom(MCP_ADDR,1);
          Wire.read();
          xSemaphoreGive(mux_I2CLock);
          f_enableISR=true;
        }
      }
    }
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}
