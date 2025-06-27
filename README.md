# DrinkCreator6000 – RTOS System on Custom AVR Board

Welcome to DrinkCreator6000, a real-time operating system (RTOS) project designed for a custom-built drink dispensing machine powered by AVR microcontroller and FreeRTOS. The system is built entirely using static memory allocation for robustness and predictability.

This embedded system coordinates modular tasks like temperature regulation, error handling, user interaction, and diagnostics — all running concurrently on a statically managed RTOS.

> 🔧 Status: In development
> 🧪 Goal: Create a fully functional, physical drink machine and explore structured multi-tasking using FreeRTOS on bare-metal AVR.

---

## 🧱 Project Structure

All logic is implemented in statically allocated FreeRTOS tasks running on a custom PCB with AVR hardware. The system is modular and designed to support precise debugging and future expansion.

## 🛠️ Hardware and Libraries Requirements

### Hardware
- ATmega2560 / ATmega2561 microcontroller — or an Arduino Mega board for prototyping convenience
- LCD 2004 display with I²C backpack (e.g., based on HD44780, PCA9633, or AiP31068)
- 74HC595 shift register for pump control
- PCF8574N I²C I/O expander for keypad

### Software
- Arduino IDE (used for development and uploading)
- Arduino FreeRTOS library (adds multitasking and RTOS features)
- LiquidCrystal_I2C library (compatible with the I²C LCD driver used)
- avr-libc (AVR C runtime, typically included with Arduino toolchain)

### 🧵 Task Overview

| Task ID | Task Name           | Description                                                                 |
|---------|---------------------|-----------------------------------------------------------------------------|
| 00      | `taskErrorHandler`  | Handles critical faults like stack overflows and logs errors to EEPROM.     |
| 01      | `taskStackDebugger` | Monitors stack and RAM usage across all tasks.                             |
| 02      | `taskMain`          | Coordinates the system, manages high-level logic, and activates tasks.      |
| 03      | `taskUpdateScreen`  | Periodically updates the LCD with system data (RAM, uptime, diagnostics).   |
| 04      | `taskRegulateTemp`  | Controls heating/cooling to maintain the target fluid temperature.          |
| 05      | `taskReadInput`     | Handles input devices like buttons or encoders and passes events to the main task. |
| 06      | `taskSelectDrink`   | Displays the current drink selection and related status.                    |
| 07      | `taskOrderDrink`    | Processes drink orders and manages state transitions for dispensing.        |
| 08      | `taskShowInfo`      | Displays error details on the LCD after a system reset due to failure.      |
| 09      | `taskShowTemp`      | Shows the current measured temperature on the LCD.                         |
| 10      | `taskShowLastError` | Presents recent errors, task states, and priority information on the LCD.  |

---

### 📊 RAM Usage Overview (Start, End, Size)

| Region    | Start Address | End Address | Size (bytes) |
|-----------|---------------|-------------|--------------|
| .bss      | 0x0200        | 0x0688      | 1160         |
| .data     | 0x0688        | 0x17EE      | 4454         |
| Heap      | 0x17EE        | 0x17EE      | 0            |
| CPU Stack | 0x2165        | 0x21FF      | 154          |

**Total free memory:** 2423 bytes

*Note:*  
- FreeRTOS task stacks are statically allocated and included in the `.data` segment size.  
- CPU Stack refers to the main processor stack (not individual task stacks).

### 💾 EEPROM Memory Map

| Address (hex) | Size (bytes) | Description                       |
|---------------|--------------|-----------------------------------|
| 0x0000        | 1            | Number of drinks in memory (n)    |
| 0x0001        | 34 * n       | Drinks data (n ≤ 26)              |
| 0x0400        | 4            | Temperature set in freezer        |
| 0x0404        | 4            | Temperature hysteresis width      |
| 0x0800        | 134          | Last saved error                  |
| 0x0C00        | 2            | Bootups count                     |

---

## 🧠 Design Goals

- 🎯 Explore real-time scheduling and modular task separation
- 💾 Use 100% static memory allocation (no malloc, no heap)
- 🧰 Track system stability via runtime task/memory debug tools
- 🔁 Ensure recovery after failure using EEPROM fault logging
- 📟 Provide full system visibility through LCD diagnostics and monitoring
- 🧪 Serve as a practical testbed for FreeRTOS and embedded RTOS design
- 📚 Designed as an educational project to deepen understanding of multitasking, resource sharing, and fail-safe system design in bare-metal embedded systems
- 🧩 Implement low-level memory management techniques for optimized and reliable resource control

---

## 🗺️ Roadmap

- [X] Create custom PCB with AVR MCU and additional components
- [X] Create functions for static allocation of Queues, Mutexes, and Semaphores
- [X] Create function for displaying current RAM usage via serial monitor
- [X] Create function for debugging the last unconfirmed error stored in EEPROM
- [X] Create function for displaying the boot count from EEPROM via serial port
- [X] Create stackOverflowHook for handling stack overflow errors
- [X] Create task for handling critical system errors such as stack overflows and logging them to EEPROM
- [X] Create task for debugging stack usage and runtime status of all tasks via serial monitor
- [ ] Create main task for coordinating other tasks
- [X] Create task for handling regular LCD updates
- [X] Create task for regulating temperature inside the freezer
- [X] Create task for handling keyboard input from PCF8574
- [ ] Create task for selecting the drink to be ordered
- [ ] Create task for processing the ordered drink (pump activation)
- [X] Create task to display project information such as author, startup count, and current runtime
- [ ] Create task to display current temperature settings and actual temperature inside the freezer
- [ ] Create task to display and confirm the last saved error
- [ ] Implement software guard zones between task stacks for added protection and reliability
- [ ] Review .map file and optimize memory by efficient variable placement using linker script (.ld file)
---
