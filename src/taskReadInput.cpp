#include <stdio.h>
#include <avr/io.h>

#include <taskReadInput.h>

#include <uart.h>
#include <i2c.h>
#include <board_io.h>

#include <DrinkCreator6000_Pins.h>
#include <DrinkCreator6000_Config.h>


void taskReadInput(void*pvParameters){
    uint8_t keyboardInput=0; // Input read from INTCAPA
    uint8_t currentKeyboardInput=0; // Input read form GPUA
    const uint8_t MAX_TIMEOUT=25;
    uint8_t timeout=0;

  
    for(;;){
        // Handling interrupt with debounce
        if(read_keyboard_int()==false){
            if(xSemaphoreTake(mux_I2CLock,pdMS_TO_TICKS(100))==pdTRUE){
                mcp.read_byte_blocking(MCP_INTCAP);
                keyboardInput=mcp.read_byte_blocking(MCP_INTCAP);
                
                xSemaphoreGive(mux_I2CLock);

                xQueueSend(qKeyboardData,&keyboardInput,pdMS_TO_TICKS(50));
                keyboardInput=0;
                // Up to this point I'm handling the first interrupt which occurred
                // Which is correct as I'm processing INTCAP
                // I also need to handle debounce in this section, along with clearing int flag
            }

            if(read_keyboard_int()==false){
                vTaskDelay(pdMS_TO_TICKS(100));
                // Checking if button is still pressed
                while(read_keyboard_int()==false){
                    vTaskDelay(pdMS_TO_TICKS(200));
                    if(xSemaphoreTake(mux_I2CLock,pdMS_TO_TICKS(100))==pdTRUE){
                        currentKeyboardInput=mcp.read_byte_blocking(MCP_GPIO);
                        xSemaphoreGive(mux_I2CLock);

                        if(currentKeyboardInput!=keyboardInput){
                            break;
                        }
                        xQueueSend(qKeyboardData,&keyboardInput,pdMS_TO_TICKS(50));
                    }
                }
                vTaskDelay(pdMS_TO_TICKS(350));
                if(xSemaphoreTake(mux_I2CLock,pdMS_TO_TICKS(100)==pdTRUE)){
                    mcp.read_byte_blocking(MCP_INTCAP);
                    xSemaphoreGive(mux_I2CLock);
                }        
            }
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
