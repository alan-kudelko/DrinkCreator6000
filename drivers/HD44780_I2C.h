/**
 * @file HD44780_I2C.h
 * @brief Command and flag definitions for controlling HD44780-based character LCDs via I²C.
 *
 * This header provides command opcodes, control flags, and bit masks for driving
 * HD44780-compatible LCD modules (e.g., 1602, 2004) through an I²C expander such as PCF8574.
 *
 * It is designed for use with a dedicated LCD control class that implements both
 * blocking and non-blocking write operations. The implementation will also support
 * configurable timing parameters between write operations to meet various LCD
 * module speed requirements.
 *
 * Compatible display types:
 *  - 1602 (16x2 characters)
 *  - 2004 (20x4 characters)
 *  - Other HD44780-based modules
 *
 * Features:
 *  - Blocking write functions for simple use-cases
 *  - Non-blocking write functions for real-time applications (e.g., with FreeRTOS)
 *  - Configurable inter-command and inter-data delays for different LCD timings
 */
#ifndef _HD44780_I2C_H_
    #define _HD44780_I2C_H_

#include <stdint.h>

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define En 0x04  // Enable bit
#define Rw 0x02  // Read/Write bit
#define Rs 0x01  // Register select bit

class HD44780_LCD{
    uint8_t _address;
    uint8_t _rows;
    uint8_t _cols;
    uint8_t _charsize;
    uint8_t _backlightval;
    uint8_t _displaymode;
    uint8_t _displaycontrol;
    uint8_t _displayfunction;
public:
    HD44780_LCD(uint8_t address,uint8_t cols,uint8_t rows,uint8_t charsize=LCD_5x8DOTS);
    ~HD44780_LCD();

    // Blocking methods
    void begin_blocking();
    void clear_blocking();
    void home_blocking();

    void noBacklight_blocking();
    void backlight_blocking();

    void setCursor_blocking(uint8_t col,uint8_t row);
    void noCursor_blocking();
    void cursor_blocking();
    void noBlink_blocking();
    void blink_blocking();

    void scrollDisplayLeft_blocking();
    void scrollDisplayRight_blocking();

    void leftToRight_blocking();
    void rightToLeft_blocking();
    void autoscroll_blocking();
    void noAutoscroll_blocking();

    void noDisplay_blocking();
    void display_blocking();

    void createChar_blocking(uint8_t location,uint8_t*charmap);
    void load_custom_character_blocking(uint8_t char_num,uint8_t*rows);

    void command_blocking(uint8_t value);
    void write_blocking(uint8_t value);

    // non blocking methods

    bool getBacklight_non_Blocking();
private:
    // Low level operations

    void send_blocking(uint8_t value,uint8_t mode);
    void write4bits_blocking(uint8_t value);
    void expanderWrite_blocking(uint8_t value);
    void pulseEnable_blocking(uint8_t value);
};

#endif // _HD44780_I2C_H_