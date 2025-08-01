#include "DrinkCreator6000_Init.h"
#include <Arduino.h>
#include <Wire.h>
//////////////////////////////////////////////////////////////////
// IO initialization:
// Configures I/O pins and attaches interrupts related to pin events
void initializeIO(){
  pinMode(INTPin,INPUT_PULLUP);
}
//////////////////////////////////////////////////////////////////
// Memory initialization:
// Static allocation of stacks, queues, semaphores, and mutexes
void initializeMemory(){
  qScreenData=xQueueCreateStatic(SCREEN_QUEUE_BUFFER_COUNT,sizeof(sScreenData),screenQueueBuffer,&screenQueueStructBuffer);
  qKeyboardData=xQueueCreateStatic(KEYBOARD_QUEUE_BUFFER_COUNT,sizeof(uint8_t),keyboardQueueBuffer,&keyboardQueueStructBuffer);
  qErrorId=xQueueCreateStatic(ERROR_ID_QUEUE_BUFFER_COUNT,sizeof(TaskHandle_t),errorIdQueueBuffer,&errorIdQueueStructBuffer);
  
  sem_ReadData=xSemaphoreCreateBinaryStatic(&semReadDataBuffer);
  mux_I2CLock=xSemaphoreCreateMutexStatic(&muxI2CLockBuffer);
  mux_SerialLock=xSemaphoreCreateMutexStatic(&muxSerialLockBuffer);

  memset((void*)&UI_Context,0,sizeof(UI_Context));
  
  memset((void*)guardZone0,MEMORY_FILL_PATTERN,GUARD_ZONE_SIZE);
  memset((void*)guardZone1,MEMORY_FILL_PATTERN,GUARD_ZONE_SIZE);
  memset((void*)guardZone2,MEMORY_FILL_PATTERN,GUARD_ZONE_SIZE);
  memset((void*)guardZone3,MEMORY_FILL_PATTERN,GUARD_ZONE_SIZE);
  memset((void*)guardZone4,MEMORY_FILL_PATTERN,GUARD_ZONE_SIZE);
  memset((void*)guardZone5,MEMORY_FILL_PATTERN,GUARD_ZONE_SIZE);
  memset((void*)guardZone6,MEMORY_FILL_PATTERN,GUARD_ZONE_SIZE);
  memset((void*)guardZone7,MEMORY_FILL_PATTERN,GUARD_ZONE_SIZE);
  memset((void*)guardZone8,MEMORY_FILL_PATTERN,GUARD_ZONE_SIZE);
  memset((void*)guardZone9,MEMORY_FILL_PATTERN,GUARD_ZONE_SIZE);
  memset((void*)guardZone10,MEMORY_FILL_PATTERN,GUARD_ZONE_SIZE);
  memset((void*)guardZone11,MEMORY_FILL_PATTERN,GUARD_ZONE_SIZE);
}
//////////////////////////////////////////////////////////////////
// Hardware initialization:
// Initializes Serial, LCD display, I²C keyboard (MCP23017), 
// shift register (74HC595), and temperature sensor
void initializeHardware(){
////////////////////////////////////////////////////////////////// Serial init
  Serial.begin(115200);	
////////////////////////////////////////////////////////////////// LCD init	
  lcd.begin();
  lcd.backlight();
////////////////////////////////////////////////////////////////// Keyboard init	  
// Configure IOCON register SEQOP enabled (NOSEQOP), BANK=0
  Wire.beginTransmission(MCP_ADDR);
  Wire.write(0x0A);
  Wire.write(0x20);
  Wire.endTransmission();

// Set GPA as inputs (IODIRA)
  Wire.beginTransmission(MCP_ADDR);
  Wire.write(0x00);
  Wire.write(0xFF); //Git
  Wire.endTransmission();  

// Invert polarity: LOW = pressed (IPOLA)
  //Wire.beginTransmission(MCP_ADDR);
  //Wire.write(0x02);
  //Wire.write(0xFF); //Git
  //Wire.endTransmission();

// Enable interrupt-on-change on all GPA pins (GPINTENA)
  Wire.beginTransmission(MCP_ADDR);
  Wire.write(0x04);
  Wire.write(0xFF); //Git
  Wire.endTransmission();

// Set default comparison value (HIGH) (DEFVALA)
  Wire.beginTransmission(MCP_ADDR);
  Wire.write(0x06);
  Wire.write(0xFF); //Git
  Wire.endTransmission();

// Configure interrupt on mismatch with DEFVAL (INTCONA)
  Wire.beginTransmission(MCP_ADDR);
  Wire.write(0x08);
  Wire.write(0xFF); //Ew. 0x00
  Wire.endTransmission();

// Enable pull-ups on GPA (GPPUA)
  Wire.beginTransmission(MCP_ADDR);
  Wire.write(0x0C);
  Wire.write(0xFF); //Git
  Wire.endTransmission();
// Clear pending interrupt
  while(digitalRead(INTPin)==LOW){
    Wire.beginTransmission(MCP_ADDR);
    Wire.write(0x10);
    Wire.endTransmission();
    Wire.requestFrom(MCP_ADDR,1);  
    Wire.read();
  }
////////////////////////////////////////////////////////////////// Shift register init
////////////////////////////////////////////////////////////////// Thermometer init
}
