#include <LiquidCrystal_AIP31068_I2C.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <Wire.h>
#include "LcdCharacters.h"
#include "CustomDataTypes.h"
#include "EEPROM_Management.h"

//enum {DSPin=PIN_PC0, STPin=PIN_PC1, SHPin=PIN_PC2,OEnable=PIN_PC3,INTPin=PIN_PD2,THERMOMETER_PIN=PIN_PD3,Pelt1Pin=PIN_PD4,Pelt2Pin=PIN_PD5};
enum{INTPin=19,Pelt1Pin=9,Pelt2Pin=8};
enum{E_WELCOME=0,E_SELECTION=1,E_SHOW_INFO=2,E_TEMP_INFO=3,E_ORDER_DRINK=4,E_TEST_PUMPS=5};
enum{E_LOADING_BAR=17};
enum{E_GREEN_BUTTON=1,E_LWHITE_BUTTON=2,E_RWHITE_BUTTON=4,E_BLUE_BUTTON=8,E_RED_BUTTON=16};

enum{LCD_WIDTH=16,LCD_HEIGHT=2};

static LiquidCrystal_AIP31068_I2C lcd(0x3E, LCD_WIDTH, LCD_HEIGHT);

const static sDrinkData drink[20]={
  {"Raz", 50, 0, 0, 0, 0, 0, 0, 0, 0},                   //1
  {"Dwa", 200, 200, 0, 0, 0, 0, 0, 0, 0},                //2
  {"Trzy", 50, 100, 150, 200, 250, 300, 350, 400, 450},  //3
  {"Trzy", 50, 100, 150, 200, 250, 300, 350, 400, 450},  //4
  {"Trzy", 50, 100, 150, 200, 250, 300, 350, 400, 450},  //5
  {"Trzy", 50, 100, 150, 200, 250, 300, 350, 400, 450},  //6
  {"Trzy", 50, 100, 150, 200, 250, 300, 350, 400, 450},  //7
  {"Trzy", 50, 100, 150, 200, 250, 300, 350, 400, 450},  //8
  {"Trzy", 50, 100, 150, 200, 250, 300, 350, 400, 450},  //9
  {"Trzy", 50, 100, 150, 200, 250, 300, 350, 400, 450},  //10
  {"Trzy", 50, 100, 150, 200, 250, 300, 350, 400, 450},  //11
  {"Trzy", 50, 100, 150, 200, 250, 300, 350, 400, 450},  //12
  {"Trzy", 50, 100, 150, 200, 250, 300, 350, 400, 450},  //13
  {"Trzy", 50, 100, 150, 200, 250, 300, 350, 400, 450},  //14
  {"Trzy", 50, 100, 150, 200, 250, 300, 350, 400, 450},  //15
  {"Trzy", 50, 100, 150, 200, 250, 300, 350, 400, 450},  //16
  {"Trzy", 50, 100, 150, 200, 250, 300, 350, 400, 450},  //17
  {"Trzy", 50, 100, 150, 200, 250, 300, 350, 400, 450},  //18
  {"Trzy", 50, 100, 150, 200, 250, 300, 350, 400, 450},  //19
  {"Trzy", 50, 100, 150, 200, 250, 300, 350, 400, 450}   //20
};
const static char ingredients[8][LCD_WIDTH-4-4]{
  {"Whiskey"},    //1
  {"Jager"},      //2
  {"Rum"},        //3
  {"Wodka"},      //4
  {"Cola"},       //5
  {"Sok pom"},    //6
  {"Sok cyt"},    //7
  {"Woda"}        //8
};
// Each pump pumps at different rate
const static uint8_t pumpsEff[8]{
  200,
  200,
  200,
  200,
  200,
  200,
  200,
  200
};
// Flow control variables
enum{SCREEN_QUEUE_BUFFER_COUNT=6,
	 KEYBOARD_QUEUE_BUFFER_COUNT=2,
	 DRINK_ID_QUEUE_BUFFER_COUNT=2,
	 SHOW_INFO_QUEUE_BUFFER_COUNT=2,
	 ERROR_ID_QUEUE_BUFFER_COUNT=1
};

