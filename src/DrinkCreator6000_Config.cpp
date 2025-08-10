#include <DrinkCreator6000_Config.h>

//////////////////////////////////////////////////////////////////
// Task data:
// Stack sizes for all tasks and their IDs
// Guard zones for stack overflow protection

StaticTask_t errorHandlerTCB         __attribute__((section(".tdat"))); //0
StaticTask_t serialSystemDebuggerTCB __attribute__((section(".tdat"))); //1
StaticTask_t mainTCB                 __attribute__((section(".tdat"))); //2
StaticTask_t readInputTCB            __attribute__((section(".tdat"))); //3
StaticTask_t serialInputTCB          __attribute__((section(".tdat"))); //4
StaticTask_t updateScreenTCB         __attribute__((section(".tdat"))); //5
StaticTask_t readTempTCB             __attribute__((section(".tdat"))); //6
StaticTask_t regulateTempTCB         __attribute__((section(".tdat"))); //7
StaticTask_t selectDrinkTCB          __attribute__((section(".tdat"))); //8
StaticTask_t orderDrinkTCB           __attribute__((section(".tdat"))); //9
StaticTask_t showSystemInfoTCB       __attribute__((section(".tdat"))); //10
StaticTask_t welcomeScreenTCB        __attribute__((section(".tdat"))); //11

TaskHandle_t taskHandles[TASK_N];

volatile StackType_t guardZone0[GUARD_ZONE_SIZE]                             __attribute__((section(".tdat.guardZone0")));
StackType_t errorHandlerStack[TASK_ERROR_HANDLER_STACK_SIZE]                 __attribute__((section(".tdat.errorHandlerStack"))); //0

volatile StackType_t guardZone1[GUARD_ZONE_SIZE]                             __attribute__((section(".tdat.guardZone1")));
StackType_t serialSystemDebuggerStack[TASK_SERIAL_SYSTEM_DEBUGGER_STACK_SIZE]__attribute__((section(".tdat.serialSystemDebuggerStack"))); //1

volatile StackType_t guardZone2[GUARD_ZONE_SIZE]                             __attribute__((section(".tdat.guardZone2")));
StackType_t mainStack[TASK_MAIN_STACK_SIZE]                                  __attribute__((section(".tdat.mainStack"))); //2

volatile StackType_t guardZone3[GUARD_ZONE_SIZE]                             __attribute__((section(".tdat.guardZone3")));
StackType_t readInputStack[TASK_READ_INPUT_STACK_SIZE]                       __attribute__((section(".tdat.readInputStack"))); //3

volatile StackType_t guardZone4[GUARD_ZONE_SIZE]                             __attribute__((section(".tdat.guardZone4")));
StackType_t serialInputStack[TASK_SERIAL_INPUT_STACK_SIZE]                   __attribute__((section(".tdat.serialInputStack"))); //4

volatile StackType_t guardZone5[GUARD_ZONE_SIZE]                             __attribute__((section(".tdat.guardZone5")));
StackType_t updateScreenStack[TASK_UPDATE_SCREEN_STACK_SIZE]                 __attribute__((section(".tdat.updateScreenStack"))); //5

volatile StackType_t guardZone6[GUARD_ZONE_SIZE]                             __attribute__((section(".tdat.guardZone6")));
StackType_t readTempStack[TASK_READ_TEMP_STACK_SIZE]                         __attribute__((section(".tdat.readtempStack"))); //6

volatile StackType_t guardZone7[GUARD_ZONE_SIZE]                             __attribute__((section(".tdat.guardZone7")));
StackType_t regulateTempStack[TASK_REGULATE_TEMP_STACK_SIZE]                 __attribute__((section(".tdat.regulateTempStack"))); //7

volatile StackType_t guardZone8[GUARD_ZONE_SIZE]                             __attribute__((section(".tdat.guardZone8")));
StackType_t selectDrinkStack[TASK_SELECT_DRINK_STACK_SIZE]                   __attribute__((section(".tdat.selectDrinkStack"))); //8

volatile StackType_t guardZone9[GUARD_ZONE_SIZE]                             __attribute__((section(".tdat.guardZone9")));
StackType_t orderDrinkStack[TASK_ORDER_DRINK_STACK_SIZE]                     __attribute__((section(".tdat.orderDrinkStack"))); //9

