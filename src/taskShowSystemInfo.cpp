/**
 * @file taskShowSystemInfo.cpp
 * @brief Implementation of taskShowSystemInfo.h
 * 
 * @author Alan Kudełko
 * @copyright
 * Copyright (c) 2025 Alan Kudełko.  
 * All rights reserved.  
 * For educational and research purposes only.  
 * Redistribution, modification, or commercial use prohibited without
 * explicit written permission.
 */

#include <stdio.h>
#include <string.h>

#include <taskShowSystemInfo.h>
#include <DrinkCreator6000_RamStats.h>
#include <DrinkCreator6000_Progmem.h>

#include <DrinkCreator6000_Config_C.h>

extern "C" void updateMemoryUsage(void);

uint8_t taskShowSystemInfo_ProcessContext(uint8_t*keyboardData,volatile struct sUIContext*UI_context){
    // Fix to be MISRA C compliant
    // Magic numbers should be replaced with defines or enums
    // Default case should be added to switch statements
    if((*keyboardData&E_LWHITE_BUTTON)==E_LWHITE_BUTTON){
        UI_context->currentSubMenu--;

        switch(UI_context->currentMenu){
            case UI_SHOW_INFO_MENU_FIRMWARE:
                if(UI_context->currentSubMenu>1){
                    UI_context->currentSubMenu=0;
                }
            break;
            case UI_SHOW_INFO_MENU_TEMP:
                if(UI_context->currentSubMenu>0){
                    UI_context->currentSubMenu=0;
                }
            break;
            case UI_SHOW_INFO_MENU_MEMORY:
                if(UI_context->currentSubMenu>4){
                    UI_context->currentSubMenu=0;
                }
            break;
            case UI_SHOW_INFO_MENU_TASK:
                if(UI_context->currentSubMenu>TASK_N-1){
                    UI_context->currentSubMenu=0;
                }
            break;
            case UI_SHOW_INFO_MENU_ERROR:
                UI_context->currentSubMenu=0; // Switch is only possible when confirming an error
            break;
            default:
                UI_context->currentSubMenu=0; // Temporary do nothing
        }
        return 0;
    }
    else if((*keyboardData&E_BLUE_BUTTON)==E_BLUE_BUTTON){
        UI_context->currentMenu++;
        if(UI_context->currentMenu>UI_SHOW_INFO_MENUS_COUNT){
            taskENTER_CRITICAL();
            UI_context->currentTask++;
            UI_context->currentMenu=0;
            UI_context->currentSubMenu=0;
            taskEXIT_CRITICAL();
            return 1; // Change task
        }
    }
    else if((*keyboardData&E_RED_BUTTON)==E_RED_BUTTON){
        taskENTER_CRITICAL();
        UI_context->currentMenu--;
        UI_context->currentSubMenu=0;
        taskEXIT_CRITICAL();
        if(UI_context->currentMenu>UI_SHOW_INFO_MENUS_COUNT){
            taskENTER_CRITICAL();
            UI_context->currentTask-=2;
            UI_context->currentMenu=0;
            taskEXIT_CRITICAL();
            return 1; // Change task
        }
    }
    else if((*keyboardData&E_GREEN_BUTTON)==E_GREEN_BUTTON){
        if(UI_context->currentMenu==UI_SHOW_INFO_MENU_ERROR){
            // Update EEPROM
            f_errorConfirmed=1;
            taskENTER_CRITICAL();
            UI_context->currentSubMenu=0;
            taskEXIT_CRITICAL();
        }
        return 0;
    }
    else{
        return 0;
    }
    return 0;
}