static uint8_t screenQueueBuffer[SCREEN_QUEUE_BUFFER_COUNT*sizeof(sScreenData)];
static uint8_t keyboardQueueBuffer[KEYBOARD_QUEUE_BUFFER_COUNT*sizeof(uint8_t)];
static uint8_t drinkIdQueueBuffer[DRINK_ID_QUEUE_BUFFER_COUNT*sizeof(uint8_t)];
static uint8_t showInfoQueueBuffer[SHOW_INFO_QUEUE_BUFFER_COUNT*sizeof(uint8_t)];
static uint8_t errorIdQueueBuffer[ERROR_ID_QUEUE_BUFFER_COUNT*sizeof(TaskHandle_t )];

static StaticQueue_t screenQueueStructBuffer{};
static StaticQueue_t keyboardQueueStructBuffer{};
static StaticQueue_t drinkIdQueueStructBuffer{};
static StaticQueue_t showInfoQueueStructBuffer{};
static StaticQueue_t errorIdQueueStructBuffer{};

static StaticSemaphore_t semReadDataBuffer{};
static StaticSemaphore_t muxI2CLockBuffer{};
static StaticSemaphore_t muxSerialLockBuffer{};

static QueueHandle_t qScreenData{};
static QueueHandle_t qKeyboardData{};
static QueueHandle_t qDrinkId{};
static QueueHandle_t qShowInfoId{};
static QueueHandle_t qErrorId{};

static SemaphoreHandle_t sem_ReadData{};
static SemaphoreHandle_t mux_I2CLock{};
static SemaphoreHandle_t mux_SerialLock{};
// Flow control variables

// Task variables
// Stack size will need tuning in last release
enum{TASK_ERROR_HANDLER_STACK_SIZE=192};   //0
enum{TASK_STACK_DEBUGGER_STACK_SIZE=192};  //1
enum{TASK_MAIN_STACK_SIZE=192};            //2
enum{UPDATE_SCREEN_STACK_SIZE=180};        //3
enum{TASK_REGULATE_TEMP_STACK_SIZE=128};   //4
enum{TASK_READ_INPUT_STACK_SIZE=128};      //5
enum{TASK_SELECT_DRINK_STACK_SIZE=192};    //6
enum{TASK_ORDER_DRINK_STACK_SIZE=320};     //7
enum{TASK_SHOW_INFO_STACK_SIZE=192};       //8
enum{TASK_SHOW_TEMP_STACK_SIZE=192};       //9
enum{TASK_SHOW_LAST_ERROR_STACK_SIZE=200}; //10

enum{TASK_ERROR_HANDLER=0,
	 TASK_STACK_DEBUGGER=1,
	 TASK_MAIN=2,
	 TASK_UPDATE_SCREEN=3,
	 TASK_REGULATE_TEMP=4,
	 TASK_READ_INPUT=5,
	 TASK_SELECT_DRINK=6,
	 TASK_ORDER_DRINK=7,
	 TASK_SHOW_INFO=8,
	 TASK_SHOW_TEMP=9,
	 TASK_SHOW_LAST_ERROR=10};

enum{GUARD_ZONE_SIZE=32};
	 
enum{TASK_N=11}; // Zmiana

static StaticTask_t errorHandlerTCB{};                                   //0
static StaticTask_t stackDebuggerTCB{};                                  //1
static StaticTask_t mainTCB{};                                           //2
static StaticTask_t updateScreenTCB{};                                   //3
static StaticTask_t regulateTempTCB{};                                   //4
static StaticTask_t readInputTCB{};                                      //5
static StaticTask_t selectDrinkTCB{};                                    //6
static StaticTask_t orderDrinkTCB{};                                     //7
static StaticTask_t showInfoTCB{};                                       //8
static StaticTask_t showTempTCB{};                                       //9
static StaticTask_t showLastErrorTCB{};                                  //10

