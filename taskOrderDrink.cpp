#include "taskOrderDrink.h"

void taskOrderDrink(void*pvParameters){
  uint32_t f_run=false;
  uint16_t totalVolume=0;
  uint8_t currentPumpId=0;
  uint8_t pumpsActivationMask=0;
  uint8_t i=0;
  uint8_t progress=0; //0 - 100 % needed for loading bar and % value
  
  sScreenData screenData{};
  // UI_Context should be used as a "queue" i mean, to indicate which drink should be ordered
  // upon received notification selected drink should be read from the EEPROM
  // When task is completed I see no other option but to send notification to taskSelectDrink and change UI_Context without involing main
  // I don't like that but for now i let this as it is
  
  for(;;){
    if(xTaskNotifyWait(0,0,&f_run,0)>0){
      memset(screenData.lines[0],0,sizeof(sScreenData));
      sprintf(screenData.lines[0],"[%2d]%s",UI_Context.currentSubMenu+1,drink[UI_Context.currentSubMenu].drinkName);   
      memset(screenData.lines[3],'-',12);
      screenData.lines[3][0]='[';
      screenData.lines[3][11]=']';
      memset(screenData.lines[3]+1,'#',progress);
      sprintf(screenData.lines[3]+13,"%3d %%",progress*10);
          
      if(f_run==1){
        // Order drink flag
        strcpy(screenData.lines[2],"Please wait...");

        totalVolume=0;

        // Calculate total volume to be pumped
        for(i=0;i<8;i++){
          
        }

        
        progress=0;

        
        
      }
      if(f_run==0){
        // Drink order finished succesfully
        strcpy(screenData.lines[2],"Done!");
        
        for(i=0;i<5;i++)
          xQueueSend(qScreenData,&screenData,pdMS_TO_TICKS(50));
          
        taskENTER_CRITICAL();
        UI_Context.currentTask=DRINK_SELECT;
        UI_Context.currentMenu=0;
        UI_Context.currentSubMenu=0;
        taskEXIT_CRITICAL();
        
        xTaskNotify(taskHandles[TASK_SELECT_DRINK],1,eSetValueWithOverwrite);        
      }
      if(f_run==2){
        // Drink order aborted
        strcpy(screenData.lines[2],"Aborted");
        
        for(i=0;i<5;i++)
          xQueueSend(qScreenData,&screenData,pdMS_TO_TICKS(50));
          
        taskENTER_CRITICAL();
        UI_Context.currentTask=DRINK_SELECT;
        UI_Context.currentMenu=0;
        UI_Context.currentSubMenu=0;
        taskEXIT_CRITICAL();
        
        xTaskNotify(taskHandles[TASK_SELECT_DRINK],1,eSetValueWithOverwrite);
      }
    }
    if(f_run==1){
      progress++;
      memset(screenData.lines[3]+1,'#',progress);
      sprintf(screenData.lines[3]+13,"%3d",progress*10);
      xQueueSend(qScreenData, &screenData, pdMS_TO_TICKS(50));

      if(progress==10){
        xTaskNotify(taskHandles[TASK_ORDER_DRINK],0,eSetValueWithOverwrite);   
      }
    }
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}
