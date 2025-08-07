#include "taskReadInput.h"
#include <uart.h>

void taskReadInput(void*pvParameters){
  uint8_t keyboardInput=0; // Input read from INTCAPA
  uint8_t currentKeyboardInput=0; // Input read form GPUA
  
  for(;;){
    // Handling interrupt with debounce
    if(digitalRead(INTPin)==LOW){
      if(xSemaphoreTake(mux_I2CLock,pdMS_TO_TICKS(portMAX_DELAY))==pdTRUE){
        Wire.beginTransmission(MCP_ADDR);
        Wire.write(0x10);
        Wire.endTransmission();
        Wire.requestFrom(MCP_ADDR,1);
        
        keyboardInput=Wire.read()^0b11111111;
                
        xSemaphoreGive(mux_I2CLock);
        
        //uart_puts("Dane klawa: ");
        char buffer[6];
        snprintf(buffer,sizeof(buffer),"0x%3X",keyboardInput);
        //uart_puts(buffer);
        //uart_putc('\n');

        xQueueSend(qKeyboardData,&keyboardInput,pdMS_TO_TICKS(50));
        // Up to this point I'm handling the first interrupt which occurred
        // Which is correct as I'm processing INTCAP
        // I also need to handle debounce in this section, along with clearing int flag
      }
      if(digitalRead(INTPin)==LOW){
        vTaskDelay(pdMS_TO_TICKS(100));
        // Checking if button is still pressed
        while(digitalRead(INTPin)==LOW){
          vTaskDelay(pdMS_TO_TICKS(400));
          if(xSemaphoreTake(mux_I2CLock,pdMS_TO_TICKS(portMAX_DELAY))==pdTRUE){
            Wire.beginTransmission(MCP_ADDR);
            Wire.write(0x12);
            Wire.endTransmission();
            Wire.requestFrom(MCP_ADDR,1);
            currentKeyboardInput=Wire.read();
            xSemaphoreGive(mux_I2CLock);

            if(currentKeyboardInput!=keyboardInput)
              break;
            xQueueSend(qKeyboardData,&keyboardInput,pdMS_TO_TICKS(50));
          }
        }
        vTaskDelay(pdMS_TO_TICKS(100));
        // Clearing interrupt PIN in MCP
        if(xSemaphoreTake(mux_I2CLock,pdMS_TO_TICKS(portMAX_DELAY)==pdTRUE)){
          Wire.beginTransmission(MCP_ADDR);
          Wire.write(0x10);
          Wire.endTransmission();
          Wire.requestFrom(MCP_ADDR,1);
          Wire.read();
          xSemaphoreGive(mux_I2CLock);
        }        
      }
    }
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}