static TaskHandle_t taskHandles[TASK_N]{};

static StackType_t errorHandlerStack[TASK_ERROR_HANDLER_STACK_SIZE]{};   //0
static StackType_t guardZone0[GUARD_ZONE_SIZE]{};
static StackType_t stackDebuggerStack[TASK_STACK_DEBUGGER_STACK_SIZE]{}; //1
static StackType_t guardZone1[GUARD_ZONE_SIZE]{};
static StackType_t mainStack[TASK_MAIN_STACK_SIZE]{};                    //2
static StackType_t guardZone2[GUARD_ZONE_SIZE]{};
static StackType_t updateScreenStack[UPDATE_SCREEN_STACK_SIZE]{};        //3
static StackType_t guardZone3[GUARD_ZONE_SIZE]{};
static StackType_t regulateTempStack[TASK_REGULATE_TEMP_STACK_SIZE]{};   //4
static StackType_t guardZone4[GUARD_ZONE_SIZE]{};
static StackType_t readInputStack[TASK_READ_INPUT_STACK_SIZE]{};         //5
static StackType_t guardZone5[GUARD_ZONE_SIZE]{};
static StackType_t selectDrinkStack[TASK_SELECT_DRINK_STACK_SIZE]{};     //6
static StackType_t guardZone6[GUARD_ZONE_SIZE]{};
static StackType_t orderDrinkStack[TASK_ORDER_DRINK_STACK_SIZE]{};       //7
static StackType_t guardZone7[GUARD_ZONE_SIZE]{};
static StackType_t showInfoStack[TASK_SHOW_INFO_STACK_SIZE]{};           //8
static StackType_t guardZone8[GUARD_ZONE_SIZE]{};
static StackType_t showTempStack[TASK_SHOW_TEMP_STACK_SIZE]{};           //9
static StackType_t guardZone9[GUARD_ZONE_SIZE]{};
static StackType_t showLastErrorStack[TASK_SHOW_LAST_ERROR_STACK_SIZE]{};//10
static StackType_t guardZone10[GUARD_ZONE_SIZE]{};
// Add guardzones between variables for potential safe overwrite
// Task variables

// Global variables
static uint8_t f_errorConfirmed=0;;
static uint16_t bootupsCount=0;
static sSystemError lastSystemError{};

static float currentTemperature=10.0f;
static float setTemperature=4.0f;
static float temperatureHysteresis=1.0f;

