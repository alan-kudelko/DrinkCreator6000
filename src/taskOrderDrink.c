#include <string.h>
#include <stdio.h>

#include <avr/io.h>
#include <util/delay.h>

#include <taskOrderDrink.h>
#include <DrinkCreator6000_Config_C.h>
#include <DrinkCreator6000_Pins.h>

void shiftOut(uint8_t value){
    uint8_t i=0;

    // SH_PIN PC2 -> Shift Clock
    // DS_PIN PC0 -> Serial Data
    // ST_PIN PC1 -> Latch

    // Latch jest open drain, a przynajmniej musi
    // W sumie wszystko mamy tutaj open drain
    PORTC&=~(1<<ST_PIN);

    for(;i<8;i++){
        if(value&(1<<(7-i))){
            PORTC|=(1<<DS_PIN);
        }
        else{
            PORTC&=~(1<<DS_PIN);
        }

        PORTC|=(1<<SH_PIN);
        _delay_us(1);
        PORTC&=~(1<<SH_PIN);
        _delay_us(1);

    }

    PORTC|=(1<<ST_PIN);
}

void taskOrderDrink(void*pvParameters){
    uint32_t f_run=0;
    uint16_t totalTime=0;
    uint32_t currentTime=0;
    uint8_t currentPumpId=0;
    uint8_t i=0;
    uint8_t progress=0; //0 - 100 % needed for loading bar and % value

    uint16_t pumpTime[8]={0};
    uint16_t currentPumpTime=0;
  
    struct sScreenData screenData={0};
    // UI_Context should be used as a "queue" i mean, to indicate which drink should be ordered
    // upon received notification selected drink should be read from the EEPROM
    // When task is completed I see no other option but to send notification to taskSelectDrink and change UI_Context without involing main
    // I don't like that but for now i let this as it is
  
  for(;;){
    if(xTaskNotifyWait(0,0,&f_run,0)>0){
      memset(screenData.lines[0],0,sizeof(struct sScreenData));
      sprintf(screenData.lines[0],"[%2d]%s",UI_Context.currentSubMenu+1,drink[UI_Context.currentSubMenu].drinkName);   
      memset(screenData.lines[3],'-',12);
      screenData.lines[3][0]='[';
      screenData.lines[3][11]=']';
          
      if(f_run==1){
        // Order drink flag
        currentPumpId=0;
        progress=0;
        totalTime=0;
        currentTime=0;
        currentPumpTime=0;
        
        memset(pumpTime,0,8*sizeof(uint16_t));
        strcpy(screenData.lines[2],"Please wait...");
        memset(screenData.lines[3]+1,'#',progress/10);
        sprintf(screenData.lines[3]+13,"%3d %%",progress);
        // Calculate total volume to be pumped
        for(i=0;i<8;i++){
          if(drink[UI_Context.currentSubMenu].ingredients[i])
            pumpTime[i]=1000/TASK_ORDER_DRINK_REFRESH_RATE*60*drink[UI_Context.currentSubMenu].ingredients[i]/(uint16_t)pumpsEff[i];
          totalTime+=pumpTime[i];
        }
        // Remember to stop all pumps upon receiving f_run==0 or f_run==2
      }
      if(f_run==0){
        // Drink order finished succesfully
        shiftOut(0);

        strcpy(screenData.lines[2],"Done!");
        memset(screenData.lines[3]+1,'#',progress/10);
        sprintf(screenData.lines[3]+13,"%3d %%",progress);
             
        for(i=0;i<5;i++)
          xQueueSend(qScreenData,&screenData,pdMS_TO_TICKS(50));
          
        taskENTER_CRITICAL();
        UI_Context.currentTask=DRINK_SELECT;
        UI_Context.currentMenu=0;
        taskEXIT_CRITICAL();
        
        xTaskNotify(taskHandles[TASK_SELECT_DRINK],1,eSetValueWithOverwrite);        
      }
      if(f_run==2){
        // Drink order aborted
        shiftOut(0);

        strcpy(screenData.lines[2],"Aborted");
        memset(screenData.lines[3]+1,'#',progress/10);
        sprintf(screenData.lines[3]+13,"%3d %%",progress);
             
        for(i=0;i<5;i++)
          xQueueSend(qScreenData,&screenData,pdMS_TO_TICKS(50));
          
        taskENTER_CRITICAL();
        UI_Context.currentTask=DRINK_SELECT;
        UI_Context.currentMenu=0;
        taskEXIT_CRITICAL();
        
        xTaskNotify(taskHandles[TASK_SELECT_DRINK],1,eSetValueWithOverwrite);
      }
    }
    if(f_run==1){
      memset(screenData.lines[3]+1,'#',progress/10);
      sprintf(screenData.lines[3]+13,"%3d",progress);
      xQueueSend(qScreenData, &screenData, pdMS_TO_TICKS(50));

      while(!pumpTime[currentPumpId]){
        currentPumpId++;
        currentPumpTime=0;
      }
      if(currentPumpTime==pumpTime[currentPumpId]){
        currentPumpId++;
        currentPumpTime=0;
      }
      shiftOut(1<<currentPumpId);
            
      currentTime++;
      currentPumpTime++;
      progress=100*currentTime/totalTime;

      if(currentTime==totalTime){
        xTaskNotify(taskHandles[TASK_ORDER_DRINK],0,eSetValueWithOverwrite);   
      }
    }
    vTaskDelay(pdMS_TO_TICKS(TASK_ORDER_DRINK_REFRESH_RATE));
  }
}
