#include <stdio.h>
#include <string.h>

#include <DrinkCreator6000_Config_C.h>
#include <timers.h>
#include <portable.h>
#include <portmacro.h>

#include <taskWelcomeScreen.h>

void taskWelcomeScreen(void*pvParameters){
  uint16_t timePassed=0;
  uint8_t keyboardData=E_GREEN_BUTTON;
  uint32_t notification=0;
  
  sScreenData screenData{};  
  sprintf(screenData.lines[0],"%s","Drink Creator 6000");
  sprintf(screenData.lines[1],"%s","Initializing...");
  sprintf(screenData.lines[2],"%12s %2d %%","[----------]",(100*timePassed)/TASK_WELCOME_TICKS_TO_CLOSE);
  sprintf(screenData.lines[3],"%s","Please wait");
  
  for(;;){ 
	if(timePassed>=TASK_WELCOME_TICKS_TO_CLOSE+1){
    vTaskDelay(pdMS_TO_TICKS(TASK_WELCOME_SCREEN_REFRESH_RATE));
    xQueueSend(qKeyboardData,&keyboardData,pdMS_TO_TICKS(50));
	  vTaskDelete(NULL);
	}
	if(xTaskNotifyWait(0,0,&notification,0)>0){
    if(notification==0)
	   vTaskDelete(NULL);
	}
	memset((void*)(screenData.lines[2]+1),0xFF,10*timePassed/TASK_WELCOME_TICKS_TO_CLOSE);
	sprintf(screenData.lines[2]+13,"%3u %%",100*timePassed/TASK_WELCOME_TICKS_TO_CLOSE);
  
	timePassed++;
 
	xQueueSend(qScreenData,&screenData,pdMS_TO_TICKS(50));
	
  vTaskDelay(pdMS_TO_TICKS(TASK_WELCOME_SCREEN_REFRESH_RATE));
  }
}
