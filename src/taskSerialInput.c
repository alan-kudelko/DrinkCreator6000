#include <taskSerialInput.h>
#include <uart.h>
#include <DrinkCreator6000_Config.h>

void taskSerialInput(void*pvParameters){
  uint8_t keyboardData=0;
  // Added for testing as PCF8574 is a bad choice for a keyboard driver
  // will switch to MCP23017
  for(;;){
    while(uart_getc()!=-1){
      vTaskDelay(pdMS_TO_TICKS(100));
    }
    if(uart_peekc()==10)
      uart_getc(); // Clear the newline character from the buffer
    if(uart_peekc()){
      keyboardData=(1<<(uart_getc()-49)); // We need the test data to be powers of 2
      xQueueSend(qKeyboardData,&keyboardData,pdMS_TO_TICKS(50));
    }
  }
}

