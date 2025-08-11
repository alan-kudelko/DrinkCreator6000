#include <string.h>
#include <stdio.h>

#include <taskSelectDrink.h>
#include <DrinkCreator6000_Config.h>

void taskSelectDrink(void*pvParameters){  
    uint32_t f_run=0;
  
    uint8_t currentScroll=0;
    uint8_t i=0;
    struct sScreenData screenData={0};
  
    for(;;){
        if(xTaskNotifyWait(0,0,&f_run,0)>0){
	          if(f_run==1){
                currentScroll=0;

                if(UI_Context.currentSubMenu>DRINK_COUNT){
                    UI_Context.currentSubMenu=DRINK_COUNT-1;
                }
                if(UI_Context.currentSubMenu==DRINK_COUNT){
                    UI_Context.currentSubMenu=0;
                }
        
                memset(&screenData,0,sizeof(screenData));
                sprintf(screenData.lines[0],"[%2d]%s",UI_Context.currentSubMenu+1,drink[UI_Context.currentSubMenu].drinkName);		  
              }
        }
    if(f_run==1){
      memset(screenData.lines[1],0,sizeof(screenData.lines[0])*3);
	  
	  if((currentScroll==5)&&(!drink[UI_Context.currentSubMenu].ingredients[currentScroll]))
        currentScroll=0;

      if(currentScroll>5)
        currentScroll=0;
      for(i=0;i<3;i++){
		    while(((i+currentScroll)<6)&&(!drink[UI_Context.currentSubMenu].ingredients[i+currentScroll])){
			    currentScroll++;
		    }
		    if(currentScroll==6){
			    break;
		    }
		    // Do not display empty values (needs optimization)
        drink[UI_Context.currentSubMenu].ingredients[i+currentScroll]?sprintf(screenData.lines[1+i],"%s",ingredients[i+currentScroll]):1;
        drink[UI_Context.currentSubMenu].ingredients[i+currentScroll]?sprintf(screenData.lines[1+i]+13,"%3d[ml]",drink[UI_Context.currentSubMenu].ingredients[i+currentScroll]):1;
      }
      currentScroll++;

      xQueueSend(qScreenData, &screenData, pdMS_TO_TICKS(50));
      
      vTaskDelay(pdMS_TO_TICKS(1000));
    }
    else{
        vTaskDelay(pdMS_TO_TICKS(500));
    }
  }
}
