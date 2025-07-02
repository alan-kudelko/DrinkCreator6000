#include "DrinkCreator6000_Config.h"

////////////////////////////////////////////////////////////////// TASK DATA
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
StackType_t guardZone0[GUARD_ZONE_SIZE];

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
////////////////////////////////////////////////////////////////// SCREEN DATA
LiquidCrystal_I2C lcd(LCD_ADDR,LCD_WIDTH,LCD_HEIGHT);
////////////////////////////////////////////////////////////////// FLOW CONTROL
uint8_t screenQueueBuffer[SCREEN_QUEUE_BUFFER_COUNT*sizeof(sScreenData)]{0xF};
uint8_t keyboardQueueBuffer[KEYBOARD_QUEUE_BUFFER_COUNT*sizeof(uint8_t)]{0xF};
uint8_t drinkIdQueueBuffer[DRINK_ID_QUEUE_BUFFER_COUNT*sizeof(uint8_t)]{0xF};
uint8_t showInfoQueueBuffer[SHOW_INFO_QUEUE_BUFFER_COUNT*sizeof(uint8_t)]{0xF};
uint8_t errorIdQueueBuffer[ERROR_ID_QUEUE_BUFFER_COUNT*sizeof(TaskHandle_t)]{0xF};

StaticQueue_t screenQueueStructBuffer{0xF};
StaticQueue_t keyboardQueueStructBuffer{0xF};
StaticQueue_t drinkIdQueueStructBuffer{0xF};
StaticQueue_t showInfoQueueStructBuffer{0xF};
StaticQueue_t errorIdQueueStructBuffer{0xF};

StaticSemaphore_t semReadDataBuffer{0xF};
StaticSemaphore_t muxI2CLockBuffer{0xF};
StaticSemaphore_t muxSerialLockBuffer{0xF};

QueueHandle_t qScreenData{0xF};
QueueHandle_t qKeyboardData{0xF};
QueueHandle_t qDrinkId{0xF};
QueueHandle_t qOrderDrinkId{0xF};
QueueHandle_t qShowInfoId{0xF};
QueueHandle_t qErrorId{0xF};

SemaphoreHandle_t sem_ReadData{0xF};
SemaphoreHandle_t mux_I2CLock{0xF};
SemaphoreHandle_t mux_SerialLock{0xF};
////////////////////////////////////////////////////////////////// GLOBAL VARIABLES
sSystemError lastSystemError{0xF};
uint8_t f_errorConfirmed=0;;
uint16_t bootupsCount=0;

float currentTemperature=10.0f;
float setTemperature=4.0f;
float temperatureHysteresis=1.0f;

volatile bool f_enableISR=true;
////////////////////////////////////////////////////////////////// DRINK DATA
const sDrinkData drink[20]={
  {"Raz",    50, 0, 0, 0, 0, 0, 0, 0, 0},                   //1
  {"Dwa",   200, 200, 0, 0, 0, 0, 0, 0, 0},                //2
  {"Trzy",   50, 100, 150, 200, 250, 300, 350, 400, 0},  //3
  {"Cztery", 50, 100, 150, 200, 250, 300, 350, 400, 0},  //4
  {"Piec",   50, 0, 0, 0, 250, 300, 350, 400, 0},  //5
  {"Szesc",   50, 100, 150, 200, 250, 0, 350, 0, 0},  //6
  {"Siedem",   50, 100, 0, 200, 250, 0, 0, 0, 0},  //7
  {"Osiem",   50, 100, 150, 200, 250, 300, 350, 400, 0},  //8
  {"Dziewiec",   50, 100, 150, 200, 250, 300, 350, 400, 0},  //9
  {"Dziesiec",   50, 100, 150, 200, 250, 300, 350, 400, 0},  //10
  {"Trzy", 50, 100, 150, 200, 250, 300, 350, 400, 0},  //11
  {"Trzy", 50, 100, 150, 200, 250, 300, 350, 400, 0},  //12
  {"Trzy", 50, 100, 150, 200, 250, 300, 350, 400, 0},  //13
  {"Trzy", 50, 100, 150, 200, 250, 300, 350, 400, 0},  //14
  {"Trzy", 50, 100, 150, 200, 250, 300, 350, 400, 0},  //15
  {"Trzy", 50, 100, 150, 200, 250, 300, 350, 400, 0},  //16
  {"Trzy", 50, 100, 150, 200, 250, 300, 350, 400, 0},  //17
  {"Trzy", 50, 100, 150, 200, 250, 300, 350, 400, 0},  //18
  {"Trzy", 50, 100, 150, 200, 250, 300, 350, 400, 0},  //19
  {"Trzy", 50, 100, 150, 200, 250, 300, 350, 400, 0}   //20
};
//
const char ingredients[8][LCD_WIDTH-4-4]{
  {"Whiskey"},    //1
  {"Jager"},      //2
  {"Rum"},        //3
  {"Wodka"},      //4
  {"Cola"},       //5
  {"Sok pom"},    //6
  {"Sok cyt"},    //7
  {"Woda"}        //8
};
//
const uint8_t pumpsEff[8]{
  200,
  200,
  200,
  200,
  200,
  200,
  200,
  200
};
