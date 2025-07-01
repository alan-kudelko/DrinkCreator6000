#ifndef _DRINK_CREATOR6000_CONFIG_H_
#define _DRINK_CREATOR6000_CONFIG_H_


// Task data
enum{TASK_ERROR_HANDLER_STACK_SIZE=200};     //0
enum{TASK_STACK_DEBUGGER_STACK_SIZE=200};    //1
enum{TASK_MAIN_STACK_SIZE=256};              //2
enum{UPDATE_SCREEN_STACK_SIZE=232};          //3
enum{TASK_REGULATE_TEMP_STACK_SIZE=128};     //4
enum{TASK_READ_INPUT_STACK_SIZE=150};        //5
enum{TASK_SELECT_DRINK_STACK_SIZE=256};      //6
enum{TASK_ORDER_DRINK_STACK_SIZE=320};       //7
enum{TASK_SHOW_INFO_STACK_SIZE=256};         //8
enum{TASK_SHOW_TEMP_STACK_SIZE=192};         //9
enum{TASK_SHOW_LAST_ERROR_STACK_SIZE=256};   //10
enum{TASK_KEYBOARD_SIM_STACK_SIZE=192};      //11

enum{
  TASK_ERROR_HANDLER=0,
  TASK_STACK_DEBUGGER=1,
  TASK_MAIN=2,
  TASK_UPDATE_SCREEN=3,
  TASK_REGULATE_TEMP=4,
  TASK_READ_INPUT=5,
  TASK_SELECT_DRINK=6,
  TASK_ORDER_DRINK=7,
  TASK_SHOW_INFO=8,
  TASK_SHOW_TEMP=9,
  TASK_SHOW_LAST_ERROR=10,
  TASK_KEYBOARD_SIM=11
};

enum{GUARD_ZONE_SIZE=16};
	 
enum{TASK_N=12};

StaticTask_t errorHandlerTCB{0xF};                                   //0
StaticTask_t stackDebuggerTCB{0xF};                                  //1
StaticTask_t mainTCB{0xF};                                           //2
StaticTask_t updateScreenTCB{0xF};                                   //3
StaticTask_t regulateTempTCB{0xF};                                   //4
StaticTask_t readInputTCB{0xF};                                      //5
StaticTask_t selectDrinkTCB{0xF};                                    //6
StaticTask_t orderDrinkTCB{0xF};                                     //7
StaticTask_t showInfoTCB{0xF};                                       //8
StaticTask_t showTempTCB{0xF};                                       //9
StaticTask_t showLastErrorTCB{0xF};                                  //10
StaticTask_t keyboardSimTCB{0xF};                                    //11

TaskHandle_t taskHandles[TASK_N]{0xF};

StackType_t errorHandlerStack[TASK_ERROR_HANDLER_STACK_SIZE]{0xF};   //0
StackType_t guardZone0[GUARD_ZONE_SIZE]{0xF};
StackType_t stackDebuggerStack[TASK_STACK_DEBUGGER_STACK_SIZE]{0xF}; //1
StackType_t guardZone1[GUARD_ZONE_SIZE]{0xF};
StackType_t mainStack[TASK_MAIN_STACK_SIZE]{0xF};                    //2
StackType_t guardZone2[GUARD_ZONE_SIZE]{0xF};
StackType_t updateScreenStack[UPDATE_SCREEN_STACK_SIZE]{0xF};        //3
StackType_t guardZone3[GUARD_ZONE_SIZE]{0xF};
StackType_t regulateTempStack[TASK_REGULATE_TEMP_STACK_SIZE]{0xF};   //4
StackType_t guardZone4[GUARD_ZONE_SIZE]{0xF};
StackType_t readInputStack[TASK_READ_INPUT_STACK_SIZE]{0xF};         //5
StackType_t guardZone5[GUARD_ZONE_SIZE]{0xF};
StackType_t selectDrinkStack[TASK_SELECT_DRINK_STACK_SIZE]{0xF};     //6
StackType_t guardZone6[GUARD_ZONE_SIZE]{0xF};
StackType_t orderDrinkStack[TASK_ORDER_DRINK_STACK_SIZE]{0xF};       //7
StackType_t guardZone7[GUARD_ZONE_SIZE]{0xF};
StackType_t showInfoStack[TASK_SHOW_INFO_STACK_SIZE]{0xF};           //8
StackType_t guardZone8[GUARD_ZONE_SIZE]{0xF};
StackType_t showTempStack[TASK_SHOW_TEMP_STACK_SIZE]{0xF};           //9
StackType_t guardZone9[GUARD_ZONE_SIZE]{0xF};
StackType_t showLastErrorStack[TASK_SHOW_LAST_ERROR_STACK_SIZE]{0xF};//10
StackType_t guardZone10[GUARD_ZONE_SIZE]{0xF};
StackType_t keyboardSimStack[TASK_KEYBOARD_SIM_STACK_SIZE]{0xF};     //11
StackType_t guardZone11[GUARD_ZONE_SIZE]{0xF};
// Task data



