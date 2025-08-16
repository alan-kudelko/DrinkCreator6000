#ifndef _DRINK_CREATOR6000_CONFIG_H_
  #define _DRINK_CREATOR6000_CONFIG_H_

#include <DrinkCreator6000_Config_C.h>


// Task stacks and guard zones
//////////////////////////////////////////////////////////////////
// Screen data:
// LCD dimensions and I2C address


#define E_LOADING_BAR 17
// LCD custom characters

#define SHOW_INFO_MENUS_COUNT 4
// Show info submenu count

#define WELCOME_SCREEN 0
#define DRINK_SELECT 1
#define DRINK_ORDER 2
#define SHOW_INFO 3
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
