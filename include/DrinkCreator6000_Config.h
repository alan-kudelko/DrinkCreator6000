/**
 * @file DrinkCreator6000_Config.h
 * @brief Centralized configuration header for the DrinkCreator6000 system, containing global C++ variables.
 *
 * This header defines project-wide configuration macros and declares global
 * C++ objects used across the DrinkCreator6000 project, such as:
 *   - HD44780_LCD controller object
 *   - MCP2300XX I/O expander object
 *
 * @note These are global variables in C++ and accessible from any module
 *       including this header. 
 */
#ifndef _DRINK_CREATOR6000_CONFIG_H_
    #define _DRINK_CREATOR6000_CONFIG_H_

#include <HD44780_I2C.h>


// Task stacks and guard zones
//////////////////////////////////////////////////////////////////
// Screen data:
// LCD dimensions and I2C address

extern HD44780_LCD lcd;
// Screen identifiers and navigation overview
/*
    Screen transition diagram:

          +-------------+
          | 0 Welcome   |
          +------+------+  
                 |
                 v
          +-------------+  
          | 1 Drink     |  (scrollable)
          | Select      |
          +------+------+  
             /       \
            v         v
      +-------------+  +-------------+  (scrollable)
      | 2 Drink     |  | 3 Show      |
      | Order       |  | System Info |
      +-------------+  +------+------+
                              |
                              v
                   +------------------+
                   | 4 Show Last      |  (scrollable)
                   | Error            |
                   +------------------+
                              |
                              v
                   +------------------+
                   | 5 Test pumps     |  (scrollable)
                   |                  |
                   +------------------+

    Legend:
    - (scrollable) marks screens that support scrolling
    - (editable) marks screen that supports editing
*/


#endif // _DRINK_CREATOR6000_CONFIG_H_
