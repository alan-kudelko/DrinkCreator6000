#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <avr/eeprom.h>

#include <DrinkCreator6000_Init.h>
#include <DrinkCreator6000_Progmem.h>
#include <DrinkCreator6000_Config.h>

#include <uart.h>
#include <i2c.h>

extern "C" void initRamSize(void);

void initializeUART(void){
    uart_init();
    sei();
    _delay_ms(10);

    uart_puts_P_blocking(msg_UartReady);
}

//////////////////////////////////////////////////////////////////
// IO initialization:
// Configures I/O pins and attaches interrupts related to pin events
void initializeIO(){
    cli();
    // Initialize keyboard interrupt pin as INPUT with PULLUP
    DDRB&=~(1<<KEYBOARD_INT_PIN);
    PORTB|=(1<<KEYBOARD_INT_PIN);
    // Initialize peltier2 pin as OUTPUT
    DDRB|=(1<<PELTIER2_PIN);
    // Initialize buffer pin as OUTPUT
    DDRB|=(1<<BUZZER_PIN);
    PORTB&=~(1<<BUZZER_PIN);

    // Verify pins of 74HC595
    // Initialize shift register's DS pin as OUTPUT
    DDRE|=(1<<DS_PIN);
    PORTB&=~(1<<DS_PIN);
    // Initialize shift register's ST pin as OUTPUT
    DDRE|=(1<<ST_PIN);
    PORTB&=~(1<<ST_PIN);
    // Initialize shift register's SH pin as OUTPUT
    DDRE|=(1<<SH_PIN);
    PORTB&=~(1<<SH_PIN);
    // Initialize circulation pump pin as OUTPUT
    DDRE|=(1<<CIRCULATION_PUMP_PIN);
    PORTB&=~(1<<CIRCULATION_PUMP_PIN);
    // Initialize fans pin as OUTPUT
    DDRE|=(1<<FANS_PIN);
    PORTE&=~(1<<FANS_PIN);

    sei();

    uart_puts_P_blocking(msg_IOInitialized);
}

