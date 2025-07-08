#include "taskSelectDrink.h"

void taskSelectDrink(void*pvParameters){
  uint8_t drinkId=0;
  bool f_run=false;
  uint8_t currentScroll=0;
  uint8_t i=0;
  uint8_t firstNonZero=0;
  uint8_t lastNonZero=6; //-2
  sScreenData screenData{};
  for(;;){
    if(ulTaskNotifyTake(pdTRUE,0)>0){
      while(xQueueReceive(qDrinkId,&drinkId,pdMS_TO_TICKS(100))==pdPASS); // Remember to clear the queue
      f_run=false;
    }
    if(xQueueReceive(qDrinkId,&drinkId,pdMS_TO_TICKS(50))==pdPASS){
      f_run=true;
      currentScroll=0;
      firstNonZero=0;
	  
      for(i=0;(i<8)&&(!drink[drinkId].ingredients[i]);i++);
      
      firstNonZero=i;
      for(i=7;(i>=0)&&(!drink[drinkId].ingredients[i]);i--);

      lastNonZero=i-1;
      memset(&screenData,0,sizeof(screenData));
      sprintf(screenData.lines[0],"[%2d]%s",drinkId+1,drink[drinkId].drinkName);
    }

    if(f_run){
      //Other scrollable lines
      if(currentScroll>=lastNonZero)
        currentScroll=firstNonZero;
      
      for(i=0;i<3;i++){
        while((currentScroll<lastNonZero)&&(!drink[drinkId].ingredients[i+currentScroll])){
          currentScroll++;
        }
        if(currentScroll>=lastNonZero){
          currentScroll=firstNonZero;
        }
        else{
          memset(screenData.lines[1+i],0,sizeof(screenData.lines[0]));
          sprintf(screenData.lines[1+i],"%s",ingredients[i+currentScroll]);
          sprintf(screenData.lines[1+i]+13,"%3d[ml]",drink[drinkId].ingredients[i+currentScroll]);
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