void showInfo_Firmware_Sub_0(sScreenData*screenData){
    strncpy_P(screenData->lines[0],msg_showInfo_ProjectName,LCD_WIDTH);
    strncpy_P(screenData->lines[1],msg_showInfo_SoftwareVersion,LCD_WIDTH);
    strncpy_P(screenData->lines[2],msg_showInfo_Author,LCD_WIDTH);
    strncpy_P(screenData->lines[3],msg_showInfo_StartupCount,LCD_WIDTH);
    snprintf(screenData->lines[3]+strlen_P(msg_showInfo_StartupCount),LCD_WIDTH-strlen_P(msg_showInfo_StartupCount),"%d",bootupsCount);
}
void showInfo_Firmware_Sub_1(sScreenData*screenData){
    uint32_t currentRunTimeMS=0;
    uint8_t runTimeDays=0;
    uint8_t runTimeHours=0;
    uint8_t runTimeMinutes=0;
    uint8_t runTimeSeconds=0;

    currentRunTimeMS=xTaskGetTickCount()/1000;
    runTimeDays=currentRunTimeMS/86400;
    runTimeHours=(currentRunTimeMS/3600)%24;
    runTimeMinutes=(currentRunTimeMS/60)%60;
    runTimeSeconds=currentRunTimeMS%60;
  
    strncpy_P(screenData->lines[0],msg_showInfo_ProjectName,LCD_WIDTH);
    strncpy_P(screenData->lines[1],msg_showInfo_CurrentRunTime,LCD_WIDTH);  
  
    sprintf(screenData->lines[2],"%02d %s %02d %s",runTimeDays,"days",runTimeHours,"h");
  
    memset((void*)screenData->lines[3],0,sizeof(screenData->lines[3]));
  
    sprintf(screenData->lines[3],"%02d %s  %02d %s",runTimeMinutes,"min",runTimeSeconds,"s");
}
void showInfo_Temp_Sub_0(sScreenData*screenData){
    // sprintf with %f is disabled on AVR, because it requires extra code
    uint8_t mantissa;
    uint8_t integer;
    strncpy_P(screenData->lines[0],msg_showInfo_ProjectName,LCD_WIDTH);
  
    integer=currentTemperature;
    mantissa=uint8_t(currentTemperature*10)%10;
    sprintf(screenData->lines[1],"T: %2d.%d\xDF""C S:",integer,mantissa);
  
    integer=setTemperature;
    mantissa=uint8_t(setTemperature*10)%10;
    sprintf(screenData->lines[1]+14,"%2d.%d\xDF""C",integer,mantissa);
  
    integer=temperatureHysteresis;
    mantissa=uint8_t(temperatureHysteresis*10)%10;
    sprintf(screenData->lines[2],"Hyst: %d.%d\xDF""C",integer,mantissa);
  
    //Dobra tutaj dodac abstrakcje odczytu stanu lodowki
    //sprintf(screenData->lines[3],"Status: %s",digitalRead(P)==HIGH?"Cooling":"Idle");
}
void showInfo_Memory_Sub_N(sScreenData*screenData,volatile sUIContext*UI_context){
    updateMemoryUsage();  

    memset((void*)screenData,0,sizeof(sScreenData));
  
    if(UI_context->currentSubMenu==0){
	    // Free RAM
	    char buffer[13]{};
    
	    uint8_t ram_percent=100*uint32_t(ram_in_use)/ram_size;  
    
        sprintf(screenData->lines[0],"%s","RAM Info");	  
	    memset(buffer,'-',sizeof(buffer)-1);
	    buffer[0]='[';
	    buffer[11]=']';
	    memset((void*)(buffer+1),'#',ram_percent/10);
        sprintf(screenData->lines[1],"%s %4u B/%4u B","Usage:",ram_in_use,ram_size);
        sprintf(screenData->lines[2],"%12s %3u %%",buffer,ram_percent);
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
        //.tdata segment
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
  
    if(UI_context->currentSubMenu>=(int16_t)errorTextLength-LCD_WIDTH+1){
        UI_context->currentSubMenu=0;
    }
  
    memcpy((void*)screenData->lines[0],(void*)(lastSystemError.errorText+UI_context->currentSubMenu),LCD_WIDTH);
  
    strncpy(screenData->lines[1],"Fault time signature",LCD_WIDTH);
    sprintf(screenData->lines[2],"%02d days %02d h",lastSystemError.days,lastSystemError.hours);

    sprintf(screenData->lines[3],"%02d min  %02d s",lastSystemError.minutes,lastSystemError.seconds);
  
    if(errorTextLength>LCD_WIDTH){
	    if(UI_context->autoScrollEnable==1){
	        UI_context->currentSubMenu++;
        }
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
    uint32_t f_run=0;
    uint8_t keyboardData=0;
    uint8_t changeTask=0; // 1 if there is a change of task, 0 if not
  
    sScreenData screenData{};
    for(;;){
        if(xTaskNotifyWait(0,0,&f_run,0)>0){
            if(f_run==1){
                // In theory this is onWakeup event
            }
        }
	    if(f_run==1){
            memset((void*)&screenData,0,sizeof(screenData));
	        // Context will be managed by main task
            if(UI_Context.currentMenu>5){
                UI_Context.currentMenu=0;
            }
        
	        switch(UI_Context.currentMenu){
	            case UI_SHOW_INFO_MENU_FIRMWARE:
                    if(UI_Context.currentSubMenu==2){
                        UI_Context.currentSubMenu=0;
                    }
                    if(UI_Context.currentSubMenu>2){
                        UI_Context.currentSubMenu=1;
                    }
		            switch(UI_Context.currentSubMenu){
		                case 0:
			                showInfo_Firmware_Sub_0(&screenData);
			                break;
			            case 1:
			                showInfo_Firmware_Sub_1(&screenData);
			            break;
		            }
		            break;
		        case UI_SHOW_INFO_MENU_TEMP:
			        showInfo_Temp_Sub_0(&screenData);
		            break;
		        case UI_SHOW_INFO_MENU_MEMORY:
		            showInfo_Memory_Sub_N(&screenData,&UI_Context);
		            break;
		        case UI_SHOW_INFO_MENU_TASK:
		            showInfo_Task_Sub_N(&screenData,&UI_Context);
		            break;
		        case UI_SHOW_INFO_MENU_ERROR:
                    UI_Context.autoScrollEnable=1;
		            showInfo_Error_Sub_N(&screenData,&UI_Context);
		            break;
		        case 5:
		            showInfo_ConfError_Sub_0(&screenData,&UI_Context);
		            break;
	        }

            if(xQueueReceive(qKeyboardData,&keyboardData,0)==pdTRUE){
                changeTask=taskShowSystemInfo_ProcessContext(&keyboardData,&UI_Context);
                if(changeTask==1){
                    xTaskNotify(taskHandles[TASK_SHOW_SYS_INFO],0,eSetValueWithOverwrite); // Notify self to terminate
                }
                else{
                    xTaskNotify(taskHandles[TASK_SHOW_SYS_INFO],1,eSetValueWithOverwrite); // Notify self to process keyboard data
                }
            }
            xTaskNotify(taskHandles[TASK_MAIN],changeTask,eSetValueWithOverwrite); // Notify main to change context
            xQueueSend(qScreenData,&screenData,pdMS_TO_TICKS(50));
        }
        vTaskDelay(pdMS_TO_TICKS(TASK_SHOW_SYSTEM_INFO_REFRESH_RATE));
    }
}
