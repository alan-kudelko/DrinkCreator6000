#ifndef CUSTOM_DATA_TYPES_H
#define CUSTOM_DATA_TYPES_H

struct sDrinkData{
  char drinkName[16];
  uint16_t ingredients[8];
  uint16_t numberOfOrders;
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
struct sSystemError{
  char errorText[50];
  uint8_t seconds;
  uint8_t minutes;
  uint16_t hours;
  uint8_t days;
  uint8_t confirmed; // If its not comfirmed display it on the LCD and on the serial port
};
//struct to pass to taskUpdateScreen(void*)
#endif