// Screen data
enum{
  LCD_WIDTH=20,
  LCD_HEIGHT=4
};


enum{
  WELCOME_SCREEN=0,
  DRINK_SELECT=1,
  DRINK_ORDER=2,
  SHOW_INFO=3,
  TEMP_INFO=4,
  RAM_INFO=5,
  STACK_INFO=6
};

  // 0 welcome screen
  // 1 drink select
  // 2 drink order
  // 3 show system info
  // 4 show temp info
  // 5 show ram info
  // 6 show stack size info
  
  // 0 -> 1 -> 2
  //      | -> 3 -> 4 -> 5 -> 6  

LiquidCrystal_I2C lcd(0x27, LCD_WIDTH, LCD_HEIGHT);
// Screen data

// Flow control

enum{
  SCREEN_QUEUE_BUFFER_COUNT=6,
  KEYBOARD_QUEUE_BUFFER_COUNT=2,
  DRINK_ID_QUEUE_BUFFER_COUNT=2,
  SHOW_INFO_QUEUE_BUFFER_COUNT=2,
  ERROR_ID_QUEUE_BUFFER_COUNT=1
};

uint8_t screenQueueBuffer[SCREEN_QUEUE_BUFFER_COUNT*sizeof(sScreenData)]{};
uint8_t keyboardQueueBuffer[KEYBOARD_QUEUE_BUFFER_COUNT*sizeof(uint8_t)]{};
uint8_t drinkIdQueueBuffer[DRINK_ID_QUEUE_BUFFER_COUNT*sizeof(uint8_t)]{};
uint8_t showInfoQueueBuffer[SHOW_INFO_QUEUE_BUFFER_COUNT*sizeof(uint8_t)]{};
uint8_t errorIdQueueBuffer[ERROR_ID_QUEUE_BUFFER_COUNT*sizeof(TaskHandle_t )]{};

StaticQueue_t screenQueueStructBuffer{};
StaticQueue_t keyboardQueueStructBuffer{};
StaticQueue_t drinkIdQueueStructBuffer{};
StaticQueue_t showInfoQueueStructBuffer{};
StaticQueue_t errorIdQueueStructBuffer{};

StaticSemaphore_t semReadDataBuffer{};
StaticSemaphore_t muxI2CLockBuffer{};
StaticSemaphore_t muxSerialLockBuffer{};

QueueHandle_t qScreenData{};
QueueHandle_t qKeyboardData{};
QueueHandle_t qDrinkId{};
QueueHandle_t qShowInfoId{};
QueueHandle_t qErrorId{};

SemaphoreHandle_t sem_ReadData{};
SemaphoreHandle_t mux_I2CLock{};
SemaphoreHandle_t mux_SerialLock{};

// Flow control

// Global variables
uint8_t f_errorConfirmed=0;;
uint16_t bootupsCount=0;
sSystemError lastSystemError;

float currentTemperature=10.0f;
float setTemperature=4.0f;
float temperatureHysteresis=1.0f;

volatile bool f_enableISR=true;
// Global variables



#endif 