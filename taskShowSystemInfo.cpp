#include "taskShowSystemInfo.h"

void showInfo_Firmware_Sub_0(sScreenData*screenData){
  sprintf(screenData->lines[0],"%s","Drink Creator 6000");
  sprintf(screenData->lines[1],"%s","Software ver. 3.0");
  sprintf(screenData->lines[2],"%s","Author: Alan Kudelko");
  //sprintf(screenData->lines[3],"%s %d","Startup count: ",*(uint16_t*)pvParameters); //Fix
}
void showInfo_Firmware_Sub_1(sScreenData*screenData){
  uint64_t runTimeFromMillis=0;
  uint8_t runTimeDays=0;
  uint8_t runTimeHours=0;
  uint8_t runTimeMinutes=0;
  uint8_t runTimeSeconds=0;

  runTimeFromMillis=millis()/1000;
  runTimeDays=runTimeFromMillis/86400;
  runTimeHours=runTimeFromMillis/3600%24;
  runTimeMinutes=runTimeFromMillis/60%60;
  runTimeSeconds=runTimeFromMillis%60;
  
  sprintf(screenData->lines[0],"%s","Drink Creator 6000");
  sprintf(screenData->lines[1],"%s","Current run time");  
  
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
  
  sprintf(screenData->lines[3],"Status: %s",digitalRead(Pelt1Pin)==HIGH?"Cooling":"Idle");
}
void showInfo_Memory_Sub_N(sScreenData*screenData,sUIContext*UI_Context){
  updateMemoryUsage();  
  
  if(UI_Context->currentSubMenu>2)
    UI_Context->currentSubMenu=0;

  memset(screenData,0,sizeof(sScreenData));
  
  sprintf(screenData->lines[0],"%s","RAM Info");
  
  if(UI_Context->currentSubMenu==0){
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
  else if(UI_Context->currentSubMenu==1){
	//.data sector
    sprintf(screenData->lines[1],".data: 0x%4X-0x%4X",uint16_t(&__data_start),uint16_t(&__data_end));
    //.bss sector
    sprintf(screenData->lines[2],".bss:  0x%4X-0x%4X",uint16_t(&__bss_start),uint16_t(&__bss_end));
	//Size of each memory segment
    sprintf(screenData->lines[3],"Size:  %4uB %4uB",uint16_t(&__data_end)-uint16_t(&__data_start),uint16_t(&__bss_end)-uint16_t(&__bss_start));
  }
  else if(UI_Context->currentSubMenu==2){
	//heap sector
    sprintf(screenData->lines[1],"HEAP: 0x%4X-0x%4X",uint16_t(&__heap_start),heap_end);
	//stack sector	
    sprintf(screenData->lines[2],"STACK:  0x%4X-0x%4X",uint16_t(stack_ptr),RAMEND);
	//Size of each memory segment
    sprintf(screenData->lines[3],"Size:  %4uB %4uB",heap_size,stack_size);	  
  }
}
void showInfo_Task_Sub_N(sScreenData*screenData,sUIContext*UI_Context){
  if(UI_Context->currentSubMenu>=TASK_N)
    UI_Context->currentSubMenu=0;

  sprintf(screenData->lines[0],"%s","Task Info");
  
  if(taskHandles[UI_Context->currentSubMenu]==NULL){
	sprintf(screenData->lines[1],"%s%dCorrupted","Task id=",UI_Context->currentSubMenu);
  }
  else{
	char buffer[10]{};
	TaskStatus_t taskStatus{};
	vTaskGetInfo(taskHandles[UI_Context->currentSubMenu],&taskStatus,pdTRUE,eInvalid);
	
    sprintf(screenData->lines[1],"[%2u]%s",UI_Context->currentSubMenu,taskStatus.pcTaskName);
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
}
void showInfo_Error_Sub_N(sScreenData*screenData,sUIContext*UI_Context){
  //Global struct with error info 
  //Or use this EEPROMGetLastStartupError()
  int8_t errorTextLength=strlen(lastSystemError.errorText);
  
  if(UI_Context->currentSubMenu>=(int16_t)errorTextLength-LCD_WIDTH)
    UI_Context->currentSubMenu=0;
  
  memcpy(screenData->lines[0],lastSystemError.errorText+UI_Context->currentSubMenu,1);
  
  strncpy(screenData->lines[1],"Fault time signature",LCD_WIDTH);
  sprintf(screenData->lines[2],"%2d days %2d h",lastSystemError.days,lastSystemError.hours);
  if(lastSystemError.days<10)
    screenData->lines[2][0]='0';
  if(lastSystemError.hours<10)
    screenData->lines[2][8]='0';

  sprintf(screenData->lines[3],"%2d min  %d s",lastSystemError.minutes,lastSystemError.seconds);
  if(lastSystemError.minutes<10)
    screenData->lines[3][0]='0';
  if(lastSystemError.seconds<10)
    screenData->lines[3][8]='0';
  
  if(errorTextLength>LCD_WIDTH){
	if(UI_Context->autoScrollEnable==1)
	  UI_Context->currentSubMenu++;	  
  }
}
void showInfo_ConfError_Sub_0(sScreenData*screenData,sUIContext*UI_Context){
  sprintf(screenData->lines[0],"%s","Error confirmed");
  sprintf(screenData->lines[1],"%s","EEPROM Updated");
  
  // Mechanism for confirming errors, but logic shouldnt be in this function
  // Logic of course should be in the main task
}
void taskShowSystemInfo(void*pvParameters){
  // pvParameters is type sUiContext
  
  sUIContext*UI_context=&UI_Context;
  bool f_run=true;
  
  sScreenData screenData{};
  for(;;){
    if(ulTaskNotifyTake(pdTRUE,0)>0){
	  //while(xQueueReceive(qShowInfoId,&showInfoScreenId,pdMS_TO_TICKS(100))==pdPASS);
	  
      f_run=false;
    }
	//if(xQueueReceive(qShowInfoId,&showInfoScreenId,pdMS_TO_TICKS(50))==pdPASS){
	//  f_run=true;
	//}
	if(f_run){
      memset(&screenData,0,sizeof(screenData));
	  // Context will be managed by main task
	  switch(UI_context->currentMenu){
	    case 0:
		  switch(UI_context->currentSubMenu){
		    case 0:
			  showInfo_Firmware_Sub_0(&screenData);
			break;
			case 1:
			  showInfo_Firmware_Sub_1(&screenData);
			break;
		  }
		break;
		case 1:
		  switch(UI_context->currentSubMenu){
		    case 0:
			  showInfo_Temp_Sub_0(&screenData);
			break;
		  }
		break;
		case 2:
		  showInfo_Memory_Sub_N(&screenData,UI_context);
		break;
		case 3:
		  showInfo_Task_Sub_N(&screenData,UI_context);
		break;
		case 4:
		  showInfo_Error_Sub_N(&screenData,UI_context);
		break;
		case 5:
		  showInfo_ConfError_Sub_0(&screenData,UI_context);
		break;
	  }
	  
	  xQueueSend(qScreenData,&screenData,pdMS_TO_TICKS(50));
	}
  vTaskDelay(pdMS_TO_TICKS(TASK_SHOW_INFO_REFRESH_RATE));
  }
}