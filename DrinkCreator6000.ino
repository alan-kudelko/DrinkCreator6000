#include "DrinkCreator6000_Config.h"
#include "DrinkCreator6000_Init.h"
#include "LcdCharacters.h"
#include "EEPROM_Management.h"
#include <Wire.h>

void updateMemoryUsage(){
  heap_end=__brkval?__brkval:(void*)&__heap_start;
  heap_size=(uint16_t)heap_end-(uint16_t)&__heap_start;
  stack_size=(uint16_t)RAMEND-(uint16_t)stack_ptr;
  
  ram_total_free=(uint16_t)stack_ptr-(uint16_t)heap_end;
  ram_in_use=uint16_t(ram_size)-uint16_t(ram_total_free);
}
void ram_dump(){
  char buffer[6]{};
  updateMemoryUsage();
  
  Serial.println(F("[#####]====[MEMORY STATUS]====[#####]"));
  Serial.println(F("[     ] START |  END  | SIZE  [     ]"));

  sprintf(buffer,"%04X",(uint16_t)&__data_start);
  Serial.print(F("[.DATA]0x")); Serial.print(buffer);
  sprintf(buffer,"%04X",(uint16_t)&__data_end);
  Serial.print(F("|0x")); Serial.print(buffer);
  sprintf(buffer,"%4d",(uint16_t)((uint16_t)&__data_end-(uint16_t)&__data_start));
  Serial.print(F("|")); Serial.print(buffer); Serial.println(F(" B[.DATA]"));

  sprintf(buffer,"%04X",(uint16_t)&__bss_start);
  Serial.print(F("[.BSS ]0x")); Serial.print(buffer);
  sprintf(buffer,"%04X",(uint16_t)&__bss_end);
  Serial.print(F("|0x")); Serial.print(buffer);
  sprintf(buffer,"%4d",(uint16_t)((uint16_t)&__bss_end-(uint16_t)&__bss_start));
  Serial.print(F("|")); Serial.print(buffer); Serial.println(F(" B[.BSS ]"));

  sprintf(buffer,"%04X",(uint16_t)&__heap_start);
  Serial.print(F("[HEAP ]0x")); Serial.print(buffer);
  sprintf(buffer,"%04X",(uint16_t)heap_end);
  Serial.print(F("|0x")); Serial.print(buffer);
  sprintf(buffer,"%4d",heap_size);
  Serial.print(F("|")); Serial.print(buffer); Serial.println(F(" B[HEAP ]"));

  sprintf(buffer,"%04X",(uint16_t)stack_ptr);
  Serial.print(F("[STACK]0x")); Serial.print(buffer);
  sprintf(buffer,"%04X",RAMEND);
  Serial.print(F("|0x")); Serial.print(buffer);
  sprintf(buffer,"%4d",stack_size);
  Serial.print(F("|")); Serial.print(buffer); Serial.println(F(" B[STACK]"));

  Serial.print(F("[FREE ]-------------|"));
  sprintf(buffer, "%4d",ram_total_free);
  Serial.print(buffer);
  Serial.println(F(" B[FREE ]"));

  Serial.println(F("[#####]====[MEMORY STATUS]====[#####]"));
}
void lastError_dump(sSystemError*lastError){
  char buffer[50]{};
  Serial.println(F("[XXXXX]================[LAST ERROR DUMP]=================[XXXXX]"));
    sprintf(buffer,"[XXXXX]%50s[XXXXX]",lastError->errorText);
  Serial.println(buffer);
    sprintf(buffer,"[XXXXX]Failure after:           %3d days %2d h %2d min %2d s[XXXXX]",lastError->days,lastError->hours,lastError->minutes,lastError->seconds);
  Serial.println(buffer);
  Serial.println(F("[XXXXX]================[LAST ERROR DUMP]=================[XXXXX]"));
  Serial.println(F(""));
}
void lastBootup_dump(uint16_t*bootup){
  Serial.print(F("[#####]Bootups count: "));
  Serial.print(*bootup);
  Serial.println(F("[#####]"));
  Serial.println(F(""));
}
void faultStart(){
  // Startup after detecting malloc failure, stack overflow or other errors
  taskHandles[TASK_ERROR_HANDLER]  =xTaskCreateStatic(taskErrorHandler ,"ERROR HANDLER",TASK_ERROR_HANDLER_STACK_SIZE  ,NULL                ,1,errorHandlerStack,&errorHandlerTCB);
  taskHandles[TASK_STACK_DEBUGGER] =xTaskCreateStatic(taskStackDebugger,"STACK DEBUG"  ,TASK_STACK_DEBUGGER_STACK_SIZE ,NULL                ,1,stackDebuggerStack,&stackDebuggerTCB);
  taskHandles[TASK_UPDATE_SCREEN]  =xTaskCreateStatic(taskUpdateScreen ,"UPDATE SCREEN",UPDATE_SCREEN_STACK_SIZE       ,NULL                ,1,updateScreenStack,&updateScreenTCB);
  taskHandles[TASK_MAIN]           =xTaskCreateStatic(taskMain         ,"MAIN"         ,TASK_MAIN_STACK_SIZE           ,NULL                ,1,mainStack,&mainTCB); 
  taskHandles[TASK_READ_INPUT]     =xTaskCreateStatic(taskReadInput    ,"READ INPUT"   ,TASK_READ_INPUT_STACK_SIZE     ,NULL                ,3,readInputStack,&readInputTCB);
  taskHandles[TASK_SHOW_LAST_ERROR]=xTaskCreateStatic(taskShowLastError,"LAST ERROR"   ,TASK_SHOW_LAST_ERROR_STACK_SIZE,NULL,2,showLastErrorStack,&showLastErrorTCB);  
  taskHandles[TASK_KEYBOARD_SIM]   =xTaskCreateStatic(taskKeyboardSim  ,"KEYBOARD SIM" ,TASK_KEYBOARD_SIM_STACK_SIZE   ,NULL                    ,1,keyboardSimStack,&keyboardSimTCB);	  
}
void normalStart(){
  // Normal operating mode
  // error handler should be suspended
  // For now i will leave Serial debugging active, will be deleted in release
  taskHandles[TASK_ERROR_HANDLER]  =xTaskCreateStatic(taskErrorHandler ,"ERROR HANDLER",TASK_ERROR_HANDLER_STACK_SIZE  ,NULL                ,1,errorHandlerStack,&errorHandlerTCB);
  taskHandles[TASK_STACK_DEBUGGER] =xTaskCreateStatic(taskStackDebugger,"STACK DEBUG"  ,TASK_STACK_DEBUGGER_STACK_SIZE ,NULL                ,1,stackDebuggerStack,&stackDebuggerTCB);
  taskHandles[TASK_UPDATE_SCREEN]  =xTaskCreateStatic(taskUpdateScreen ,"UPDATE SCREEN",UPDATE_SCREEN_STACK_SIZE       ,NULL                ,1,updateScreenStack,&updateScreenTCB);
  taskHandles[TASK_MAIN]           =xTaskCreateStatic(taskMain         ,"MAIN"         ,TASK_MAIN_STACK_SIZE           ,NULL                ,1,mainStack,&mainTCB);
  taskHandles[TASK_REGULATE_TEMP]  =xTaskCreateStatic(taskRegulateTemp ,"REGULATE TEMP",TASK_REGULATE_TEMP_STACK_SIZE  ,NULL                ,1,regulateTempStack,&regulateTempTCB);
  taskHandles[TASK_READ_INPUT]     =xTaskCreateStatic(taskReadInput    ,"READ INPUT"   ,TASK_READ_INPUT_STACK_SIZE     ,NULL                ,3,readInputStack,&readInputTCB);
  taskHandles[TASK_SELECT_DRINK]   =xTaskCreateStatic(taskSelectDrink  ,"SELECT DRINK" ,TASK_SELECT_DRINK_STACK_SIZE   ,NULL                ,1,selectDrinkStack,&selectDrinkTCB);
  taskHandles[TASK_ORDER_DRINK]    =xTaskCreateStatic(taskOrderDrink   ,"ORDER DRINK"  ,TASK_ORDER_DRINK_STACK_SIZE    ,NULL                    ,1,orderDrinkStack,&orderDrinkTCB);
  taskHandles[TASK_SHOW_INFO]      =xTaskCreateStatic(taskShowInfo     ,"SHOW INFO"    ,TASK_SHOW_INFO_STACK_SIZE      ,(void*)&bootupsCount    ,1,showInfoStack,&showInfoTCB);
  taskHandles[TASK_SHOW_LAST_ERROR]=xTaskCreateStatic(taskShowLastError,"LAST ERROR"   ,TASK_SHOW_LAST_ERROR_STACK_SIZE,NULL,2,showLastErrorStack,&showLastErrorTCB);
  taskHandles[TASK_KEYBOARD_SIM]   =xTaskCreateStatic(taskKeyboardSim  ,"KEYBOARD SIM" ,TASK_KEYBOARD_SIM_STACK_SIZE   ,NULL                    ,1,keyboardSimStack,&keyboardSimTCB);	
}
extern "C" void vApplicationStackOverflowHook(TaskHandle_t xTask,char*pcTaskName){
  vTaskResume(taskHandles[TASK_ERROR_HANDLER]);
  xQueueSend(qErrorId,&xTask,pdMS_TO_TICKS(50));
  //Wake up higher prority tasks
}
uint16_t countDigits(uint16_t n){
    if(n==0)
      return 1;
    uint16_t count=0;
    while(n!=0) {
        n/=10;
        count++;
    }
    return count;
}
void setInputFlag(){
    BaseType_t xHigherPriorityTaskWoken=pdFALSE;
    if(f_enableISR){
      xSemaphoreGiveFromISR(sem_ReadData,&xHigherPriorityTaskWoken);
      //f_enableISR=false;
    }
}
uint8_t processDrinkSelectMenu(uint8_t keyboardData,uint8_t*actualScreenPar,uint8_t*nextScreenPar){
	if((keyboardData&E_GREEN_BUTTON)==E_GREEN_BUTTON){
		// Code for ordering drink
		// To be implemented
		return DRINK_ORDER;
	}
	else if((keyboardData&E_LWHITE_BUTTON)==E_LWHITE_BUTTON){
		// Code for scrolling drinks
		if(*actualScreenPar>0)
			(*actualScreenPar)--;
		
		xQueueSend(qDrinkId,actualScreenPar,pdMS_TO_TICKS(50));
		
		return DRINK_SELECT;
	}
	else if((keyboardData&E_RWHITE_BUTTON)==E_RWHITE_BUTTON){
		// Code for scrolling drinks
		if(*actualScreenPar<19)
			(*actualScreenPar)++;

		xQueueSend(qDrinkId,actualScreenPar,pdMS_TO_TICKS(50));
		return DRINK_SELECT;
	}
	else if((keyboardData&E_BLUE_BUTTON)==E_BLUE_BUTTON){
		// Code for opening show info menu
		xQueueSend(qShowInfoId,nextScreenPar,pdMS_TO_TICKS(50)); // Activate 
		xTaskNotifyGive(taskHandles[TASK_SELECT_DRINK]); // Stop select drink task
		return SHOW_INFO;
	}
}
uint8_t processShowInfoMenu(uint8_t keyboardData,uint8_t*actualScreenPar,uint8_t*nextScreenPar,uint8_t*previousScreenPar){
  uint8_t defval=0;
  if((keyboardData&E_LWHITE_BUTTON)==E_LWHITE_BUTTON){
    // Code for scrolling info
    if(*actualScreenPar>0)
      (*actualScreenPar)--;
    
    xQueueSend(qShowInfoId,actualScreenPar,pdMS_TO_TICKS(50));
    
    return SHOW_INFO;
  }
  else if((keyboardData&E_RWHITE_BUTTON)==E_RWHITE_BUTTON){
    // Code for scrolling info
    if(*actualScreenPar<SHOW_INFO_MENUS_COUNT)
      (*actualScreenPar)++;

    xQueueSend(qShowInfoId,actualScreenPar,pdMS_TO_TICKS(50));
    return SHOW_INFO;
  }
  else if((keyboardData&E_BLUE_BUTTON)==E_BLUE_BUTTON){
	xQueueSend(qLastErrorId,&defval,pdMS_TO_TICKS(50)); // Show last Error
	xTaskNotifyGive(taskHandles[TASK_SHOW_INFO]);
    return SHOW_LAST_ERROR;
  }
  else if((keyboardData&E_RED_BUTTON)==E_RED_BUTTON){
    xQueueSend(qDrinkId,previousScreenPar,pdMS_TO_TICKS(50));
	xTaskNotifyGive(taskHandles[TASK_SHOW_INFO]);
    return DRINK_SELECT;
  }
}
uint8_t processLastErrorMenu(uint8_t keyboardData,uint8_t*actualScreenPar,uint8_t*nextScreenPar,uint8_t*previousScreenPar){
  uint8_t defval=0;
  if((keyboardData&E_GREEN_BUTTON)==E_GREEN_BUTTON){
	//xQueueSend(  
  }
  if((keyboardData&E_RED_BUTTON)==E_RED_BUTTON){
	xQueueSend(qShowInfoId,&defval,pdMS_TO_TICKS(50));
	xTaskNotifyGive(taskHandles[TASK_SHOW_LAST_ERROR]);
    return SHOW_INFO;
  }
  // Kasowanie bledu
}
void ShowInfo_Display1Sub(sScreenData*screenData,uint8_t*showInfoScreenId,uint8_t*textScroll,void*pvParameters){
  uint64_t runTimeFromMillis=0;
  uint8_t runTimeDays=0;
  uint8_t runTimeHours=0;
  uint8_t runTimeMinutes=0;
  uint8_t runTimeSeconds=0;	
  
  switch(*showInfoScreenId){
    case 0:
	  sprintf(screenData->lines[1],"%s","Software ver. 3.0");
	  sprintf(screenData->lines[2],"%s","Author: Alan Kudelko");
	  sprintf(screenData->lines[3],"%s %d","Startup count: ",*(uint16_t*)pvParameters);	
	break;
	case 1:
	  sprintf(screenData->lines[1],"%s","Current run time");
      runTimeFromMillis=millis()/1000;
      runTimeDays=runTimeFromMillis/86400;
      runTimeHours=runTimeFromMillis/3600%24;
      runTimeMinutes=runTimeFromMillis/60%60;
      runTimeSeconds=runTimeFromMillis%60;
      memset(screenData->lines[2],0,sizeof(screenData->lines[2]));
      sprintf(screenData->lines[2],"%2d %s %2d %s",runTimeDays,"days",runTimeHours,"h");
      if(runTimeDays<10)
        screenData->lines[2][0]='0';
      if(runTimeHours<10)
        screenData->lines[2][8]='0';
      memset(screenData->lines[3],0,sizeof(screenData->lines[3]));
      sprintf(screenData->lines[3],"%2d %s  %2d %s",runTimeMinutes,"min",runTimeSeconds,"s");
      if(runTimeMinutes<10)
        screenData->lines[3][0]='0';
      if(runTimeSeconds<10)
        screenData->lines[3][8]='0';	
	break;
  }
  sprintf(screenData->lines[0],"%s","Drink Creator 6000");  
}
void ShowInfo_Display2Sub(sScreenData*screenData){
  // sprintf with %f is disabled on AVR, because it requires extra code
  uint8_t mantissa;
  uint8_t integer;
  sprintf(screenData->lines[0],"%s","Drink Creator 6000");
  
  integer=currentTemperature;
  mantissa=uint8_t(currentTemperature*10)%10;
  sprintf(screenData->lines[1],"T: %2d.%d\xDF""C S:",integer,mantissa);
  
  integer=setTemperature;
  mantissa=uint8_t(setTemperature*10)%10;
  sprintf(screenData->lines[1]+14,"%2d.%d\xDF""C",integer,mantissa);
  
  integer=temperatureHysteresis;
  mantissa=uint8_t(temperatureHysteresis*10)%10;
  sprintf(screenData->lines[2],"Hyst: %d.%d\xDF""C",integer,mantissa);
  
  sprintf(screenData->lines[3],"Status: %s",digitalRead(Pelt1Pin)==HIGH?"Cooling":"Idle");
}
void ShowInfo_Display3Sub(sScreenData*screenData,uint8_t*scroll){
  updateMemoryUsage();
  memset(screenData,0,sizeof(sScreenData));
  sprintf(screenData->lines[0],"%s","RAM Info");
  if(*scroll==0){
	  // Free RAM
	  char buffer[13]{};
	  uint8_t ram_percent=100*uint32_t(ram_in_use)/ram_size;  
	  
	  memset(buffer,'-',sizeof(buffer)-1);
	  buffer[0]='[';
	  buffer[11]=']';
	  memset(buffer+1,'#',ram_percent/10);
    sprintf(screenData->lines[1],"%s %4u B/%4u B","Usage:",ram_in_use,ram_size);
    sprintf(screenData->lines[2],"%12s %3u %%",buffer,ram_percent);
  }
  else if(*scroll==1){
	   //.data sector
      sprintf(screenData->lines[1],".data: 0x%4X-0x%4X",uint16_t(&__data_start),uint16_t(&__data_end));
	   //.bss sector
      sprintf(screenData->lines[2],".bss:  0x%4X-0x%4X",uint16_t(&__bss_start),uint16_t(&__bss_end));
	   //Size of each memory segment
      sprintf(screenData->lines[3],"Size:  %4uB %4uB",uint16_t(&__data_end)-uint16_t(&__data_start),uint16_t(&__bss_end)-uint16_t(&__bss_start));
  }
  else if(*scroll==2){
	   //heap sector
      sprintf(screenData->lines[1],"HEAP: 0x%4X-0x%4X",uint16_t(&__heap_start),heap_end);
	   //stack sector	
      sprintf(screenData->lines[2],"STACK:  0x%4X-0x%4X",uint16_t(stack_ptr),RAMEND);
	   //Size of each memory segment
      sprintf(screenData->lines[3],"Size:  %4uB %4uB",heap_size,stack_size);	  
  }
  
  (*scroll)++;
  if((*scroll)>2)
	  *scroll=0;
}
void ShowInfo_Display4Sub(sScreenData*screenData,uint8_t*scroll){
  // Should work but needs fixing
  sprintf(screenData->lines[0],"%s","Task Info");
  
  if(taskHandles[*scroll]==nullptr){
	sprintf(screenData->lines[1],"%s%dCorrupted","Task id=",*scroll);
  }
  else{
	char buffer[10]{};
	TaskStatus_t taskStatus{};
	vTaskGetInfo(taskHandles[*scroll],&taskStatus,pdTRUE,eInvalid);
	
    sprintf(screenData->lines[1],"[%2u]%s",*scroll,taskStatus.pcTaskName);
	sprintf(screenData->lines[2],"Highwater mark: %3u",taskStatus.uxStackHighWaterMark);
	sprintf(screenData->lines[3],"Pr:%1u State:",taskStatus.uxCurrentPriority);
    switch(taskStatus.eCurrentState){
      case eReady:
        strcpy(buffer,"Ready");
      break;
      case eRunning:
        strcpy(buffer,"Running");
      break;
      case eBlocked:
        strcpy(buffer,"Blocked");
      break;
      case eSuspended:
        strcpy(buffer,"Suspended");
      break;
      case eDeleted:
        strcpy(buffer,"Deleted");
      break;
      default:
        strcpy(buffer,"Unknown");
    }	
	memcpy(screenData->lines[3]+11,buffer,9);
  }
  
  
  (*scroll)++;
  if((*scroll)>=TASK_N)
	  *scroll=0;
}
// Tasks
void taskErrorHandler(void*pvParameters){
  uint8_t i=1;
  uint32_t runTimeFromMillis=0;
  sSystemError lastError{};
  bool f_run=false;
  TaskHandle_t overflowedTask{};
  
  for(;;){
    if(xQueueReceive(qErrorId,&overflowedTask,pdMS_TO_TICKS(50))==pdPASS){
      f_run=true;
	  
      for(;i<TASK_N;i++)
        vTaskSuspend(taskHandles[i]);    
	
      sprintf(lastError.errorText,"Stack overflow in task: %s",pcTaskGetName(overflowedTask));
      runTimeFromMillis=millis()/1000;
      lastError.days=runTimeFromMillis/3600/24;
      lastError.hours=runTimeFromMillis/3600%24;
      lastError.minutes=runTimeFromMillis/60%60;
      lastError.seconds=runTimeFromMillis%60;
      lastError.confirmed=false;
	  
	  Serial.println(F("[####################]====SYSTEM CRITICAL====[##]"));
      Serial.println((const char*)lastError.errorText);
      Serial.println(F("[####################]====SYSTEM CRITICAL====[##]"));
      Serial.println("");
	  
      //EEPROMUpdateLastStartupError(&lastError);
	  
	  //stopAllDevices();	  
    }
    if(f_run){
	  // activate speaker
	  // Delay and systems restart
	  // IF error occured, all devices should be safely shut down
    }
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}
void taskStackDebugger(void*pvParameters){
  char buffer[21]{};
  byte i=0;
  uint8_t nameLength{};
  TaskStatus_t taskStatus{};
  for(;;){
      Serial.println();
      Serial.println(F("[####################]====TASK STATUS===[##]"));
      Serial.println(F("[     TASK NAME      ]STACK|  STATE  |PR[ID]"));
      for(i=0;i<TASK_N;i++){
        memset(buffer,0,sizeof(buffer));
        if(taskHandles[i]==nullptr)
          continue;
        vTaskGetInfo(taskHandles[i],&taskStatus,pdTRUE,eInvalid);
        sprintf(buffer,"%s",taskStatus.pcTaskName);
        nameLength=strlen(taskStatus.pcTaskName);
        memset(buffer+nameLength,' ',sizeof(buffer)-nameLength);
        buffer[20]='\0';
        Serial.print(F("[")); Serial.print(buffer); Serial.print(F("]"));
        sprintf(buffer,"%5d",taskStatus.uxStackHighWaterMark);
        Serial.print(buffer); Serial.print(F("|"));
        switch(taskStatus.eCurrentState){
          case eReady:
            Serial.print(F("Ready    "));
          break;
          case eRunning:
            Serial.print(F("Running  "));
          break;
          case eBlocked:
            Serial.print(F("Blocked  "));
            break;
          case eSuspended:
            Serial.print(F("Suspended"));
          break;
          case eDeleted:
            Serial.print(F("Deleted  "));
            break;
          default:
            Serial.print(F("Unknown  "));
        }
        Serial.print(F("| ")); Serial.print(taskStatus.uxCurrentPriority);
        sprintf(buffer,"%2d",i);
        Serial.print(F("[")); Serial.print(buffer); Serial.println(F("]"));
      }
    Serial.println(F("[####################]====TASK STATUS===[##]"));

    updateMemoryUsage();
    // Stack, heap etc
    Serial.print("stack_ptr: ");
    Serial.println(uint16_t(stack_ptr),HEX);

    Serial.print("heap_end: ");
    Serial.println(uint16_t(heap_end),HEX);

    
    Serial.print("stack_size: ");
    Serial.println(uint16_t(stack_size),HEX);
    
    Serial.print("heap_size: ");
    Serial.println(uint16_t(heap_size),HEX);

    Serial.print("ram_total_free: ");
    Serial.println(uint16_t(ram_total_free));
    
    Serial.print("ram_in_use: ");
    Serial.println(uint16_t(ram_in_use));

    Serial.print("ram_size: ");
    Serial.println(uint16_t(ram_size));

    Serial.print("__data_start: ");
    Serial.println(uint16_t(&__data_start),HEX);

    Serial.print("__data_end: ");
    Serial.println(uint16_t(&__data_end),HEX);
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}
void taskMain(void*pvParameters){
  sScreenData screenData{};
  bool f_keyboardDataReceived=false;
  uint8_t screenId=1;
  uint8_t drinkId=5;
  uint8_t showInfoId=0;
  uint8_t showRamId=0;
  uint8_t showTempId=0;
  uint8_t keyboardData=0;

  xQueueSend(qDrinkId,&drinkId,pdMS_TO_TICKS(50));
  
  for(;;){
    if(xQueueReceive(qKeyboardData,&keyboardData,pdMS_TO_TICKS(1000))){
	    f_keyboardDataReceived=true;
    }
	if(f_keyboardDataReceived){
      switch(screenId){
	    case WELCOME_SCREEN: 
		1; //screenId=processWelcomeMenu(keyboardData)// aktywacja drink select
	    break;
		case DRINK_SELECT:
		  screenId=processDrinkSelectMenu(keyboardData,&drinkId,&showInfoId);
		break;
	    case DRINK_ORDER:
		  1;
		break;
		case SHOW_INFO:
		  screenId=processShowInfoMenu(keyboardData,&showInfoId,&showTempId,&drinkId);
		break;
		case SHOW_LAST_ERROR:
		  1; //screenId=processLastErrorMenu(keyboardData,&showInfoId,
		break;
		}
		f_keyboardDataReceived=false;
		// Should be executed only once after received qKeyboardData
	  }
  }
}
//////////////////////////////////////////////////////////////////
// Screen update task:
// Consumer of screen data queue, handles LCD display refresh,
// custom characters, and cursor control with I2C bus synchronization - mux_I2CLock
void taskUpdateScreen(void*pvParameters){
  sScreenData receivedLcdData{};

  TickType_t xLastWakeTime=xTaskGetTickCount();
  //const TickType_t xFrequency=pdMS_TO_TICKS(300);
  uint8_t i=0;
  uint8_t j=0;
  
  for(;;){
    if(xQueueReceive(qScreenData,&receivedLcdData,pdMS_TO_TICKS(0))==pdPASS){
      if(xSemaphoreTake(mux_I2CLock,pdMS_TO_TICKS(0))==pdPASS){
        for(i=0;i<LCD_HEIGHT;i++){
          lcd.setCursor(0,i);
          for(j=0;j<LCD_WIDTH;j++)
          if(receivedLcdData.lines[i][j]==0)
            lcd.write(' ');
          else if(receivedLcdData.lines[i][j]==E_LOADING_BAR)
            lcd.write(0);
          else if(receivedLcdData.lines[i][j]==18)
            lcd.write(1);
          else if(receivedLcdData.lines[i][j]==19)
            lcd.write(2);
          else if(receivedLcdData.lines[i][j]==20)
            lcd.write(3);
          else if(receivedLcdData.lines[i][j]==21)
            lcd.write(4);
          else if(receivedLcdData.lines[i][j]==22)
            lcd.write(5);
          else if(receivedLcdData.lines[i][j]==23)
            lcd.write(6);
          else if(receivedLcdData.lines[i][j]==24)
            lcd.write(7);
          else
            lcd.write(receivedLcdData.lines[i][j]);
        }
        lcd.setCursor(receivedLcdData.lcdCursorX,receivedLcdData.lcdCursorY);
        if(receivedLcdData.lcdCursorBlink)
          lcd.blink();
        else
          lcd.noBlink();
        xSemaphoreGive(mux_I2CLock);
      }
    }
    vTaskDelayUntil(&xLastWakeTime,pdMS_TO_TICKS(TASK_UPDATE_SCREEN_REFRESH_RATE)); 
  }
}
//////////////////////////////////////////////////////////////////
// Temperature regulation task:
// Uses global temperature variables to control Peltier elements
// by switching their pins HIGH or LOW based on hysteresis thresholds.
void taskRegulateTemp(void*pvParameters){
  for(;;){
    if(currentTemperature>=setTemperature+temperatureHysteresis){
      digitalWrite(Pelt1Pin,HIGH); 
      digitalWrite(Pelt2Pin,HIGH);
    }
    else if(currentTemperature<=setTemperature-temperatureHysteresis){
      digitalWrite(Pelt1Pin,LOW);
      digitalWrite(Pelt2Pin,LOW);
    }
    vTaskDelay(pdMS_TO_TICKS(TASK_REGULATE_TEMP_REFRESH_RATE));
  }
}
void taskReadInput(void*pvParameters){
  uint8_t keyboardInput=0;
  //TickType_t xLastWakeTime=xTaskGetTickCount();
  
  for(;;){
    if(xSemaphoreTake(sem_ReadData,pdMS_TO_TICKS(portMAX_DELAY))==pdTRUE){
      Serial.println("Woke up from ISR");
      if(xSemaphoreTake(mux_I2CLock,pdMS_TO_TICKS(portMAX_DELAY))==pdTRUE){
        Wire.beginTransmission(MCP_ADDR);
        Wire.write(0x08);
        Wire.endTransmission();
        Wire.requestFrom(MCP_ADDR,1);
        
        keyboardInput=Wire.read();

        Wire.beginTransmission(MCP_ADDR);
        Wire.write(0x00);
        Wire.endTransmission();
        Wire.requestFrom(MCP_ADDR,1);
        Wire.read();
                
        xSemaphoreGive(mux_I2CLock);
        
        Serial.print("Dane klawa: ");
        Serial.println(keyboardInput,BIN);

        xQueueSend(qKeyboardData,&keyboardInput,pdMS_TO_TICKS(50));
        //Serial.print(keyboardInput); Serial.println(" ISR");
        //f_enableISR=true;
        //vTaskDelayUntil(&xLastWakeTime,pdMS_TO_TICKS(100));
          //keyboardInput=~keyboardInput;
      }
    }
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}
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
void taskShowInfo(void*pvParameters){
  uint8_t showInfoScreenId=0;
  uint8_t scroll=0;
  // 0 - 1 version, runtime informations 1 second
  // 2 temperature informations 1 second
  // 3 ram information with scroll 2 seconds
  // 4 stack information with scroll 2 seconds
  bool f_run=false;
  
  sScreenData screenData{};
  for(;;){
    if(ulTaskNotifyTake(pdTRUE,0)>0){
	    while(xQueueReceive(qShowInfoId,&showInfoScreenId,pdMS_TO_TICKS(100))==pdPASS);
		scroll=0;
      f_run=false;
    }
	if(xQueueReceive(qShowInfoId,&showInfoScreenId,pdMS_TO_TICKS(50))==pdPASS){
	  f_run=true;
	  scroll=0;
	}
	if(f_run){
    Serial.println(scroll);
    memset(&screenData,0,sizeof(screenData));
	  
	  switch(showInfoScreenId){
		case 0:
		case 1:
		ShowInfo_Display1Sub(&screenData,&showInfoScreenId,&scroll,pvParameters);
		break;
    
		case 2:
		ShowInfo_Display2Sub(&screenData);
		break;
    
		case 3:
		ShowInfo_Display3Sub(&screenData,&scroll);
		break;
    
		case 4:
		ShowInfo_Display4Sub(&screenData,&scroll);
		break;
	  }
	  xQueueSend(qScreenData,&screenData,pdMS_TO_TICKS(50));
	}
  vTaskDelay(pdMS_TO_TICKS(TASK_SHOW_INFO_REFRESH_RATE));
  }
}
void taskShowLastError(void*pvParameters){
  uint8_t scroll=0;
  uint8_t keyboardData=0; //Change name to sth meaningful
  uint8_t errorLength=0;
  sScreenData screenData{};
  sSystemError lastSystemError{};
  bool f_run=false;
  // Error should be confirmed in here, but task name needs to be changed
  // to avoid ambiguity
  for(;;){
	if(ulTaskNotifyTake(pdTRUE,0)>0){
		while(xQueueReceive(qLastErrorId,&keyboardData,pdMS_TO_TICKS(50)));
		f_run=false;
	}
    if(xQueueReceive(qLastErrorId,&keyboardData,pdMS_TO_TICKS(50))){
	  EEPROMGetLastStartupError(&lastSystemError);	
	  memset(&screenData,0,sizeof(screenData));
	  
	  if(keyboardData==0){	
		errorLength=strlen(lastSystemError.errorText);

		strncpy(screenData.lines[1],"Fault time signature",LCD_WIDTH);
		sprintf(screenData.lines[2],"%2d days %2d h",lastSystemError.days,lastSystemError.hours);
		if(lastSystemError.days<10)
		  screenData.lines[2][0]='0';
        if(lastSystemError.hours<10)
          screenData.lines[2][8]='0';
      
        sprintf(screenData.lines[3],"%2d min  %2d s",lastSystemError.minutes,lastSystemError.seconds);
        if(lastSystemError.minutes<10)
          screenData.lines[3][0]='0';
        if(lastSystemError.seconds<10)
          screenData.lines[3][8]='0';   	
		//Add text that its confirmed or not
	  }
	  else if(keyboardData==1){
		// Confirming error
		sprintf(screenData.lines[0],"%s","Error confirmed");
		sprintf(screenData.lines[1],"%s","EEPROM Updated");
		
		lastSystemError.confirmed=true;
		EEPROMUpdateLastStartupError(&lastSystemError);
		
		xQueueSend(qScreenData,&screenData,pdMS_TO_TICKS(50));
	    vTaskDelay(pdMS_TO_TICKS(1500));
		keyboardData=0;
		xQueueSend(qLastErrorId,&keyboardData,pdMS_TO_TICKS(50));
	  }
	  scroll=0;
	  
	  f_run=true;
    }
	if(f_run){
	  strncpy(screenData.lines[0],lastSystemError.errorText+scroll,LCD_WIDTH);
	  scroll++;
	  if(scroll==errorLength-LCD_WIDTH+2){
		scroll=0;
      }
      xQueueSend(qScreenData,&screenData,pdMS_TO_TICKS(50));
	}
    vTaskDelay(pdMS_TO_TICKS(400));
  }
}
void taskKeyboardSim(void*pvParameters){
  uint8_t keyboardData=0;
  // Added for testing as PCF8574 is a bad choice for a keyboard driver
  // will switch to MCP23017
  for(;;){
    while(!Serial.available()){
      vTaskDelay(pdMS_TO_TICKS(100));
    }
    if(Serial.peek()==10)
      Serial.read();
    if(Serial.available()){
      keyboardData=(1<<(Serial.read()-49)); // We need the test data to be powers of 2
      xQueueSend(qKeyboardData,&keyboardData,pdMS_TO_TICKS(50));
    }
  }
}
void taskWelcome(void*pvParameters){
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
	
    vTaskDelay(pdMS_TO_TICKS(TASK_WELCOME_REFRESH_RATE));
  }
}
void setup(){
  while(!eeprom_is_ready());
  
  EEPROMUpdateBootups(&bootupsCount);
  //EEPROMGetLastStartupError(&lastSystemError);
  
  initializeIO();
  initializeMemory();
  initializeHardware();
  
  //f_errorConfirmed=lastSystemError.confirmed;
  f_errorConfirmed=1;

  lastBootup_dump(&bootupsCount);  
  //lastError_dump(&lastSystemError);
  stack_ptr=(uint8_t*)SP;
  // Save stack pointer before FreeRTOS starts, because SP becomes volatile due to context switching.
  // After vTaskStartScheduler(), SP will change dynamically depending on the active task.
  // So we treat this saved SP as the top of the main stack (pre-RTOS).  
  ram_dump();    
  
  if(f_errorConfirmed==0){
    faultStart();
  }
  else{	  
    normalStart();
  }
}
void loop(){

}
