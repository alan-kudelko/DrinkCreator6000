#include "DrinkCreator6000_Init.h"
#include <Arduino.h>
#include <Wire.h>
//////////////////////////////////////////////////////////////////
// IO initialization:
// Configures I/O pins and attaches interrupts related to pin events
void initializeIO(){
  pinMode(INTPin,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INTPin),setInputFlag,FALLING);  
}
//////////////////////////////////////////////////////////////////
// Memory initialization:
// Static allocation of stacks, queues, semaphores, and mutexes
void initializeMemory(){
  qScreenData=xQueueCreateStatic(SCREEN_QUEUE_BUFFER_COUNT,sizeof(sScreenData),screenQueueBuffer,&screenQueueStructBuffer);
  qKeyboardData=xQueueCreateStatic(KEYBOARD_QUEUE_BUFFER_COUNT,sizeof(uint8_t),keyboardQueueBuffer,&keyboardQueueStructBuffer);
  qDrinkId=xQueueCreateStatic(DRINK_ID_QUEUE_BUFFER_COUNT,sizeof(uint8_t),drinkIdQueueBuffer,&drinkIdQueueStructBuffer);
  qShowInfoId=xQueueCreateStatic(SHOW_INFO_QUEUE_BUFFER_COUNT,sizeof(uint8_t),showInfoQueueBuffer,&showInfoQueueStructBuffer);
  qErrorId=xQueueCreateStatic(ERROR_ID_QUEUE_BUFFER_COUNT,sizeof(TaskHandle_t),errorIdQueueBuffer,&errorIdQueueStructBuffer);
  qLastErrorId=xQueueCreateStatic(LAST_ERROR_ID_QUEUE_BUFFER_COUNT,sizeof(uint8_t),lastErrorIdQueueBuffer,&lastErrorIdQueueStructBuffer);
  
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
  memset(guardZone11,0xF,GUARD_ZONE_SIZE);
}
//////////////////////////////////////////////////////////////////
// Hardware initialization:
// Initializes Serial, LCD display, I²C keyboard (PCF8574), 
// shift register (74HC595), and temperature sensor
void initializeHardware(){
////////////////////////////////////////////////////////////////// Serial init
  Serial.begin(9600);	
////////////////////////////////////////////////////////////////// LCD init	
  lcd.begin();
  lcd.backlight();
  // Load custom characters from "LcdCharacters.h"
  lcd.load_custom_character(0,fullSquare);
  lcd.load_custom_character(1,fullSquare);
  lcd.load_custom_character(2,fullSquare);
  lcd.load_custom_character(3,fullSquare);
  lcd.load_custom_character(4,fullSquare);
  lcd.load_custom_character(5,fullSquare);
  lcd.load_custom_character(6,fullSquare);
  lcd.load_custom_character(7,fullSquare);	
////////////////////////////////////////////////////////////////// Keyboard init	  
  // Configure IOCON register (Bank = 1, SEQOP disabled)
  Wire.beginTransmission(MCP_ADDR);
  Wire.write(0x0A);
  Wire.write(0xA0);
  Wire.endTransmission();
  // Set GPA as inputs
  Wire.beginTransmission(MCP_ADDR);
  Wire.write(0x00);
  Wire.write(0xFF);
  Wire.endTransmission();  
  // Invert polarity: LOW = pressed
  Wire.beginTransmission(MCP_ADDR);
  Wire.write(0x01);
  Wire.write(0xFF);
  Wire.endTransmission();
  // Enable interrupt-on-change
  Wire.beginTransmission(MCP_ADDR);
  Wire.write(0x02);
  Wire.write(0xA0);
  Wire.endTransmission();
  // Set default comparison value (HIGH)
  Wire.beginTransmission(MCP_ADDR);
  Wire.write(0x03);
  Wire.write(0xFF);
  Wire.endTransmission();
  // Configure interrupt on mismatch with DEFVAL
  Wire.beginTransmission(MCP_ADDR);
  Wire.write(0x04);
  Wire.write(0xFF);
  Wire.endTransmission();
  // Enable pull-ups on GPA
  Wire.beginTransmission(MCP_ADDR);
  Wire.write(0x06);
  Wire.write(0xFF);
  Wire.endTransmission();
////////////////////////////////////////////////////////////////// Shift register init
////////////////////////////////////////////////////////////////// Thermometer init
}
