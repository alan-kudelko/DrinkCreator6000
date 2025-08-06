#ifndef _DRINK_CREATOR6000_DATA_TYPES_H_
  #define _DRINK_CREATOR6000_DATA_TYPES_H_

#include <stdint.h>

#define DRINK_NAME_LENGTH 16


struct sDrinkData{
    char drinkName[DRINK_NAME_LENGTH];
    uint16_t ingredients[8];
    uint16_t numberOfOrders;
    // Can be optimzed
};
//struct to pass to taskActivatePumps(void*)
struct sScreenData{
    char lines[4][20];
    struct{
        uint8_t lcdCursorX: 5;
        uint8_t lcdCursorY: 2;
        uint8_t lcdCursorBlink: 1;
    };
};
//struct to pass to taskUpdateScreen(void*)
struct sSystemError{
    char errorText[50];
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t days;
    uint8_t confirmed; // If its not comfirmed display it on the LCD and on the serial port
    uint8_t taskId; // Id of task which caused error
    // Bit field can be optimzed!
};
//struct containing error information
struct sUIContext{
    uint8_t autoScrollEnable: 1;	
    uint8_t currentTask: 2;
    //current UI bounded task 0 - 3
    //0 taskWelcomeScreen
    //1 taskSelectDrink
    //2 taskOrderDrink
    //3 taskShowSystemInfo
    uint8_t currentMenu: 3;
    // current Menu in given task
    // I.E
    // 0 showInfo_Firmware
    // 1 showInfo_Temp
    // 2 showInfo_Memory
    // 3 showInfo_Task
    // 4 showInfo_Error_Sub_N        --submenu for scrolling message
    // 5 showInfo_ConfError
    // 
    // default submenu will be indexed by value 0
    uint8_t currentSubMenu; // Cannot be optimzed with current UI
    // used for scrolling inside currentMenu
};
//struct describing current program context
#endif // _DRINK_CREATOR6000_DATA_TYPES_H_