static volatile bool f_enableISR=true;
// Global variables
void initializeMemory(){
  qScreenData=xQueueCreateStatic(SCREEN_QUEUE_BUFFER_COUNT,sizeof(sScreenData),screenQueueBuffer,&screenQueueStructBuffer);
  qKeyboardData=xQueueCreateStatic(KEYBOARD_QUEUE_BUFFER_COUNT,sizeof(uint8_t),keyboardQueueBuffer,&keyboardQueueStructBuffer);
  qDrinkId=xQueueCreateStatic(DRINK_ID_QUEUE_BUFFER_COUNT,sizeof(uint8_t),drinkIdQueueBuffer,&drinkIdQueueStructBuffer);
  qShowInfoId=xQueueCreateStatic(SHOW_INFO_QUEUE_BUFFER_COUNT,sizeof(uint8_t),showInfoQueueBuffer,&showInfoQueueStructBuffer);
  qErrorId=xQueueCreateStatic(ERROR_ID_QUEUE_BUFFER_COUNT,sizeof(TaskHandle_t),errorIdQueueBuffer,&errorIdQueueStructBuffer);
  
  sem_ReadData=xSemaphoreCreateBinaryStatic(&semReadDataBuffer);
  mux_I2CLock=xSemaphoreCreateMutexStatic(&muxI2CLockBuffer);
  mux_SerialLock=xSemaphoreCreateMutexStatic(&muxSerialLockBuffer);
  
  memset(guardZone0,0xF,GUARD_ZONE_SIZE);
  memset(guardZone1,0xF,GUARD_ZONE_SIZE);
  memset(guardZone2,0xF,GUARD_ZONE_SIZE);
  memset(guardZone3,0xF,GUARD_ZONE_SIZE);
  memset(guardZone4,0xF,GUARD_ZONE_SIZE);
  memset(guardZone5,0xF,GUARD_ZONE_SIZE);
  memset(guardZone6,0xF,GUARD_ZONE_SIZE);
  memset(guardZone7,0xF,GUARD_ZONE_SIZE);
  memset(guardZone8,0xF,GUARD_ZONE_SIZE);
  memset(guardZone9,0xF,GUARD_ZONE_SIZE);
  memset(guardZone10,0xF,GUARD_ZONE_SIZE);
}
void ram_dump(){
  char buffer[6]{};
  extern uint16_t __data_start;
  extern uint16_t __data_end;

  extern uint16_t __bss_start;
  extern uint16_t __bss_end;

  extern uint16_t __heap_start;
  extern void *__brkval;

  void* heap_end=__brkval?__brkval:(void*)&__heap_start;
  uint8_t* stack_ptr=(uint8_t*)SP;

  uint16_t heap_size=(uint16_t)heap_end-(uint16_t)&__heap_start;
  uint16_t stack_size=(uint16_t)RAMEND-(uint16_t)stack_ptr;
  uint16_t total_free=(uint16_t)stack_ptr-(uint16_t)heap_end;
  
  Serial.println(F("[XXXXX]=====[RAM DUMP]=====[XXXXX]"));
  Serial.println(F("[     ]START | END  | SIZE [     ]"));

  sprintf(buffer,"%04X",(uint16_t)&__data_start);
  Serial.print(F("[DATA ]0x")); Serial.print(buffer);
  sprintf(buffer,"%04X",(uint16_t)&__data_end);
  Serial.print(F("|0x")); Serial.print(buffer);
  sprintf(buffer,"%4d",(uint16_t)((uint16_t)&__data_end-(uint16_t)&__data_start));
  Serial.print(F("|")); Serial.print(buffer); Serial.println(F(" B[DATA ]"));

  sprintf(buffer,"%04X",(uint16_t)&__bss_start);
  Serial.print(F("[BSS  ]0x")); Serial.print(buffer);
  sprintf(buffer,"%04X",(uint16_t)&__bss_end);
  Serial.print(F("|0x")); Serial.print(buffer);
  sprintf(buffer,"%4d",(uint16_t)((uint16_t)&__bss_end-(uint16_t)&__bss_start));
  Serial.print(F("|")); Serial.print(buffer); Serial.println(F(" B[BSS  ]"));

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
  sprintf(buffer, "%4d",total_free);
  Serial.print(buffer);
  Serial.println(F(" B[FREE ]"));

  Serial.println(F("[XXXXX]=====[RAM DUMP]=====[XXXXX]"));
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
  Serial.print(F("[#####]Bootups number: "));
  Serial.print(*bootup);
  Serial.println(F("[#####]"));
  Serial.println(F(""));
}
extern "C" void vApplicationStackOverflowHook(TaskHandle_t xTask,char*pcTaskName){
  vTaskResume(taskHandles[TASK_ERROR_HANDLER]);
  xQueueSend(qErrorId,&xTask,pdMS_TO_TICKS(50));  
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
      f_enableISR=false;
    }
}
// Tasks
void taskErrorHandler(void*pvParameters){
	uint8_t i=2;
  uint32_t runTimeFromMillis=0;
  sSystemError lastError{};
  bool f_run=false;
  TaskHandle_t overflowedTask{};
  
  for(;;){
    if(xQueueReceive(qErrorId,&overflowedTask,pdMS_TO_TICKS(50))==pdPASS){
      f_run=true;
      
      sprintf(lastError.errorText,"Stack overflow in task: %s",pcTaskGetName(overflowedTask));
      runTimeFromMillis=millis()/1000;
      lastError.days=runTimeFromMillis/3600/24;
      lastError.hours=runTimeFromMillis/3600%24;
      lastError.minutes=runTimeFromMillis/60%60;
      lastError.seconds=runTimeFromMillis%60;
      lastError.confirmed=false;
      
      for(;i<TASK_N;i++)
        vTaskSuspend(taskHandles[i]);
    }
    if(f_run){
	    Serial.println(F("[####################]====SYSTEM CRITICAL====[##]"));
      Serial.println((const char*)lastError.errorText);
      Serial.println(F("[####################]====SYSTEM CRITICAL====[##]"));
      EEPROMUpdateLastStartupError(&lastError);
      f_run=false;
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
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}
void taskMain(void*pvParameters){
  sScreenData screenData{};
  char var='A'-1;
  bool f_testFlag=false;
  uint8_t counter=0;
  char drinkId=2;
  const uint8_t counterMax=4;
  uint8_t showInfoDebug=0;
  for(;;){
  if(showInfoDebug>1){
    xQueueSend(qErrorId,&taskHandles[TASK_SHOW_TEMP],pdMS_TO_TICKS(100));
    vTaskResume(taskHandles[TASK_ERROR_HANDLER]);
    showInfoDebug=0;
  }
	xQueueSend(qShowInfoId,&showInfoDebug,pdMS_TO_TICKS(100));
	showInfoDebug++;
	
	vTaskDelay(pdMS_TO_TICKS(3000));  
	continue;
    if(!f_testFlag){
      counter++;
      var++;
      if(var=='Z'+1)
        var='A';
            screenData.lines[0][0]=var;
      xQueueSend(qScreenData,&screenData,pdMS_TO_TICKS(50));
    }
    else if(f_testFlag){
      counter--;
      xQueueSend(qDrinkId,&drinkId,pdMS_TO_TICKS(50));
    }
      
    if(counter==counterMax){
      f_testFlag=true;
    }
    if(counter==0){
      f_testFlag=false;
      xTaskNotifyGive(taskHandles[TASK_SELECT_DRINK]);
    }
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
void taskUpdateScreen(void*pvParameters){
  sScreenData receivedLcdData{};

  TickType_t xLastWakeTime=xTaskGetTickCount();
  const TickType_t xFrequency=pdMS_TO_TICKS(300);
  byte i=0;
  byte j=0;
  
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
    vTaskDelayUntil(&xLastWakeTime,xFrequency); 
  }
}
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
    vTaskDelay(5000/portTICK_PERIOD_MS);
  }
}
void taskReadInput(void*pvParameters){
  byte keyboardInput=0;
  byte debounceCounter[25]{};
  uint8_t i=0;
  uint8_t j=0;
  uint8_t buttonSum=0;
  // Ustalmy ze 1 to wcisniety
  TickType_t tickSample{};
  
  for(;;){
    if(xSemaphoreTake(sem_ReadData,pdMS_TO_TICKS(portMAX_DELAY))==pdTRUE){
      if(xSemaphoreTake(mux_I2CLock,pdMS_TO_TICKS(portMAX_DELAY))==pdTRUE){
        memset(debounceCounter,0,sizeof(debounceCounter));
        keyboardInput=0;
        tickSample=xTaskGetTickCount();
        for(i=0;i<25;i++){
          if(Wire.requestFrom(0x20,1,true))
            debounceCounter[i]=~Wire.read();
          vTaskDelayUntil(&tickSample,pdMS_TO_TICKS(2));
        }
        
        xSemaphoreGive(mux_I2CLock);
        
        for(i=0;i<8;i++){
          buttonSum=0;
          for(j=0;j<25;j++){
            buttonSum+=(debounceCounter[j]>>i)&1; //Liczymy wszystkie jedynki
          }
          if(buttonSum>12)
            keyboardInput|=1<<i; //Jezeli jest wiecej jedynek niz 0 to stan ustalony to 1
        }
        Serial.print(keyboardInput); Serial.println(" ISR");
        vTaskDelay(300);
        f_enableISR=true;
          //keyboardInput=~keyboardInput;
        //processKeyboard(keyboardInput); Wysylanie kolejki do maina  dodać vtaskDelayUntil
      }
    }
  }
}
void taskSelectDrink(void*pvParameters){
  char drinkId{};
  bool f_run=false;
  uint8_t currentScroll=0;
  uint8_t characterOffset=0;
  uint8_t i=1;
  uint8_t j=0;
  char buffer[LCD_WIDTH-3-4]{};
  sScreenData screenData{};
  for(;;){
    if(ulTaskNotifyTake(pdTRUE,0)>0){
      f_run=false;
      //currentScroll=0;
      while(xQueueReceive(qDrinkId,&drinkId,0)==pdPASS); // Remember to clear the queue
    }
    if(xQueueReceive(qDrinkId,&drinkId,pdMS_TO_TICKS(50))==pdPASS){
      f_run=true;
    }

    if(f_run){
      //First non-scrollable line
      memset(&screenData,0,sizeof(sScreenData));
      sprintf(screenData.lines[0],"[%d]%s",drinkId+1,drink[drinkId].drinkName);
      //First non-scrollable line
      
      //Other scrollable lines
      for(i=1;i<LCD_HEIGHT;i++){
        for(j=0;j<LCD_HEIGHT-1;j++){
          if(!drink[drinkId].ingredients[j+currentScroll])
            continue;
          memset(screenData.lines[i],0,sizeof(screenData.lines[i]));
          sprintf(screenData.lines[i],"%s",ingredients[j+currentScroll],drink[drinkId].ingredients[j+currentScroll]);
          characterOffset=0;
          memset(&buffer,0,sizeof(buffer));
          memcpy(&buffer,ingredients[j+currentScroll],sizeof(buffer));
          characterOffset+=9;
          sprintf(screenData.lines[i]+characterOffset,"%3d[ml]",drink[drinkId].ingredients[j+currentScroll]);
          characterOffset=0;
          
        }
      }
      currentScroll++;
      if(currentScroll==10-LCD_HEIGHT)
        currentScroll=0;
      //Other scrollable lines
      xQueueSend(qScreenData, &screenData, pdMS_TO_TICKS(50));
      //Other scrollable lines
      vTaskDelay(pdMS_TO_TICKS(1000));
    }
    else{
      vTaskDelay(pdMS_TO_TICKS(50));
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
    if((!f_resetTaskData)&&(xQueueReceive(qDrinkId,&drinkId,pdMS_TO_TICKS(50)))){
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
  uint8_t runTimeDays=0;
  uint8_t runTimeHours=0;
  uint8_t runTimeMinutes=0;
  uint8_t runTimeSeconds=0;
  
  uint64_t runTimeMillis=0;
  
  bool f_run=false;

  uint8_t i=0;
  
  sScreenData screenData{};
  for(;;){
    if(ulTaskNotifyTake(pdTRUE,0)>0){
      f_run=false;
	  while(xQueueReceive(qShowInfoId,&showInfoScreenId,0)==pdPASS); // Remember to clear the queue
	  showInfoScreenId=0;
    }
	if(xQueueReceive(qShowInfoId,&showInfoScreenId,pdMS_TO_TICKS(50))==pdPASS){
	  f_run=true;
	}
	if(f_run){
    for(i=0;i<2;i++){
      memset(screenData.lines[i],0,sizeof(screenData.lines[i]));
    }
	  switch(showInfoScreenId){
		case 0:
	      sprintf(screenData.lines[1],"%s","Software ver. 3.");      // Fix in release
		    //sprintf(screenData.lines[2],"%s","Author: Alan Kudelko");  // Fix in release
		    //sprintf(screenData.lines[3],"%s %d","Startup count: ",*(uint16_t*)pvParameters);
		break;
		case 1:
	      sprintf(screenData.lines[1],"%s","Current run time");      // Fix in release
        runTimeMillis=millis()/1000;
        runTimeDays=runTimeMillis/86400;
        runTimeHours=runTimeMillis/3600%24;
        runTimeMinutes=runTimeMillis/60%60;
        runTimeSeconds=runTimeMillis%60;
        /*
        sprintf(screenData.lines[2],"%2d %s %2d %s",runTimeDays,"days",runTimeHours,"h");
        if(runTimeDays<10)
          screenData.lines[2][0]='0';
        if(runTimeHours<10)
          screenData.lines[2][8]='0';
        memset(screenData.lines[3],0,sizeof(screenData.lines[3]));
        sprintf(screenData.lines[3],"%2d %s  %2d %s",runTimeMinutes,"min",runTimeSeconds,"s");
        if(runTimeMinutes<10)
          screenData.lines[3][0]='0';
        if(runTimeSeconds<10)
          screenData.lines[3][8]='0';
        */
		break;
	  }
	  sprintf(screenData.lines[0],"%s","Drink Creator 60");      // Fix in release
	  xQueueSend(qScreenData,&screenData,pdMS_TO_TICKS(50));
	  
	  vTaskDelay(pdMS_TO_TICKS(1000));
	}
	else{
	  vTaskDelay(pdMS_TO_TICKS(50));
	}
  }
}
void taskShowTemp(void*pvParameters){
  for(;;){
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}
void taskShowLastError(void*pvParameters){
  uint8_t i=0;
  uint8_t keyboardData=0;
  uint8_t errorLength=0;
  sScreenData screenData{};

  // If last error is not confirmed
  if(*((uint8_t*)pvParameters)==0){
      //Suspend not needed tasks
    vTaskSuspend(taskHandles[TASK_MAIN]);
    vTaskSuspend(taskHandles[TASK_REGULATE_TEMP]);
    vTaskSuspend(taskHandles[TASK_SELECT_DRINK]);
    vTaskSuspend(taskHandles[TASK_ORDER_DRINK]);
    vTaskSuspend(taskHandles[TASK_SHOW_INFO]);  
    vTaskSuspend(taskHandles[TASK_SHOW_TEMP]);
    
    errorLength=strlen(lastSystemError.errorText);

    strncpy(screenData.lines[1],"Fault time signature",LCD_WIDTH);
    /*
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
      */
      //Fix in release
  }
  else
    vTaskSuspend(NULL);

  for(;;){
    if(xQueueReceive(qKeyboardData,&keyboardData,pdMS_TO_TICKS(50))){
      if((keyboardData&E_GREEN_BUTTON)&E_GREEN_BUTTON){
        lastSystemError.confirmed=1;
        EEPROMUpdateLastStartupError(&lastSystemError);
        //Restart systemu
      }
    }
    strncpy(screenData.lines[0],lastSystemError.errorText+i,LCD_WIDTH);
    xQueueSend(qScreenData,&screenData,pdMS_TO_TICKS(50));
    
    i++;
    if(i==errorLength-LCD_WIDTH+2){
      i=0;
      lastSystemError.confirmed=1;
      EEPROMUpdateLastStartupError(&lastSystemError);
    }
    vTaskDelay(pdMS_TO_TICKS(300));
  }
}
void setup(){
  while(!eeprom_is_ready());
  
  EEPROMUpdateBootups(&bootupsCount);
  EEPROMGetLastStartupError(&lastSystemError);
  
  lcd.init();
  lcd.load_custom_character(0,fullSquare);
  lcd.load_custom_character(1,fullSquare);
  lcd.load_custom_character(2,fullSquare);
  lcd.load_custom_character(3,fullSquare);
  lcd.load_custom_character(4,fullSquare);
  lcd.load_custom_character(5,fullSquare);
  lcd.load_custom_character(6,fullSquare);
  lcd.load_custom_character(7,fullSquare);
  
  Serial.begin(9600);
  
  lastBootup_dump(&bootupsCount);
  
  initializeMemory();
  pinMode(INTPin,INPUT);
  attachInterrupt(digitalPinToInterrupt(INTPin),setInputFlag,FALLING);

  f_errorConfirmed=lastSystemError.confirmed;
  if(f_errorConfirmed==0)
    lastError_dump(&lastSystemError);
  
  ram_dump();

  //xSemaphoreTake(sem_ReadData,pdMS_TO_TICKS(1000));

  taskHandles[TASK_ERROR_HANDLER]  =xTaskCreateStatic(taskErrorHandler ,"ERROR HANDLER",TASK_ERROR_HANDLER_STACK_SIZE  ,NULL                ,1,errorHandlerStack,&errorHandlerTCB);
  taskHandles[TASK_STACK_DEBUGGER] =xTaskCreateStatic(taskStackDebugger,"STACK DEBUG"  ,TASK_STACK_DEBUGGER_STACK_SIZE ,NULL                ,1,stackDebuggerStack,&stackDebuggerTCB);
  taskHandles[TASK_UPDATE_SCREEN]  =xTaskCreateStatic(taskUpdateScreen ,"UPDATE SCREEN",UPDATE_SCREEN_STACK_SIZE       ,NULL                ,1,updateScreenStack,&updateScreenTCB);
  taskHandles[TASK_MAIN]           =xTaskCreateStatic(taskMain         ,"MAIN"         ,TASK_MAIN_STACK_SIZE           ,NULL                ,1,mainStack,&mainTCB);
  taskHandles[TASK_REGULATE_TEMP]  =xTaskCreateStatic(taskRegulateTemp ,"REGULATE TEMP",TASK_REGULATE_TEMP_STACK_SIZE  ,NULL                ,1,regulateTempStack,&regulateTempTCB);
  taskHandles[TASK_READ_INPUT]     =xTaskCreateStatic(taskReadInput    ,"READ INPUT"   ,TASK_READ_INPUT_STACK_SIZE     ,NULL                ,3,readInputStack,&readInputTCB);
  taskHandles[TASK_SELECT_DRINK]   =xTaskCreateStatic(taskSelectDrink  ,"SELECT DRINK" ,TASK_SELECT_DRINK_STACK_SIZE   ,NULL                ,1,selectDrinkStack,&selectDrinkTCB);
  taskHandles[TASK_ORDER_DRINK]    =xTaskCreateStatic(taskOrderDrink   ,"ORDER DRINK"  ,TASK_ORDER_DRINK_STACK_SIZE    ,NULL                    ,1,orderDrinkStack,&orderDrinkTCB);
  taskHandles[TASK_SHOW_INFO]      =xTaskCreateStatic(taskShowInfo     ,"SHOW INFO"    ,TASK_SHOW_INFO_STACK_SIZE      ,(void*)&bootupsCount    ,1,showInfoStack,&showInfoTCB);
  taskHandles[TASK_SHOW_TEMP]      =xTaskCreateStatic(taskShowTemp     ,"SHOW TEMP"    ,TASK_SHOW_TEMP_STACK_SIZE      ,NULL                    ,1,showTempStack,&showTempTCB);
  taskHandles[TASK_SHOW_LAST_ERROR]=xTaskCreateStatic(taskShowLastError,"LAST ERROR"   ,TASK_SHOW_LAST_ERROR_STACK_SIZE,(void*)&f_errorConfirmed,2,showLastErrorStack,&showLastErrorTCB);
}
void loop(){

}
