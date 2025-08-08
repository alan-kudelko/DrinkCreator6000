#include <stdio.h>
#include <string.h>

#include "taskShowSystemInfo.h"
#include <DrinkCreator6000_RamStats.h>

extern "C" void updateMemoryUsage(void);

void showInfo_Firmware_Sub_0(sScreenData*screenData){
  sprintf(screenData->lines[0],"%s","Drink Creator 6000");
  sprintf(screenData->lines[1],"%s","Software ver. 3.0");
  sprintf(screenData->lines[2],"%s","Author: Alan Kudelko");
  sprintf(screenData->lines[3],"%s %d","Startup count: ",bootupsCount);
}
void showInfo_Firmware_Sub_1(sScreenData*screenData){
  uint32_t currentRunTimeMS=0;
  uint8_t runTimeDays=0;
  uint8_t runTimeHours=0;
  uint8_t runTimeMinutes=0;
  uint8_t runTimeSeconds=0;

  currentRunTimeMS=xTaskGetTickCount()*portTICK_PERIOD_MS;
  runTimeDays=currentRunTimeMS/86400;
  runTimeHours=currentRunTimeMS/3600%24;
  runTimeMinutes=currentRunTimeMS/60%60;
  runTimeSeconds=currentRunTimeMS%60;
  
  sprintf(screenData->lines[0],"%s","Drink Creator 6000");
  sprintf(screenData->lines[1],"%s","Current run time");  
  
  sprintf(screenData->lines[2],"%02d %s %02d %s",runTimeDays,"days",runTimeHours,"h");
  

  memset((void*)screenData->lines[3],0,sizeof(screenData->lines[3]));
  
  sprintf(screenData->lines[3],"%02d %s  %02d %s",runTimeMinutes,"min",runTimeSeconds,"s");
}
void showInfo_Temp_Sub_0(sScreenData*screenData){
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
  
  //sprintf(screenData->lines[3],"Status: %s",digitalRead(Pelt1Pin)==HIGH?"Cooling":"Idle");
}
void showInfo_Memory_Sub_N(sScreenData*screenData,volatile sUIContext*UI_context){
    updateMemoryUsage();  
  
    if(UI_context->currentSubMenu==4)
      UI_context->currentSubMenu=0;
    if(UI_context->currentSubMenu>4)
      UI_context->currentSubMenu=3;

    memset((void*)screenData,0,sizeof(sScreenData));
  
    if(UI_context->currentSubMenu==0){
	    // Free RAM
	    char buffer[13]{};
    
	  uint8_t ram_percent=100*uint32_t(ram_in_use)/ram_size;  
    
    sprintf(screenData->lines[0],"%s","RAM & CPU Info");	  
	  memset(buffer,'-',sizeof(buffer)-1);
	  buffer[0]='[';
	  buffer[11]=']';
	  memset((void*)(buffer+1),'#',ram_percent/10);
    sprintf(screenData->lines[1],"%s %4u B/%4u B","Usage:",ram_in_use,ram_size);
    sprintf(screenData->lines[2],"%12s %3u %%",buffer,ram_percent);
    uint8_t cpuLoad=100-((idleCounterPerSecond*100)/idleCalib);
    sprintf(screenData->lines[3],"CPU: %3d%%",cpuLoad);
    return;
  }
  
  sprintf(screenData->lines[0],"%s","RAM Info");
  
  if(UI_context->currentSubMenu==1){
	  //.data segment
    sprintf(screenData->lines[1],".data: 0x%04X-0x%04X",(uint16_t)&__data_start,(uint16_t)&__data_end);
    //.bss segment
    sprintf(screenData->lines[2],".bss:  0x%04X-0x%04X",(uint16_t)&__bss_start,(uint16_t)&__bss_end);
	  //Size of each memory segment
    sprintf(screenData->lines[3],"Size:  %4uB %4uB",(uint16_t)&__data_end-(uint16_t)&__data_start,(uint16_t)&__bss_end-(uint16_t)&__bss_start);
  }
  else if(UI_context->currentSubMenu==2){
    ///.tdata segment
    sprintf(screenData->lines[1],".tdat: 0x%04X-0x%04X",(uint16_t)&__tdat_start,(uint16_t)&__tdat_end);
    //Size of each memory segment
    sprintf(screenData->lines[3],"Size:  %4uB",__tdat_size);    
  }
  else if(UI_context->currentSubMenu==3){
	  //heap segment
    sprintf(screenData->lines[1],"HEAP:  0x%04X-0x%04X",(uint16_t)&__heap_start,(uint16_t)__heap_end);
	  //stack segment	
    sprintf(screenData->lines[2],"STACK: 0x%04X-0x%04X",(uint16_t)__stack_ptr,RAMEND);
	  //Size of each memory segment
    sprintf(screenData->lines[3],"Size:  %4uB %4uB",__heap_size,__stack_size);	  
  }
}
void showInfo_Task_Sub_N(sScreenData*screenData,volatile sUIContext*UI_context){
  if(UI_context->currentSubMenu==TASK_N)
    UI_context->currentSubMenu=0;
  if(UI_context->currentSubMenu>TASK_N)
    UI_context->currentSubMenu=TASK_N-1;

  sprintf(screenData->lines[0],"%s","Task Info");
  
  if(taskHandles[UI_context->currentSubMenu]==NULL){
	  sprintf(screenData->lines[1],"%s%dCorrupted","Task id=",UI_context->currentSubMenu);
  }
  else{
	  char buffer[10]{};
	  TaskStatus_t taskStatus{};
	  vTaskGetInfo(taskHandles[UI_context->currentSubMenu],&taskStatus,pdTRUE,eInvalid);
	
    sprintf(screenData->lines[1],"[%02u]%s",UI_context->currentSubMenu,taskStatus.pcTaskName);
	  sprintf(screenData->lines[2],"Highwater mark: %3u",taskStatus.usStackHighWaterMark);
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
	  memcpy((void*)(screenData->lines[3]+11),(void*)buffer,9);
  }
}
void showInfo_Error_Sub_N(sScreenData*screenData,volatile sUIContext*UI_context){
  //Global struct with error info 
  //Or use this EEPROMGetLastStartupError()
  uint8_t errorTextLength=strlen(lastSystemError.errorText);
  
  if(UI_context->currentSubMenu>=(int16_t)errorTextLength-LCD_WIDTH+1)
    UI_context->currentSubMenu=0;
  
  memcpy((void*)screenData->lines[0],(void*)(lastSystemError.errorText+UI_context->currentSubMenu),LCD_WIDTH);
  
  strncpy(screenData->lines[1],"Fault time signature",LCD_WIDTH);
  sprintf(screenData->lines[2],"%02d days %02d h",lastSystemError.days,lastSystemError.hours);

  sprintf(screenData->lines[3],"%02d min  %02d s",lastSystemError.minutes,lastSystemError.seconds);
  
  if(errorTextLength>LCD_WIDTH){
	  if(UI_context->autoScrollEnable==1)
	    UI_context->currentSubMenu++;	  
  }
}
void showInfo_ConfError_Sub_0(sScreenData*screenData,volatile sUIContext*UI_context){
  if(lastSystemError.confirmed==1){
    sprintf(screenData->lines[0],"%s","Error confirmed");
    sprintf(screenData->lines[1],"%s","EEPROM Updated");
  }
  else{
    sprintf(screenData->lines[0],"%s","Error not confirmed");
  }
  
  // Mechanism for confirming errors, but logic shouldnt be in this function
  // Logic of course should be in the main task
}
void taskShowSystemInfo(void*pvParameters){
  uint32_t f_run=false;
  
  sScreenData screenData{};
  for(;;){
    if(xTaskNotifyWait(0,0,&f_run,0)>0){
      if(f_run==1){

      }
    }
	  if(f_run==1){
      memset((void*)&screenData,0,sizeof(screenData));
	    // Context will be managed by main task
      if(UI_Context.currentMenu>5)
        UI_Context.currentMenu=0;
        
	    switch(UI_Context.currentMenu){
	      case 0:
        if(UI_Context.currentSubMenu==2)
          UI_Context.currentSubMenu=0;
        if(UI_Context.currentSubMenu>2)
          UI_Context.currentSubMenu=1;
		    switch(UI_Context.currentSubMenu){
		      case 0:
			      showInfo_Firmware_Sub_0(&screenData);
			    break;
			    case 1:
			      showInfo_Firmware_Sub_1(&screenData);
			    break;
		    }
		  break;
		  case 1:
			  showInfo_Temp_Sub_0(&screenData);
		  break;
		  case 2:
		    showInfo_Memory_Sub_N(&screenData,&UI_Context);
		  break;
		  case 3:
		    showInfo_Task_Sub_N(&screenData,&UI_Context);
		  break;
		  case 4:
        UI_Context.autoScrollEnable=1;
		    showInfo_Error_Sub_N(&screenData,&UI_Context);
		  break;
		  case 5:
		    showInfo_ConfError_Sub_0(&screenData,&UI_Context);
		  break;
	    }
	    xQueueSend(qScreenData,&screenData,pdMS_TO_TICKS(50));
	  }
  vTaskDelay(pdMS_TO_TICKS(TASK_SHOW_SYSTEM_INFO_REFRESH_RATE));
  }
}
