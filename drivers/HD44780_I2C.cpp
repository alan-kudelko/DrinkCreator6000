#include <util/delay.h>
#include <HD44780_I2C.h>
#include <i2c.h>

HD44780_LCD::HD44780_LCD(uint8_t address,uint8_t cols,uint8_t rows,uint8_t charsize){
    _address=address;
    _cols=cols;
    _rows=rows;
    _charsize=charsize;
    _backlightval=LCD_BACKLIGHT;
}

HD44780_LCD::~HD44780_LCD(){

}

// Blocking methods

void HD44780_LCD::begin_blocking(){
    // Check if i2c is enabled

    _displayfunction=LCD_4BITMODE|LCD_1LINE|LCD_5x8DOTS;

    if(_rows>1){
        _displayfunction|=LCD_2LINE;
    }

    if((_charsize!=0)&&(_rows==1)){
		_displayfunction|=LCD_5x10DOTS;
	}

    _delay_ms(50);

    expanderWrite_blocking(_backlightval);
    _delay_ms(1000);

    write4bits_blocking(0x03<<4);
    _delay_us(5000);

    write4bits_blocking(0x03<<4);
    _delay_us(5000);

    write4bits_blocking(0x03<<4);
    _delay_us(5000);

    write4bits_blocking(0x02<<4);

    command_blocking(LCD_FUNCTIONSET|_displayfunction);

    _displaycontrol=LCD_DISPLAYON|LCD_CURSOROFF|LCD_BLINKOFF;

    display_blocking();

    clear_blocking();

    _displaymode=LCD_ENTRYLEFT|LCD_ENTRYSHIFTDECREMENT;

    command_blocking(LCD_ENTRYMODESET|_displaymode);

    home_blocking();
}

void HD44780_LCD::clear_blocking(){
    command_blocking(LCD_CLEARDISPLAY);
    _delay_us(2000);
}

void HD44780_LCD::home_blocking(){
    command_blocking(LCD_RETURNHOME);
    _delay_us(2000);
}

void HD44780_LCD::noBacklight_blocking(){
	_backlightval=LCD_NOBACKLIGHT;
	expanderWrite_blocking(0);
}

void HD44780_LCD::backlight_blocking(){
	_backlightval=LCD_BACKLIGHT;
	expanderWrite_blocking(0);
}

void HD44780_LCD::setCursor_blocking(uint8_t col,uint8_t row){
	int row_offsets[]={0x00,0x40,0x14,0x54};
	if(row>_rows){
		row=_rows-1;
	}
	command_blocking(LCD_SETDDRAMADDR|(col+row_offsets[row]));    
}

void HD44780_LCD::noCursor_blocking(){
	_displaycontrol&=~LCD_CURSORON;
	command_blocking(LCD_DISPLAYCONTROL|_displaycontrol);
}

void HD44780_LCD::cursor_blocking(){
	_displaycontrol|=LCD_CURSORON;
	command_blocking(LCD_DISPLAYCONTROL|_displaycontrol);
}

void HD44780_LCD::noBlink_blocking(){
	_displaycontrol&=~LCD_BLINKON;
	command_blocking(LCD_DISPLAYCONTROL|_displaycontrol);
}

void HD44780_LCD::blink_blocking(){
	_displaycontrol|=LCD_BLINKON;
	command_blocking(LCD_DISPLAYCONTROL|_displaycontrol);
}

void HD44780_LCD::scrollDisplayLeft_blocking(){
	command_blocking(LCD_CURSORSHIFT|LCD_DISPLAYMOVE|LCD_MOVELEFT);
}

void HD44780_LCD::scrollDisplayRight_blocking(){
    command_blocking(LCD_CURSORSHIFT|LCD_DISPLAYMOVE|LCD_MOVERIGHT);
}

void HD44780_LCD::leftToRight_blocking(){
	_displaymode|=LCD_ENTRYLEFT;
	command_blocking(LCD_ENTRYMODESET|_displaymode);
}

void HD44780_LCD::rightToLeft_blocking(){
	_displaymode&=~LCD_ENTRYLEFT;
	command_blocking(LCD_ENTRYMODESET|_displaymode);
}

void HD44780_LCD::autoscroll_blocking(){
    _displaymode|=LCD_ENTRYSHIFTINCREMENT;
	command_blocking(LCD_ENTRYMODESET|_displaymode);
}

void HD44780_LCD::noAutoscroll_blocking(){
    _displaymode&=~LCD_ENTRYSHIFTINCREMENT;
	command_blocking(LCD_ENTRYMODESET|_displaymode);
}

void HD44780_LCD::noDisplay_blocking(){
	_displaycontrol&=~LCD_DISPLAYON;
	command_blocking(LCD_DISPLAYCONTROL|_displaycontrol);
}

void HD44780_LCD::display_blocking(){
	_displaycontrol|=LCD_DISPLAYON;
	command_blocking(LCD_DISPLAYCONTROL|_displaycontrol);
}

void HD44780_LCD::createChar_blocking(uint8_t location, uint8_t charmap[]) {
	location&=0x7;
	command_blocking(LCD_SETCGRAMADDR|(location<<3));
	for(uint8_t i=0;i<8;i++) {
		write_blocking(charmap[i]);
	}
}

void HD44780_LCD::load_custom_character_blocking(uint8_t char_num,uint8_t*rows){
    createChar_blocking(char_num,rows);
}

void HD44780_LCD::command_blocking(uint8_t value){
    send_blocking(value,0);
}

void HD44780_LCD::write_blocking(uint8_t value){
    send_blocking(value,Rs);
    // return 1;
}

void HD44780_LCD::send_blocking(uint8_t value,uint8_t mode){
	uint8_t highnib=value&0xf0;
	uint8_t lownib=(value<<4)&0xf0;
	write4bits_blocking((highnib)|mode);
	write4bits_blocking((lownib)|mode);
}

void HD44780_LCD::write4bits_blocking(uint8_t value){
    expanderWrite_blocking(value);
    _delay_us(2);
    pulseEnable_blocking(value);
}

void HD44780_LCD::expanderWrite_blocking(uint8_t value){
    _delay_us(2);
    i2c_write_byte_blocking(_address,value|_backlightval);
}

void HD44780_LCD::pulseEnable_blocking(uint8_t value){
    expanderWrite_blocking(value|En);
    _delay_us(2);
    expanderWrite_blocking(value&~En);
}

// non blocking methods

bool HD44780_LCD::getBacklight_non_Blocking(){
    return _backlightval==LCD_BACKLIGHT;
}