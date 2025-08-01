#include "taskSerialInput.h"

void taskSerialInput(void*pvParameters){
  uint8_t keyboardData=0;
  // Added for testing as PCF8574 is a bad choice for a keyboard driver
  // will switch to MCP23017
  for(;;){
    while(!Serial.available()){
      vTaskDelay(pdMS_TO_TICKS(100));
    }
    if(Serial.peek()==10)
      Serial.read();
    if(Serial.available()){
      keyboardData=(1<<(Serial.read()-49)); // We need the test data to be powers of 2
      xQueueSend(qKeyboardData,&keyboardData,pdMS_TO_TICKS(50));
    }
  }
}

