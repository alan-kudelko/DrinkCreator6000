#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <avr/eeprom.h>

#include <DrinkCreator6000_Init.h>
#include <DrinkCreator6000_Progmem.h>
#include <DrinkCreator6000_Config.h>
#include <DrinkCreator6000_Pins.h>

#include <uart.h>
#include <i2c.h>

extern "C" void initRamSize(void);

void initializeUART(void){
    MCUSR=0;
    wdt_disable();

    uart_init();
    sei();
    _delay_ms(50);

    uart_puts_P_blocking(msg_UartReady);
}

//////////////////////////////////////////////////////////////////
// IO initialization:
// Configures I/O pins and attaches interrupts related to pin events
void initializeIO(){
    cli();
    // Initialize keyboard interrupt pin as INPUT
    DDRD&=~(1<<KEYBOARD_INT_PIN);
    // Initialize MCP23008 ic reset pin as output
    // Disabled by default
    DDRD|=(1<<KEYBOARD_RESET_PIN);
    PORTD|=(1<<KEYBOARD_RESET_PIN);
    // Initialize peltier2 pin as OUTPUT
    DDRD|=(1<<PELTIER1_PIN);
    DDRD|=(1<<PELTIER2_PIN);
    PORTD&=~(1<<PELTIER1_PIN);
    PORTD&=~(1<<PELTIER2_PIN);    
    // Initialize LED ring pins
    // To be done
    DDRC|=(1<<PC5);
    DDRC|=(1<<PC4);

    // Initialize buffer pin as OUTPUT
    // and configure buzzers frequency
    DDRB|=(1<<BUZZER_PIN);
    PORTB&=~(1<<BUZZER_PIN);
    TCCR1A=0;
    TCNT1=0;
    OCR1A=249;
    TCCR1B|=(1<<WGM12);
    TCCR1B|=(1<<CS11)|(1<<CS10);
    // Initialize ATmega328p ready pin
    DDRB&=~(1<<TEMPDATA_RDY);
    // Verify pins of 74HC595
    // Initialize shift register's DS pin as OUTPUT
    DDRC|=(1<<DS_PIN);
    PORTC&=~(1<<DS_PIN);
    // Initialize shift register's ST pin as OUTPUT
    DDRC|=(1<<ST_PIN);
    PORTC&=~(1<<ST_PIN);
    // Initialize shift register's SH pin as OUTPUT
    DDRC|=(1<<SH_PIN);
    PORTC&=~(1<<SH_PIN);
    // Initialize shift register's OE pin as OUTPUT
    // By default pulled high (Disabled)
    DDRC|=(1<<OE_PIN);
    PORTC|=(1<<OE_PIN);
    // Initialize circulation pump pin
    // Disabled by default
    DDRE|=(1<<CIRCULATION_PUMP_PIN);
    PORTE&=~(1<<CIRCULATION_PUMP_PIN);
    // Initialize fans pin
    // Disabled by default
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
    i2c_init();
    i2c_enable();
    uart_puts_P_blocking(msg_I2CReady);

    lcd.begin_blocking();
    _delay_ms(3);
    lcd.backlight_blocking();
    uart_puts_P_blocking(msg_lcdReady);

    //mcp.begin_blocking();
// Temporarly until class api is added
    PORTD&=~(1<<KEYBOARD_RESET_PIN);
    _delay_us(100);
    PORTD|=(1<<KEYBOARD_RESET_PIN);
    _delay_us(100);

    uint8_t data[2]={0};
    // IOCON -> configuration register
    data[0]=0x05;
    data[1]=(1<<5)|(1<<4);
    i2c_write_bytes_to_address_blocking(MCP_ADDR,data,2);
    _delay_us(100);
    // IPOL -> read state inverted
    data[0]=0x01;
    data[1]=0xff;
    i2c_write_bytes_to_address_blocking(MCP_ADDR,data,2);
    _delay_us(100);
    // GPINTEN -> interrupts enabled for all pins   
    data[0]=0x02;
    data[1]=0xff;
    i2c_write_bytes_to_address_blocking(MCP_ADDR,data,2);
    _delay_us(100);
    // DEFVAL -> default value high state
    data[0]=0x03;
    data[1]=0x00;
    i2c_write_bytes_to_address_blocking(MCP_ADDR,data,2);
    _delay_us(100);
    // INTCON -> interrupt when gpio is different from defval
    data[0]=0x04;
    data[1]=0xff;
    i2c_write_bytes_to_address_blocking(MCP_ADDR,data,2);
    _delay_us(100);
    // GPPU
    data[0]=0x06;
    data[1]=0xff;
    i2c_write_bytes_to_address_blocking(MCP_ADDR,data,2);
    _delay_us(100);

    i2c_read_reg_from_adddress_blocking(MCP_ADDR,0x09);

    uart_puts_P_blocking(msg_mcpReady);

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