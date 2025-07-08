#include "taskMain.h"

void taskMain_ProcessContext_Task_WelcomeScreen(){
	
}
void taskMain_ProcessContext_taskSelectDrink(){
	
}
void taskMain_ProcessContext_taskOrderDrink(){
	
}
void taskMain_ProcessContext_taskShowSystemInfo(){
	
}

void taskMain(void*pvParameters){
  sScreenData screenData{};
  bool f_keyboardDataReceived=true;
  uint8_t screenId=SHOW_INFO;
  uint8_t drinkId=5;
  uint8_t showInfoId=0;
  uint8_t showRamId=0;
  uint8_t showTempId=0;
  uint8_t keyboardData=0;

  //xQueueSend(qDrinkId,&drinkId,pdMS_TO_TICKS(50));
  
  for(;;){
    if(xQueueReceive(qKeyboardData,&keyboardData,pdMS_TO_TICKS(1000))){
	    f_keyboardDataReceived=true;
    }
	// Main logic of the program and sending data to running tasks
	// Main logic in here should be bounded with currentTask context
	if(f_keyboardDataReceived){
      switch(screenId){
	    case WELCOME_SCREEN: 
		1; //UI_Context.currentTask=processWelcomeMenu(keyboardData)// aktywacja drink select
	    break;
		case DRINK_SELECT:
		  //UI_Context.currentTask=processDrinkSelectMenu(keyboardData,&drinkId,&showInfoId);
		break;
	    case DRINK_ORDER:
		  1;
		break;
		case SHOW_INFO:
		  //UI_Context.currentTask=processShowInfoMenu(keyboardData,&showInfoId,&showTempId,&drinkId);
		break;
		}
		f_keyboardDataReceived=false;
		// Should be executed only once after received qKeyboardData
	  }
  }
}