volatile StackType_t guardZone10[GUARD_ZONE_SIZE]                            __attribute__((section(".tdat.guardZone10")));
StackType_t showSystemInfoStack[TASK_SHOW_SYSTEM_INFO_STACK_SIZE]            __attribute__((section(".tdat.showSystemInfoStack"))); //10

volatile StackType_t guardZone11[GUARD_ZONE_SIZE]                            __attribute__((section(".tdat.guardZone11")));
StackType_t welcomeScreenStack[TASK_WELCOME_SCREEN_STACK_SIZE]               __attribute__((section(".tdat.welcomeScreenStack"))); //11

volatile StackType_t* guardZones[TASK_N]={
  guardZone0,
  guardZone1,
  guardZone2,
  guardZone3,
  guardZone4,
  guardZone5,
  guardZone6,
  guardZone7,
  guardZone8,
  guardZone9,
  guardZone10,
  guardZone11
};
//////////////////////////////////////////////////////////////////
// Screen data:
// LCD dimensions and I2C address
//LiquidCrystal_I2C lcd(LCD_ADDR,LCD_WIDTH,LCD_HEIGHT);
//////////////////////////////////////////////////////////////////
// Flow control:
// Queue sizes and buffers for inter-task communication
uint8_t screenQueueBuffer[SCREEN_QUEUE_BUFFER_COUNT*sizeof(sScreenData)]{MEMORY_FILL_PATTERN};
uint8_t keyboardQueueBuffer[KEYBOARD_QUEUE_BUFFER_COUNT*sizeof(uint8_t)]{MEMORY_FILL_PATTERN};
uint8_t errorIdQueueBuffer[ERROR_ID_QUEUE_BUFFER_COUNT*sizeof(TaskHandle_t)]{MEMORY_FILL_PATTERN};

StaticQueue_t screenQueueStructBuffer;
StaticQueue_t keyboardQueueStructBuffer;
StaticQueue_t errorIdQueueStructBuffer;

StaticSemaphore_t semReadDataBuffer;
StaticSemaphore_t muxI2CLockBuffer;
StaticSemaphore_t muxSerialLockBuffer;

QueueHandle_t qScreenData{};
QueueHandle_t qKeyboardData{};
QueueHandle_t qDrinkId{};
QueueHandle_t qOrderDrinkId{};
QueueHandle_t qShowInfoId{};
QueueHandle_t qErrorId{};

SemaphoreHandle_t sem_ReadData{};
SemaphoreHandle_t mux_I2CLock{};
SemaphoreHandle_t mux_SerialLock{};

volatile sUIContext UI_Context{}; //Initialized in order to be stored in .data
//////////////////////////////////////////////////////////////////
// Global variables:
// System error, flags, counters and temperature parameters
sSystemError lastSystemError{};
uint8_t f_errorConfirmed=0;
uint16_t bootupsCount=0;

float currentTemperature=10.5;
float setTemperature=4.5;
float temperatureHysteresis=1.0;

volatile bool f_enableISR=true;
//////////////////////////////////////////////////////////////////
// Drink data:
// Drink definitions, ingredients, and pump efficiencies
const struct sDrinkData drink[DRINK_COUNT]={
  {"Raz",    50, 50, 50, 50, 50, 50, 50, 50, 0},                   //1
  {"Dwa",   200, 200, 0, 0, 0, 0, 0, 0, 0},                //2
  {"Trzy",   50, 100, 150, 200, 250, 300, 350, 400, 0},  //3
  {"Cztery", 50, 100, 150, 200, 250, 300, 350, 400, 0},  //4
  {"Piec",   50, 0, 0, 0, 250, 300, 350, 400, 0},  //5
  {"Szesc",   50, 100, 150, 200, 250, 0, 350, 0, 0},  //6
  {"Siedem",   50, 100, 0, 200, 250, 0, 0, 0, 0},  //7
  {"Osiem",   50, 100, 150, 200, 250, 300, 350, 400, 0},  //8
  {"Dziewiec",   50, 100, 150, 200, 250, 300, 350, 400, 0},  //9
  {"Dziesiec",   50, 100, 150, 200, 250, 300, 350, 400, 0},  //10
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
// Idle task counter
volatile uint32_t idleCounter=0;
volatile uint32_t idleCounterLast=0;
volatile uint32_t idleCounterPerSecond=0;
volatile uint32_t tickCount=0;
volatile uint32_t idleCalib=0;
