#include <taskUpdateScreen.h>
#include <DrinkCreator6000_Config.h>
#include <HD44780_I2C.h>

extern HD44780_LCD lcd;


void taskUpdateScreen(void*pvParameters){
  sScreenData receivedLcdData{};

  TickType_t xLastWakeTime=xTaskGetTickCount();
  uint8_t i=0;
  uint8_t j=0;
  
  for(;;){
    if(xQueueReceive(qScreenData,(void*)&receivedLcdData,pdMS_TO_TICKS(0))==pdPASS){
      if(xSemaphoreTake(mux_I2CLock,pdMS_TO_TICKS(0))==pdPASS){
        for(i=0;i<LCD_HEIGHT;i++){
           lcd.setCursor_blocking(0,i);
           for(j=0;j<LCD_WIDTH;j++){
           if(receivedLcdData.lines[i][j]==0)
             lcd.write_blocking(' ');
           else if(receivedLcdData.lines[i][j]==E_LOADING_BAR)
             lcd.write_blocking(0);
           else if(receivedLcdData.lines[i][j]==18)
             lcd.write_blocking(1);
           else if(receivedLcdData.lines[i][j]==19)
             lcd.write_blocking(2);
           else if(receivedLcdData.lines[i][j]==20)
             lcd.write_blocking(3);
           else if(receivedLcdData.lines[i][j]==21)
             lcd.write_blocking(4);
           else if(receivedLcdData.lines[i][j]==22)
             lcd.write_blocking(5);
           else if(receivedLcdData.lines[i][j]==23)
             lcd.write_blocking(6);
           else if(receivedLcdData.lines[i][j]==24)
             lcd.write_blocking(7);
           else
             lcd.write_blocking(receivedLcdData.lines[i][j]);
           }
        }
         //lcd.setCursor_blocking(receivedLcdData.lcdCursorX,receivedLcdData.lcdCursorY);
         //if(receivedLcdData.lcdCursorBlink)
         //  lcd.blink_blocking();
         //else
         //  lcd.noBlink_blocking();
        xSemaphoreGive(mux_I2CLock);
      }
    }
    vTaskDelayUntil(&xLastWakeTime,pdMS_TO_TICKS(TASK_UPDATE_SCREEN_REFRESH_RATE)); 
  }
}

