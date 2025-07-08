#include "taskOrderDrink.h"

void taskOrderDrink(void*pvParameters){
  //Powiadomienie zakoncz w dowolnym momencie np. przycisk awaryjny
  //Kolejka na realizacje drinka
  //Wyslanie informacji o postepie na ekran poprzez kolejke
  //flaga uruchomienia
  //Czas uruchomienia
  //Postep
  //Wyslanie emulowanego przycisku z klawiatury do maina ze zakonczono
  //Nalewanie sekwencyjne
  //Wysylanie danych do rejestru
  //W resecie nalezy wyslac same zera do pomp
  //Jezeli jest przerwany drink to informacja na lcd ze przerwano
  //Reset zmiennych lokalnych
  //Kolejka do lcd pozwala na dowolna aktualizacje ekranu, wiec nie trzeba robic twardej synchronizacji taskow
  bool f_run=false;
  bool f_resetTaskData=false;

  uint16_t totalPouringTime=0;
  uint16_t currentPouringTime=0;
  
  uint8_t drinkId=0;
  uint8_t pumpId=0;
  uint8_t pumpsActivationMask=0;
  uint8_t i=0;
  
  sScreenData screenData{};
  //Odwolanie do globalnej tablicy drinkow
  
  for(;;){
    if(ulTaskNotifyTake(pdTRUE,0)>0){
      f_run=false;
      f_resetTaskData=true;
      // Same flags in main loop (may not be necessery)
    }
    if(f_resetTaskData){
      f_resetTaskData=false;
      i=0;
      pumpsActivationMask=0;
      totalPouringTime=0;
      memset(&screenData,0,sizeof(sScreenData));
    }
    if((false)&&(!f_resetTaskData)&&(xQueueReceive(qDrinkId,&drinkId,pdMS_TO_TICKS(50)))){
      f_run=true;
      for(i=0;i<8;i++)
        totalPouringTime+=drink[drinkId].ingredients[i]*pumpsEff[i];
      totalPouringTime*=1; //Fix, dependent on vTaskDelay

      //Constant lines (do not change during execution)
      strncpy(screenData.lines[0],"Przygotowanie drinka",LCD_WIDTH);
      sprintf(screenData.lines[1],"%s",drink[drinkId].drinkName);
      sprintf(screenData.lines[1]+16,"%[2d]",drinkId);
    }
    
    if(f_run){
      // Main code
      //This is for later, atleast for now
    }
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}