void initializeEEPROM(void){
    cli();
    while(!eeprom_is_ready());
    sei();
    uart_puts_P_blocking(msg_EEPROMReady);
}
//////////////////////////////////////////////////////////////////
// Memory initialization:
// Static allocation of stacks, queues, semaphores, and mutexes
void initializeMemory(){
        cli();
      initRamSize();
      sei();
      uart_puts_P_blocking(msg_RamSizeInitialized);

        cli();
      qScreenData=xQueueCreateStatic(SCREEN_QUEUE_BUFFER_COUNT,sizeof(sScreenData),screenQueueBuffer,&screenQueueStructBuffer);
      qKeyboardData=xQueueCreateStatic(KEYBOARD_QUEUE_BUFFER_COUNT,sizeof(uint8_t),keyboardQueueBuffer,&keyboardQueueStructBuffer);
      qErrorId=xQueueCreateStatic(ERROR_ID_QUEUE_BUFFER_COUNT,sizeof(TaskHandle_t),errorIdQueueBuffer,&errorIdQueueStructBuffer);
  
      sem_ReadData=xSemaphoreCreateBinaryStatic(&semReadDataBuffer);
      mux_I2CLock=xSemaphoreCreateMutexStatic(&muxI2CLockBuffer);
      mux_SerialLock=xSemaphoreCreateMutexStatic(&muxSerialLockBuffer);

      memset((void*)&UI_Context,0,sizeof(UI_Context));
  
      memset((void*)guardZone0, MEMORY_FILL_PATTERN,GUARD_ZONE_SIZE);
      memset((void*)guardZone1, MEMORY_FILL_PATTERN,GUARD_ZONE_SIZE);
      memset((void*)guardZone2, MEMORY_FILL_PATTERN,GUARD_ZONE_SIZE);
      memset((void*)guardZone3, MEMORY_FILL_PATTERN,GUARD_ZONE_SIZE);
      memset((void*)guardZone4, MEMORY_FILL_PATTERN,GUARD_ZONE_SIZE);
      memset((void*)guardZone5, MEMORY_FILL_PATTERN,GUARD_ZONE_SIZE);
      memset((void*)guardZone6, MEMORY_FILL_PATTERN,GUARD_ZONE_SIZE);
      memset((void*)guardZone7, MEMORY_FILL_PATTERN,GUARD_ZONE_SIZE);
      memset((void*)guardZone8, MEMORY_FILL_PATTERN,GUARD_ZONE_SIZE);
      memset((void*)guardZone9, MEMORY_FILL_PATTERN,GUARD_ZONE_SIZE);
      memset((void*)guardZone10,MEMORY_FILL_PATTERN,GUARD_ZONE_SIZE);
      memset((void*)guardZone11,MEMORY_FILL_PATTERN,GUARD_ZONE_SIZE);
        sei();
      uart_puts_P_blocking(msg_MemoryInitialized);
}
//////////////////////////////////////////////////////////////////
// Hardware initialization:
// Initializes Serial, LCD display, I²C keyboard (MCP23017), 
// shift register (74HC595), and temperature sensor
void initializeHardware(){
////////////////////////////////////////////////////////////////// LCD init	
    cli();
    i2c_init();
    sei();
    uart_puts_P_blocking(msg_I2CReady);
    //lcd.begin();
    //lcd.backlight();
////////////////////////////////////////////////////////////////// Keyboard init	  
// Configure IOCON register SEQOP enabled (NOSEQOP), BANK=0
//   Wire.beginTransmission(MCP_ADDR);
//   Wire.write(0x0A);
//   Wire.write(0x20);
//   Wire.endTransmission();

// // Set GPA as inputs (IODIRA)
//   Wire.beginTransmission(MCP_ADDR);
//   Wire.write(0x00);
//   Wire.write(0xFF); //Git
//   Wire.endTransmission();  

// // Invert polarity: LOW = pressed (IPOLA)
//   //Wire.beginTransmission(MCP_ADDR);
//   //Wire.write(0x02);
//   //Wire.write(0xFF); //Git
//   //Wire.endTransmission();

// // Enable interrupt-on-change on all GPA pins (GPINTENA)
//   Wire.beginTransmission(MCP_ADDR);
//   Wire.write(0x04);
//   Wire.write(0xFF); //Git
//   Wire.endTransmission();

// // Set default comparison value (HIGH) (DEFVALA)
//   Wire.beginTransmission(MCP_ADDR);
//   Wire.write(0x06);
//   Wire.write(0xFF); //Git
//   Wire.endTransmission();

// // Configure interrupt on mismatch with DEFVAL (INTCONA)
//   Wire.beginTransmission(MCP_ADDR);
//   Wire.write(0x08);
//   Wire.write(0xFF); //Ew. 0x00
//   Wire.endTransmission();

// // Enable pull-ups on GPA (GPPUA)
//   Wire.beginTransmission(MCP_ADDR);
//   Wire.write(0x0C);
//   Wire.write(0xFF); //Git
//   Wire.endTransmission();
// // Clear pending interrupt
//   while(digitalRead(INTPin)==LOW){
//     Wire.beginTransmission(MCP_ADDR);
//     Wire.write(0x10);
//     Wire.endTransmission();
//     Wire.requestFrom(MCP_ADDR,1);  
//     Wire.read();
//   }
////////////////////////////////////////////////////////////////// Shift register init
////////////////////////////////////////////////////////////////// Thermometer init
}
void initializeInterrupts(void){
    // Yet to be implemented
    uart_puts_P_blocking(msg_InterruptsAttached);
}

void init8(void){
    initializeUART();
    initializeIO();
    initializeEEPROM();
    initializeMemory();
    initializeHardware();
    initializeInterrupts();
}