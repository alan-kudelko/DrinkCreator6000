#include "taskSelectDrink.h"

void taskSelectDrink(void*pvParameters){  
  uint32_t f_run=0;
  
  uint8_t currentScroll=0;
  uint8_t i=0;
  uint8_t firstNonZero=0;
  uint8_t lastNonZero=6; //-2
  sScreenData screenData{};
  
  // Should use UI_Context
  
  for(;;){
    if(xTaskNotifyWait(0,0,&f_run,0)>0){
	    if(f_run==1){
        currentScroll=0;
        firstNonZero=0;
	  
        for(i=0;(i<8)&&(!drink[UI_Context.currentSubMenu].ingredients[i]);i++);
      
        firstNonZero=i;
        for(i=7;(i>=0)&&(!drink[UI_Context.currentSubMenu].ingredients[i]);i--);

        lastNonZero=i-1;
        memset(&screenData,0,sizeof(screenData));
        sprintf(screenData.lines[0],"[%2d]%s",UI_Context.currentSubMenu+1,drink[UI_Context.currentSubMenu].drinkName);		  
	    }
    }
    if(f_run==1){
      //Other scrollable lines
      if(currentScroll>=lastNonZero)
        currentScroll=firstNonZero;
      
      for(i=0;i<3;i++){
        while((currentScroll<lastNonZero)&&(!drink[UI_Context.currentSubMenu].ingredients[i+currentScroll])){
          currentScroll++;
        }
        if(currentScroll>=lastNonZero){
          currentScroll=firstNonZero;
        }
        else{
          memset(screenData.lines[1+i],0,sizeof(screenData.lines[0]));
          sprintf(screenData.lines[1+i],"%s",ingredients[i+currentScroll]);
          sprintf(screenData.lines[1+i]+13,"%3d[ml]",drink[UI_Context.currentSubMenu].ingredients[i+currentScroll]);
        }
      }
      currentScroll++;
      xQueueSend(qScreenData, &screenData, pdMS_TO_TICKS(50));
      //Other scrollable lines
      vTaskDelay(pdMS_TO_TICKS(1000));
    }
    else{
      vTaskDelay(pdMS_TO_TICKS(500));
    }
  }
}
