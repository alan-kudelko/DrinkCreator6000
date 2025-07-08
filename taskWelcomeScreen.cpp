#include "taskWelcomeScreen.h"

void taskWelcomeScreen(void*pvParameters){
  enum{TASK_WELCOME_TICKS_TO_CLOSE=10};
  
  uint8_t timePassed=0;
  uint8_t keyboardData=E_GREEN_BUTTON;
  bool f_run=false;
  
  sScreenData screenData{};  
  sprintf(screenData.lines[0],"%s","Drink Creator 6000");
  sprintf(screenData.lines[1],"%s","Initializing...");
  sprintf(screenData.lines[2],"%12s","[----------]",100*timePassed/TASK_WELCOME_TICKS_TO_CLOSE);
  sprintf(screenData.lines[3],"%s","Please wait");
  
  
  for(;;){
	timePassed++;  
	if(timePassed>=TASK_WELCOME_TICKS_TO_CLOSE){
	  xQueueSend(qKeyboardData,&keyboardData,pdMS_TO_TICKS(50));
	  vTaskDelete(NULL);
	}
	if(ulTaskNotifyTake(pdTRUE,0)>0){
	  xQueueSend(qKeyboardData,&keyboardData,pdMS_TO_TICKS(50));
	  vTaskDelete(NULL);
	}
	memset(screenData.lines[2]+1,0xFF,100*timePassed/TASK_WELCOME_TICKS_TO_CLOSE);
	sprintf(screenData.lines[2]+13,"%3u %%",100*timePassed/TASK_WELCOME_TICKS_TO_CLOSE);
	
	xQueueSend(qScreenData,&screenData,pdMS_TO_TICKS(50));
	
    vTaskDelay(pdMS_TO_TICKS(TASK_WELCOME_SCREEN_REFRESH_RATE));
  }
}
