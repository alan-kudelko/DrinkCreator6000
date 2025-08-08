#include "taskUpdateScreen.h"

void taskUpdateScreen(void*pvParameters){
  sScreenData receivedLcdData{};

  TickType_t xLastWakeTime=xTaskGetTickCount();
  uint8_t i=0;
  uint8_t j=0;
  
  for(;;){
    if(xQueueReceive(qScreenData,(void*)&receivedLcdData,pdMS_TO_TICKS(0))==pdPASS){
      if(xSemaphoreTake(mux_I2CLock,pdMS_TO_TICKS(0))==pdPASS){
        for(i=0;i<LCD_HEIGHT;i++){
        //   lcd.setCursor(0,i);
        //   for(j=0;j<LCD_WIDTH;j++)
        //   if(receivedLcdData.lines[i][j]==0)
        //     lcd.write(' ');
        //   else if(receivedLcdData.lines[i][j]==E_LOADING_BAR)
        //     lcd.write(0);
        //   else if(receivedLcdData.lines[i][j]==18)
        //     lcd.write(1);
        //   else if(receivedLcdData.lines[i][j]==19)
        //     lcd.write(2);
        //   else if(receivedLcdData.lines[i][j]==20)
        //     lcd.write(3);
        //   else if(receivedLcdData.lines[i][j]==21)
        //     lcd.write(4);
        //   else if(receivedLcdData.lines[i][j]==22)
        //     lcd.write(5);
        //   else if(receivedLcdData.lines[i][j]==23)
        //     lcd.write(6);
        //   else if(receivedLcdData.lines[i][j]==24)
        //     lcd.write(7);
        //   else
        //     lcd.write(receivedLcdData.lines[i][j]);
        }
        // lcd.setCursor(receivedLcdData.lcdCursorX,receivedLcdData.lcdCursorY);
        // if(receivedLcdData.lcdCursorBlink)
        //   lcd.blink();
        // else
        //   lcd.noBlink();
        xSemaphoreGive(mux_I2CLock);
      }
    }
    vTaskDelayUntil(&xLastWakeTime,pdMS_TO_TICKS(TASK_UPDATE_SCREEN_REFRESH_RATE)); 
  }
}

