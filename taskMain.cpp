#include "taskMain.h"

// Each operation should update current context
// If one or more field is modified, operation should be atomic

void taskMain_ProcessContext_Task_WelcomeScreen(uint8_t*keyboardInput,sUIContext*UI_Context){
  if((*keyboardInput&E_GREEN_BUTTON)==E_GREEN_BUTTON){
    UI_Context->currentTask=DRINK_SELECT;
    // activate next task
    uint8_t drinkId=7;
    xQueueSend(qDrinkId,&drinkId,pdMS_TO_TICKS(100));
  }
}
void taskMain_ProcessContext_taskSelectDrink(uint8_t*keyboardInput,sUIContext*UI_Context){
	
}
void taskMain_ProcessContext_taskOrderDrink(uint8_t*keyboardInput,sUIContext*UI_Context){
	
}
void taskMain_ProcessContext_taskShowSystemInfo(uint8_t*keyboardInput,sUIContext*UI_Context){
	
}

void taskMain(void*pvParameters){
  bool f_keyboardDataReceived=false;
  
  uint8_t keyboardData=0;


  //This task should call coresponding function depended on the current context

  //xQueueSend(qDrinkId,&drinkId,pdMS_TO_TICKS(50));
  
  
  for(;;){
    if(xQueueReceive(qKeyboardData,&keyboardData,pdMS_TO_TICKS(1000))){
	    f_keyboardDataReceived=true;
    }
	// Main logic of the program and sending data to running tasks
	// Main logic in here should be bounded with currentTask context
	if(f_keyboardDataReceived){
    switch(UI_Context.currentTask){
	    case WELCOME_SCREEN: 
		    taskMain_ProcessContext_Task_WelcomeScreen(&keyboardData,&UI_Context);
	    break;
		  case DRINK_SELECT:
        taskMain_ProcessContext_taskSelectDrink(&keyboardData,&UI_Context);
		  break;
	    case DRINK_ORDER:
        ////////////
		  break;
		  case SHOW_INFO:
		    //taskMain_ProcessContext_taskShowSystemInfo(&keyboardData,&UI_Context);
		  break;
		}
		f_keyboardDataReceived=false;
		// Should be executed only once after received qKeyboardData
	  }
  }
}
