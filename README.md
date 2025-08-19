# DrinkCreator6000 – RTOS System on Custom AVR Board
DrinkCreator6000 is a custom-built, FreeRTOS-based drink dispensing system that runs on an ATmega2561 AVR microcontroller mounted on a custom-made PCB. The entire system has been engineered from scratch, combining real-time software, robust hardware design, and a structured task-based architecture.

The project runs under FreeRTOS with fully static memory allocation, ensuring high predictability and resilience. A custom memory section .tdat, defined in the linker script, is used to allocate all task stacks and their associated guard zones contiguously in SRAM, enabling precise monitoring and deterministic stack overflow detection.

The project was originally developed using the Arduino framework and libraries; however, these have been completely removed, as the Arduino IDE and Arduino libraries do not provide full control over the hardware and often hide critical implementation details. The build system has been migrated to Visual Studio Code with CMake, using a clean AVR toolchain for maximum transparency and flexibility. Additionally, the firmware is being refactored to comply with the MISRA C:2025 standard, enhancing code safety, readability, and portability in line with industry best practices.

The user interface is presented on a 2004 character LCD driven via I²C, offering a multi-screen menu system for navigating functions such as drink selection, system diagnostics, and memory usage. Input is accepted both from a physical keypad connected through an MCP23017 I²C I/O expander and from the UART interface, allowing full control either physically or remotely.

The system includes runtime diagnostics for RAM and CPU usage, task stack monitoring, and last-error reporting stored in EEPROM. These diagnostics are continuously available via UART and can also be accessed from the LCD interface through a dedicated submenu.
A dedicated error-handling task monitors guard zones around each task stack to detect overflows or memory corruption. If a fault is detected, the system logs detailed error metadata to EEPROM, displays the issue on-screen and via UART, and will automatically restart after a defined timeout (planned feature).

Temperature sensing is handled via a 1-Wire digital thermometer, and internal cooling is provided by two Peltier elements, a circulating pump, and a dedicated aluminum heat sink system. The cooling chamber is fully enclosed in polystyrene foam and lined with sealed aluminum sheets, making it waterproof and thermally isolated.

The system supports hardware test functions, allowing verification of pumps, Peltier modules, and the circulation system. All components can be tested interactively either via keypad or over UART.

The entire unit is powered by a modified ATX computer power supply, delivering stable 5V and 12V rails for logic and high-current subsystems.


> 🔧 Status: In development  
> 🧪 Goal: Create a fully functional, physical drink machine and explore structured multi-tasking using FreeRTOS AVR MCU.

---

## ✳️ Technologies & Tools

This project integrates a wide range of hardware, software, and system-level concepts typically found in professional embedded systems. Below is a breakdown of the core technologies and components involved in the design and implementation of DrinkCreator6000:

### 🔌 Hardware & Electronics
- Custom-designed PCB with ATmega2561 AVR microcontroller
- 2004 I²C LCD display for UI rendering
- MCP23017 I²C I/O expander for keypad handling
- 74HC595 shift register to control peristaltic pumps
- Peltier modules for cooling with dedicated water circulation pump
- 1-Wire digital thermometer for internal temperature measurement
- Modified 400 W ATX power supply (5V / 12V rails)
- Fully enclosed, thermally insulated cooling chamber (polystyrene foam + aluminum lining, waterproof)

### 🧠 System Architecture & Concepts
- Real-time system based on FreeRTOS
- Static memory allocation only — no malloc, no heap fragmentation
- Custom linker script with a dedicated .tdat memory section for task stacks and guard zones
- Preemptive multitasking with structured task separation
- Guard zone-based stack overflow detection
- EEPROM-based error logging and system state preservation
- Soft watchdog / recovery logic (planned restart on fatal fault)

### 💬 Input / Output Interfaces
- UART serial interface for remote monitoring, control, and debugging
- I²C bus for LCD and keypad communication
- Dual input support: physical keypad and UART commands for full UI navigation
- Multi-level LCD menu system for selecting drinks, viewing diagnostics, and testing hardware

### 🧰 Development Tools & Libraries
- C / C++ for firmware
- Native AVR libraries (avr-libc, low-level register access)
- FreeRTOS (AVR port)
- VS Code, CMake, Ninja
- Doxygen for automatic code documentation
- Firmware is being gradually refactored to comply with the MISRA C:2025 standard to improve code safety, readability, and long-term maintainability

### 📚 Documentation
The source code is fully documented using Doxygen, which generates up-to-date, browsable documentation from the annotated source files. The generated docs can be found in the docs/ directory and are updated as part of the build process.

---

## 🧠 Design Goals

- 🎯 Explore real-time scheduling and modular task separation
- 💾 Use 100% static memory allocation (no malloc, no heap)
- 🧰 Track system stability via runtime task/memory debug tools
- 🔁 Ensure recovery after failure using EEPROM fault logging
- 📟 Provide full system visibility through LCD diagnostics and monitoring
- 🧪 Serve as a practical testbed for FreeRTOS and embedded RTOS design
- 📚 Designed as an educational project to deepen understanding of multitasking, resource sharing, and fail-safe system design embedded systems
- 🧩 Implement low-level memory management techniques for optimized and reliable resource control
- 🧱 Integrate tightly with custom hardware (PCB, sensors, pumps, display)
- 🕒 Guarantee deterministic execution and timing across tasks

---

## 🖼️ Visual Overview

This section provides a visual presentation of the DrinkCreator6000 project, including photographs of the assembled machine, its hardware components, and screenshots illustrating the system’s operation. It aims to deliver a comprehensive understanding of the device’s physical design and functional behavior.

### 🎬 UI interface Demo

[![DrinkCreator6000 UI Demo](https://img.youtube.com/vi/Mg_Gc56w8Ac/0.jpg)](https://www.youtube.com/watch?v=Mg_Gc56w8Ac)

### 🎬 Project Demo

(Will be here soon)

---

## 📟 UI Flow & Screens

| ID | Screen               | Description                                                                 |
|----|----------------------|-----------------------------------------------------------------------------|
| 0  | **Welcome Screen**    | Displays the project name, firmware version, and boot count.               |
| 1  | **Drink Select Screen** | Shows the current drink name, ingredients, and related info.              |
| 2  | **Drink Order Screen**  | Displays dispensing progress, drink name, and ETA.                        |
| 3  | **Show Info Screen** | Displays general system status including uptime, firmware version, boot count, author, freezer temperature, RAM usage, and task stack diagnostics. |
| 4  | **Show Task Stack Info Screen** | Displays detailed information about FreeRTOS task stacks, including task names, priorities, and high-water marks (minimum remaining stack). |
| 5  | **Show Last Error Screen**    | Displays last unconfirmed error stored in EEPROM  |

Screen transition diagram:
  
                            ╔════════════════════╗  
                            ║Drink Creator 6000  ║   
    0 Welcome Screen        ║Initializing...     ║ 
                            ║Please wait         ║                         
                            ║[#####-----]  50 %  ║   
                            ╚════════════════════╝ 
                                      ║
                                      ║
                                      ▼                         2 Drink Order Screen                    Submenu[1]
                            ╔════════════════════╗             ╔════════════════════╗             ╔════════════════════╗
                            ║[01]Test Drink      ║             ║[01]Test Drink      ║ Submenu[1]  ║[01]Test Drink      ║
    1 Drink Select Screen   ║Whiskey       50[ml]║  ═ ═ ═ ═ >  ║                    ║ --------->  ║                    ║
                            ║Rum          100[ml]║             ║Please wait...      ║             ║Done!               ║
                            ║Cola         250[ml]║             ║[########--]  82%   ║             ║[##########] 100%   ║                    
                            ╚════════════════════╝             ╚════════════════════╝             ╚════════════════════╝
                                      ║
                                      ║        +<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------<+
                                      ║        |                                                                                                                                                                                    |
                                      ▼        ▼                                                                                                                                                                                    |
                            ╔════════════════════╗             ╔════════════════════╗            ╔════════════════════╗            ╔════════════════════╗            ╔════════════════════╗            ╔════════════════════╗       |
                            ║Drink Creator 6000  ║  Submenu[1] ║Drink Creator 6000  ║ Submenu[2] ║Drink Creator 6000  ║ Submenu[3] ║RAM Info            ║ Submenu[4] ║RAM Info            ║ Submenu[5] ║RAM Info            ║       |
    3 Show System Info[0]   ║Software ver. 3.0   ║  ---------> ║Current run time    ║ ---------> ║T: 21.2°C  S:11.0°C ║ ---------> ║Usage: 6722 B/8192 B║ ---------> ║.data: 0x0200-0x1522║ ---------> ║HEAP:  0x1BF7-0x1BF7║ ----->+
                            ║Author: Alan Kudelko║             ║21 days  19 h       ║            ║Hyst: 4.0°C         ║            ║[########--]  82%   ║            ║.bss:  0x1522-0x1BF7║            ║STACK: 0x21B5-0x21FF║
                            ║Startup count: 1000 ║             ║39 min   22 s       ║            ║Status: Cooling     ║            ║                    ║            ║Size:  4898 B 1749 B║            ║Size:     0 B   74 B║
                            ╚════════════════════╝             ╚════════════════════╝            ╚════════════════════╝            ╚════════════════════╝            ╚════════════════════╝            ╚════════════════════╝
                                      ║
                                      ║
                                      ▼
                            ╔════════════════════╗             ╔════════════════════╗
                            ║[03]Task information║  Submenu[1] ║[03]Task information║
    4 Show Task Stack Info  ║UPDATE SCREEN       ║  ---------> ║####################║
                            ║Highwater mark:  100║             ║  MEMORY CORRUPTED  ║
                            ║PR:1 State:Suspended║             ║####################║
                            ╚════════════════════╝             ╚════════════════════╝
                                      ║
                                      ║        +<-------------------------------------------+
                                      ║        |                                            |
                                      ▼        ▼                                            |
                            ╔════════════════════╗             ╔════════════════════╗       |
                            ║Stack overflow in t:║ Submenu[1]  ║Error confirmed     ║       |
    5 Show Last Error       ║Error time signature║ --------->  ║EEPROM updated      ║ ----->+
                            ║21 days  19 h       ║             ║                    ║
                            ║39 min   22 s       ║             ║                    ║ 
                            ╚════════════════════╝             ╚════════════════════╝
                                     ║
                                     ║
                                     ▼
                           ╔════════════════════╗              ╔════════════════════╗
                           ║HW Testing Mode     ║  Submenu[1]  ║HW Testing Mode     ║
    6 Test hardware        ║Pumps Test Menu     ║  --------->  ║Pumps Test Menu     ║
                           ║Pump ID: 12345678   ║              ║Pump ID: 12345678   ║
                           ║Status: 0b00000011  ║              ║Status: 0b00000011  ║
                           ╚════════════════════╝              ╚════════════════════╝
			   
![UI Navigation](Media/UI_Flow_Chart.png)

---

## 🗺️ Roadmap

- ✅ Create custom PCB with AVR MCU and additional components
- ✅ Create functions for static allocation of Queues, Mutexes, and Semaphores
- ✅ Create function for displaying current RAM usage via serial monitor
- ✅ Create function for debugging the last unconfirmed error stored in EEPROM
- ✅ Create function for displaying the boot count from EEPROM via serial port
- ✅ Create stackOverflowHook for handling stack overflow errors
- ✅ Create task for handling critical system errors such as stack overflows and logging them to EEPROM
- ✅ Create task for debugging stack usage and runtime status of all tasks via serial monitor
- ✅ Create main task for coordinating other tasks
- ✅ Create task for handling regular LCD updates
- ✅ Create task for regulating temperature inside the freezer
- 🔄 Create task for handling keyboard input from MCP23017 with software debounce
- 🔄 Create task for selecting the drink to be ordered
- ✅ Create welcome screen task to display a greeting message with project name, version, and boot count on the LCD at system startup
- 🔄 Create task for processing the ordered drink (pump activation)
- ✅ Create task to display project information such as author, startup count, and current runtime
- ✅ Implement software guard zones between task stacks for added protection and reliability
- ✅ Review .map file and optimize memory by efficient variable placement using linker script (.ld file)
- ✅ Create a custom memory segment named `.tdat` to store Task Control Blocks (TCBs), task stacks, and stack guard zones by modifying the linker script (.ld file)
- ✅ Implement a guard zone watchdog inside `taskErrorHandler` to detect guard zone corruption, indicating potential stack overflows
- ✅ Separate code into multiple files for better readability
- 🔄 Add EEPROM-based drink recipe loading at startup
- 🔄 Add automatic system reset after fatal system error (e.g. guard zone or memory corruption)
- 🔄 Implement `stopPumps()` function to safely disable all pump outputs
- 🔄 Implement `stopCooler()` function to safely disable the cooling system
- 🔄 Refactor embedded codebase to comply with MISRA C:2025 coding standard for improved safety, portability, and maintainability
- 🔄 Develop custom low-level UART and I2C drivers with queuing support, non-blocking read/write operations, and efficient MCU time usage
- 🔄 Refactor project structure and clean up `#include` dependencies based on Doxygen documentation analysis
- ✅ Migrate project to VS Code, CMake, and Ninja build system with `avr-gcc` toolchain

---

## ⚙️ Technical Overview

### 1. 🛠️ Hardware and Libraries Requirements

#### 1.1 Hardware
- ATmega2560 / ATmega2561 microcontroller — or an Arduino Mega board for prototyping convenience
- LCD 2004 display with I²C backpack (e.g., based on HD44780, PCA9633, or AiP31068)
- 74HC595 shift register for pump control
- MCP23017 I²C I/O expander for keypad

#### 1.2 Software
- Firmware developed in C / C++ using native AVR libraries (`avr-libc`) with low-level register access
- FreeRTOS (AVR port) for real-time multitasking
- Custom drivers for UART and I2C with queuing and non-blocking communication
- Build system based on Visual Studio Code, CMake, and Ninja with `avr-gcc` toolchain
- Doxygen for automatic code documentation generation
- Firmware is gradually refactored to comply with MISRA C:2025 coding standard to improve safety, readability, and maintainability

---

### 2. 🧵 Task Overview

| Task ID | Task Name                  | Description                                                                                                               | Priority | Stack Size | Free Stack |
|---------|----------------------------|---------------------------------------------------------------------------------------------------------------------------|----------|------------|------------|
| 00      | `taskErrorHandler`         | Handles critical faults such as stack overflows and guard zone corruption, and logs errors to EEPROM                      |    3     |    256     |     50     |
| 01      | `taskSerialSystemDebugger` | Monitors stack and RAM usage across all tasks and outputs the data to the serial port                                     |    1     |    270     |     47     |
| 02      | `taskMain`                 | Coordinates the system, manages high-level logic, activates tasks, and handles the current UI context                     |    1     |    200     |    129     |
| 03      | `taskReadInput`            | Reads keyboard data from the MCP23017 I²C I/O expander                                                                    |    2     |    150     |     75     |
| 04      | `taskSerialInput`          | Simulates keyboard input via the serial port for debugging or testing purposes                                            |    2     |    150     |     46     |
| 05      | `taskUpdateScreen`         | Periodically updates the LCD based on current context of the systems                                                      |    1     |    250     |     55     |
| 06      | `taskReadTemp`             | Reads the current temperature inside the freezer and updates a global variable                                            |    1     |    180     |    118     |
| 07      | `taskRegulateTemp`         | Regulates temperature based on the current readings and configured thresholds                                             |    1     |    180     |    118     |
| 08      | `taskSelectDrink`          | Handles drink selection logic and displays in on the LCD                                                                  |    1     |    270     |     95     |
| 09      | `taskOrderDrink`           | Controls the 74HC595 shift register and pump sequence when processing a drink order                                       |    1     |    320     |    175     |
| 10      | `taskShowSystemInfo`       | Displays various system statuses—RAM usage, temperature, task states, boot count, uptime, and last saved error—on the LCD |    1     |    300     |     80     |
| 11      | `taskWelcomeScreen`        | Displays a decorative welcome screen to give the system a more professional appearance                                    |    1     |    222     |     42     |
| 12      | `taskTestHardware`         | Allows for testing of individual pumps, cooling fan, Peltier elements (Not implemented yet)                               |    1     |    222     |      -     |

*Note:*  
- Task stacks will be fine-tuned in the final release
- taskWelcomeScreen and taskTestHardware share the same TCB and stack, as the former is deleted after execution. This reuse is necessary due to limited RAM (~800 bytes remaining). This setup also helps me better understand the behavior of task stack/TCB reuse in constrained memory environments

---

### 3. 📊 RAM Usage Overview (Start, End, Size)

| Region    | Start Address | End Address | Size (bytes) |
|-----------|---------------|-------------|--------------|
| .data     | 0x0200        | 0x0B08      | 2312         |
| .bss      | 0x0B08        | 0x10AC      | 1444         |
| .tdat     | 0x10AC        | 0x1EB0      | 3588         |
| Heap      | 0x1BF7        | 0x1BF7      | 0            |
| CPU Stack | 0x21B5        | 0x21FF      | 154          |

**Total free memory:** 836 bytes

*Note:*  
- FreeRTOS task stacks are statically allocated and included within the `.tdat` segment.
- The CPU Stack refers to the main processor stack (used before the scheduler starts), not individual task stacks.
  
---
### 4. 💾 EEPROM Memory Map

| Address (hex) | Size (bytes) | Description                       |
|---------------|--------------|-----------------------------------|
| 0x0000        | 1            | Number of drinks in memory (n)    |
| 0x0001        | 34 * n       | Drinks data (n ≤ 26)              |
| 0x0400        | 4            | Temperature set in freezer        |
| 0x0404        | 4            | Temperature hysteresis width      |
| 0x0800        | 135          | Last saved error                  |
| 0x0C00        | 2            | Bootups count                     |

---


### 5. Navigation & UI Context  

Navigation within the user interface is managed through a global structure named UI_Context. This structure enables switching between different tasks by activating or deactivating them as necessary. The core control and navigation logic is implemented in the taskMain function.

The UI_Context structure is defined as follows:

    UI_Context{
      uint8_t autoScrollEnable: 1;  // Enables (1) or disables (0) auto-scrolling of the submenu
      uint8_t currentTask: 3;       // Currently active task bound to the LCD (0 – 7)
      uint8_t currentMenu: 3;       // Currently selected menu within the task (0 – 7)
      uint8_t currentSubMenu;       // Currently selected submenu (0 - 255)
    }

This structure stores information about the currently active task — the task responsible for updating the LCD by sending data via a queue to taskUpdateScreen(). It also tracks the currently selected menu and submenu within that task, providing a flexible and memory-efficient mechanism for UI navigation.

When a button is pressed (or a command is simulated via the serial port), taskMain evaluates whether a context switch is necessary. If so, it sends a task notification with a specific value (0 or 1) indicating whether the task should be deactivated or activated.

Upon receiving a deactivation notification, the affected task safely stops its execution. Then, taskMain updates the UI_Context accordingly and notifies the new task to begin its operation.

An example of this control logic is shown below:

    if((*keyboardInput&E_GREEN_BUTTON)==E_GREEN_BUTTON){
      taskENTER_CRITICAL();
      UI_Context->currentTask=DRINK_ORDER;
      UI_Context->currentMenu=0;
      UI_Context->currentSubMenu=0;
      taskEXIT_CRITICAL();
    }
    if((*keyboardInput&E_LWHITE_BUTTON)==E_LWHITE_BUTTON){
      UI_Context->currentSubMenu--;
      xTaskNotify(taskHandles[TASK_SELECT_DRINK],1,eSetValueWithOverwrite);
    }
    if((*keyboardInput&E_RWHITE_BUTTON)==E_RWHITE_BUTTON){
      UI_Context->currentSubMenu++;
      xTaskNotify(taskHandles[TASK_SELECT_DRINK],1,eSetValueWithOverwrite);
    }
    if((*keyboardInput&E_BLUE_BUTTON)==E_BLUE_BUTTON){
      taskENTER_CRITICAL();
      UI_Context->currentTask=SHOW_INFO;
      UI_Context->currentMenu=0;
      UI_Context->currentSubMenu=0;
      taskEXIT_CRITICAL();
      xTaskNotify(taskHandles[TASK_SHOW_SYS_INFO],1,eSetValueWithOverwrite);
      xTaskNotify(taskHandles[TASK_SELECT_DRINK],0,eSetValueWithOverwrite);
    }
    
*Note:*  
- When modifying multiple fields of the UI_Context structure, it is crucial to ensure the operation is "atomic". This means preventing context switches by the scheduler during the update (e.g., by entering a critical section). Without proper protection, concurrent access to UI_Context by multiple tasks may lead to inconsistent states or subtle race conditions that are difficult to debug.

---

### 6. Input Handling & MCP23017  

#### 6.1 Configuration of MCP23017

The MCP23017 is a 16-bit I/O expander IC that provides additional GPIO pins via an I2C interface. It features 22 registers (comprising 11 register pairs) that enable control of 16 pins, organized into two 8-bit ports (Port A and Port B). Its primary advantage lies in its ability to expand the number of input and output lines available to a microcontroller.

However, this IC was selected due to its inclusion of two interrupt pins, which can be utilized to signal when a button press occurs. Moreover, it maintains the previous button states within dedicated registers, allowing the interrupt to be serviced at a later time without the need for immediate response. The stored button states persist until the data is read from the device, ensuring no input events are lost.


#### 6.2 Reading data from MCP23017

Button input handling is implemented using a dedicated FreeRTOS task, which waits on a binary semaphore. This semaphore is released by an interrupt service routine (ISR) whenever the MCP23017 triggers a change on either INTA or INTB. This design ensures that input processing is deferred from the ISR context, minimizing interrupt latency and preserving real-time responsiveness.

When the semaphore is acquired, the task reads the INTCAP (Interrupt Capture) registers, which store the latched state of both Port A and Port B at the exact moment the interrupt occurred. This guarantees that no input event is lost — even if there's a delay between the interrupt and task execution.

The input algorithm implements:

Debouncing: Each press is verified against the real-time GPIOx registers. If the state persists across several iterations, it's accepted as valid.

Hold detection: A button is considered “held” if the value read from INTCAPx matches the current state in GPIOx for a given number of cycles, with delays between checks.

Active-low logic: All inputs are configured as active-low, with internal pull-ups enabled in the MCP23017. A LOW signal indicates a button press.

Deferred execution: No I²C communication occurs inside the ISR. Instead, the ISR only sets the read flag, and the task performs all data handling asynchronously.

This approach provides reliable short-press and long-press detection without the risk of input events being missed. It also ensures that system timing and stability are preserved even under frequent user interaction.

---

### 7. Project Structure & File Overview  

	📦 DrinkCreator6000/
 	│
 	├── Datasheets/      # Documentation of used ICs and components
	├── avr_peripherals/ # Low-level custom hardware drivers (UART, I2C, etc.)
	├── build/           # Build output directory (generated by CMake + Ninja)
	├── docs/            # Documentation generated by Doxygen
	├── FreeRTOS/        # FreeRTOS source files for AVR platform
	├── linker/          # Linker script and custom memory layout files
	├── include/         # Project header files (configuration, data types, tasks, etc.)
	│   ├── DrinkCreator6000_Config.h
	│   ├── DrinkCreator6000_DataTypes.h
	│   ├── DrinkCreator6000_EEPROM.h
	│   ├── DrinkCreator6000_Init.h
	│   ├── DrinkCreator6000_Pins.h
	│   ├── DrinkCreator6000_Progmem.h
	│   ├── DrinkCreator6000_RamStats.h
	│   └── ... (other header files)
	│
	├── src/             # C/C++ source files implementing application logic and FreeRTOS tasks
	│   ├── DrinkCreator6000.cpp
	│   ├── DrinkCreator6000_Config.cpp
	│   ├── DrinkCreator6000_Init.cpp
	│   ├── DrinkCreator6000_EEPROM.c
	│   ├── DrinkCreator6000_Progmem.c
	│   ├── DrinkCreator6000_RamStats.c
	│   └── ... (other source files)
	│
	├── tools/                # Helper tools, including the complete AVR toolchain (avr-gcc, avr-libc)
	│
	├── .vscode/              # Visual Studio Code configuration files
	│
	├── CMakeLists.txt        # CMake build system configuration
	├── build.ninja           # Build system file generated by CMake (Ninja backend)
	├── compile_commands.json # Compilation database for IDE integration
	├── Doxyfile              # Configuration file for Doxygen documentation generation
	├── LICENSE               # Project license file
	├── README.md             # Project overview and documentation
	├── .gitignore            # Git ignore configuration
	└── ... (other config and build files)

---

### 8. Memory Layout & Custom Segments  

#### 8.1 Memory Layout

![Current memory map](Media/ATmega2561_Data_Memory_Map.PNG)

- `__data_start` is a linker symbol representing the starting address of the `.data` section in SRAM on AVR microcontrollers.
- `__data_end` is a linker symbol representing the ending address of the `.data` section in SRAM on AVR microcontrollers.
- `__bss_start` is a linker symbol representing the starting address of the `.bss` section in SRAM on AVR microcontrollers.
- `__bss_end` is a linker symbol representing the ending address of the `.bss` section in SRAM on AVR microcontrollers.
- `__tdat_start` is a linker symbol representing the starting address of the `.tdat` section in SRAM.
- `__tdat_end` is a linker symbol representing the ending address of the `.tdat` section in SRAM.
- `__heap_start` is a linker symbol representing the starting address of the heap section in SRAM.
- `__heap_end` is a C variable defined by me to represent the current end of the heap. Its value is calculated at runtime (see Notes below).
- `__stack_ptr` is a C variable defined by me to capture the initial value of the stack pointer before the RTOS scheduler starts (see Notes below).
- `RAMEND` is a predefined constant representing the last address of SRAM on AVR microcontrollers. For the ATmega2561 used in this project, `RAMEND` is equal to `0x21FF`.

*Note:*
- The `.tdat` section is a custom memory segment defined in the linker script. It is used to store Task Control Blocks (TCBs), task stacks, and associated guard zones. By placing all task stacks contiguously within .tdat, the system ensures controlled stack allocation and simplifies stack overflow detection.
- The symbols `__tdat_start` and `__tdat_end` were predefined in the linker script, along with a custom `.tdat` section. This section is used to store Task Control Blocks (TCBs), task stacks, and corresponding guard zones. The `.tdat` section ensures that stacks and their guard zones are placed contiguously in memory, enabling reliable stack overflow monitoring.
- The `__heap_end` variable is computed as:
  
      __heap_end = (__brkval != 0) ? __brkval : (void*)&__heap_start;
  
- `__brkval` is a pointer internally managed by malloc() to indicate the current top of the heap. If no memory has been allocated yet, it remains zero.
- The `__stack_ptr` variable is initialized with the value of the `SP` register before the RTOS scheduler starts. On AVR microcontrollers, `SP` holds the current stack pointer. However, after the scheduler starts, `SP` is overwritten with the stack pointer of the currently executing task, which would lead to incorrect free memory calculations if used directly.


#### 8.2 Custom Segments

When compiling a program, the linker is responsible for placing variables and code into the correct memory segments — for example, initialized variables go into the `.data` section, uninitialized variables into `.bss`, and so on. This process is usually handled automatically by the default linker script.

However, relying solely on the default script does not guarantee that specific variables will be placed contiguously in memory. Their placement can vary depending on factors such as the order of .o files passed to the linker.

To ensure that all data related to each task — specifically the Task Control Block (TCB), task stack, and its corresponding guard zone — are placed contiguously in memory, I defined a custom .tdat memory section. This section is further divided into subsections, one for each stack and guard zone. This layout allows for reliable and predictable stack overflow detection, as a dedicated task can systematically inspect each guard zone to verify memory integrity.

I chose to manually assign each guard zone and task stack to its own subsection in the linker script to ensure strict ordering and prevent unexpected memory layout issues. This eliminates any ambiguity and guarantees that variables appear exactly where intended in SRAM.

This configuration is reflected in the following linker script fragment:

    .tdat (NOLOAD) :
    {
	. = ALIGN(1);
	PROVIDE (__tdat_start = . );
	
	KEEP(*(.tdat.guardZone0));	
	KEEP(*(.tdat.errorHandlerStack));	
	KEEP(*(.tdat.guardZone1));	
	KEEP(*(.tdat.serialSystemDebuggerStack));	
	KEEP(*(.tdat.guardZone2));	
	KEEP(*(.tdat.mainStack));	
	KEEP(*(.tdat.guardZone3));	
	KEEP(*(.tdat.readInputStack));	
	KEEP(*(.tdat.guardZone4));	
	KEEP(*(.tdat.serialInputStack));	
	KEEP(*(.tdat.guardZone5));	
	KEEP(*(.tdat.updateScreenStack));	
	KEEP(*(.tdat.guardZone6));	
	KEEP(*(.tdat.readtempStack));	
	KEEP(*(.tdat.guardZone7));	
	KEEP(*(.tdat.regulateTempStack));	
	KEEP(*(.tdat.guardZone8));	
	KEEP(*(.tdat.selectDrinkStack));	
	KEEP(*(.tdat.guardZone9));	
	KEEP(*(.tdat.orderDrinkStack));	
	KEEP(*(.tdat.guardZone10));	
	KEEP(*(.tdat.showSystemInfoStack));
	KEEP(*(.tdat.guardZone11));
	KEEP(*(.tdat.welcomeScreenStack));
	
	KEEP(*(.tdat))
	KEEP(*(.tdat*))
	PROVIDE (__tdat_end = . );
    }
    
Below is an example of how a guard zone and a task stack are declared in code:

    volatile StackType_t guardZone0[GUARD_ZONE_SIZE]              __attribute__((section(".tdat.guardZone0")));

    StackType_t errorHandlerStack[TASK_ERROR_HANDLER_STACK_SIZE]  __attribute__((section(".tdat.errorHandlerStack")));

*Note:*
- Each guard zone is declared as volatile to ensure the compiler does not optimize away accesses or overwrite them unexpectedly. Since these memory regions are checked explicitly in software for integrity (e.g., to detect overflow), they must always be preserved exactly as written in memory. Marking them as volatile prevents the compiler from assuming they remain unchanged or unused.

After compiling and inspecting the .map file, I confirmed that the .tdat section is correctly placed in SRAM. All subsections appear in the exact order defined in the linker script, starting from `__tdat_start`. Each stack and guard zone is properly aligned and located contiguously, which is essential for deterministic overflow detection logic.

    .tdat           0x008010e8      0xe97
                    0x008010e8                . = ALIGN (0x1)
                    0x008010e8                PROVIDE (__tdat_start, .)
    *(.tdat.guardZone0)
    .tdat.guardZone0
                    0x008010e8      0x20  C:\Users\kujon\AppData\Local\Temp\ccjR9wYB.ltrans0.ltrans.o
    *(.tdat.errorHandlerStack)
    .tdat.errorHandlerStack
                    0x00801108      0x100 C:\Users\kujon\AppData\Local\Temp\ccjR9wYB.ltrans0.ltrans.o
    *(.tdat.guardZone1)
    .tdat.guardZone1
                    0x00801208      0x20  C:\Users\kujon\AppData\Local\Temp\ccjR9wYB.ltrans0.ltrans.o

---

### 9. Free Memory Calculation  

Free memory calculation is straightforward on AVR microcontrollers.

The stack starts at `RAMEND` and grows downward. Its current position is captured in the `__stack_ptr` variable, which holds the value of the `SP` (Stack Pointer) register before the RTOS scheduler is started.

The heap begins at `__heap_start`, which is the first available address after global and static data sections (`.data`, `.bss`, and `.tdat`) are initialized. It grows upward, with its current boundary given by `__heap_end`.

Therefore, the amount of free memory available in the system is calculated as:

    Free memory = __stack_ptr - __heap_end

---

### 10. 🧩 PCB

#### 10.1 MCU Pinout (TQFP-64 ATmega2560)

| Pin | Usage |
|-----|-------|
| PE0 (RXD0/PCINT8)  | 🟢 USART0 RX / ICSP Serial Data in |
| PE1 (TXD0)         | 🟢 USART0 TX / ICSP Serial Data out |
| PE2 (XCK0/AIN0)    | ⚪ Unused |
| PE3 (OC3A/AIN1)    | 🟢 Radiator fan 1 MOSFET's gate |
| PE4 (OC3B/INT4)    | 🟢 Radiator fan 2 MOSFET's gate |
| PE5 (OC3C/INT5)    | 🟢 Cooler fan MOSFET's gate |
| PE6 (T3/INT6)      | 🟢 Circulation pump MOSFET's gate |
| PE7 (ICP3/INT7)    | ⚪ Unused |
| PB0 (SS/PCINT0)    | ⚪ Unused |
| PB1 (SCK/PCINT1)   | 🟢 ICSP Serial Clock |
| PB2 (MOSI/PCINT2)  | ⚪ Unused |
| PB3 (MISO/PCINT3)  | ⚪ Unused |
| PB4 (OC2A/PCINT4)  | ⚪ Unused |
| PB5 (OC1A/PCINT5)  | 🟢 Buzzer NPN's base |
| PB6 (OC1B/PCINT6)  | ⚪ Unused |
| PB7 (OC0A/OC1C/PCINT7) | ⚪ Unused |
| PC0 (A8)           | 🟢 74HC595 Serial data input |
| PC1 (A9)           | 🟢 74HC595 Storage register clock input |
| PC2 (A10)          | 🟢 74HC595 Shift register clock input |
| PC3 (A11)          | 🟢 74HC595 Output enable (active LOW) |
| PC4 (A12)          | ⚪ Unused |
| PC5 (A13)          | ⚪ Unused |
| PC6 (A14)          | ⚪ Unused |
| PC7 (A15)          | ⚪ Unused |
| PD0 (SCL/INT0)     | 🔵 I2C SCL |
| PD1 (SDA/INT1)     | 🔵 I2C SDA |	
| PD2 (RXD1/INT2)    | 🟢 MCP23008 INT |
| PD3 (TXD1/INT3)    | 🟢 MCP23008 RESET |
| PD4 (ICP1)         | 🟢 Peltier element 1 MOSFET's gate |
| PD5 (XCK1)         | 🟢 Peltier element 2 MOSFET's gate |
| PD6 (T1)           | 🟢 LED Ring DI |
| PD7 (T0)           | 🟢 LED Ring DO |


*Note:*  
- Timer usage in the system:  
  - **Timer2** is used by FreeRTOS for the system tick. This provides a precise periodic interrupt to drive task scheduling and timing functions.  
  - **Timer4** is dedicated to the custom I²C driver with ring buffer support, allowing non-blocking I²C communication. The timer triggers interrupts for handling I²C events, so CPU time is not blocked during transfers.  
  - **Timer1** drives the buzzer output on pin **PB5**, enabling PWM control for generating sound frequencies.  

#### 10.2 Bill of Materials (BOM)

| Reference / Designator | Component       | Footprint   | Quantity | Notes / Value   |
|------------------------|-----------------|-------------|----------|-----------------|
| U1                     | MCU             | TQFP-64     | 1        | ATmega2560      |
| U2                     | IC              | SOIC-16     | 1        | 74HC595         |
| U3                     | IC              | SOIC-16     | 1        | CH340G          |
| U4                     | IC              | SOIC-18     | 1        | MCP23008T-E/SO  |
| U5                     | MCU             | SOIC-8      | 1        | ATTINY85-20SU   |
| Q1 - Q2                | N-MOSFET        | TO-220-3    | 2        | IRFB7545PBF     |
| Q3 - Q8                | Dual N-MOSFET   | SOIC-8      | 6        | IRF7380TRPBF    |


#### 10.3 MOSFET Power Dissipation Calculations

This section contains calculations of the power dissipated by the MOSFETs to verify that the selected transistors can safely handle the intended load. Since these MOSFETs will not be driven by a PWM signal, switching losses are not considered; only conduction losses due to RDS(on) are included.
The power dissipated on a MOSFET can be calculated using the conduction losses formula:
P_D = I_D^2 × R_DS(on)

Where:
- P_D – power dissipated on the MOSFET [W]
- I_D – drain current through the MOSFET [A]
- R_DS(on) – MOSFET on-resistance at the given gate-source voltage [Ω]

##### 10.3.1 MOSFETs Q1, Q2

MOSFETs Q1 and Q2 will control the Peltier elements, each drawing 6 A at 12 V. Therefore, it is crucial that these transistors exhibit minimal power dissipation, which requires a low RDS(on). The calculations in this section are intended to confirm that the MOSFETs will operate safely under full load without excessive heating.





#### 10.4 PCB Layout



Preview of the custom-designed AVR board used in the project:

![PCB Layout - top view](Media/PCB_TOP_VIEW.PNG)
![PCB Layout - bottom view](Media/PCB_BOTTOM_VIEW.PNG)

---

### 11. 🔌 Electrical Schematic  
Full schematic of the system, including MCU, Peltier drivers, shift register control, keypad interface, and LCD wiring:

---

### 12. Additional Notes  

#### 12.1 Low-level Drivers

This project implements custom low-level drivers for core communication peripherals, providing full control over hardware and timing without relying on Arduino libraries or RTOS-specific wrappers.

##### UART Driver

- Full-duplex UART driver for the ATmega2560 using the USART0 peripheral.
- Supports blocking and non-blocking transmit and receive using ring buffers.
- Implements hardware-level ISRs for transmit buffer empty (`USART_UDRE_vect`) and receive complete (`USART_RX_vect`).
- Configurable TX and RX buffer sizes via macros.
- Supports strings stored in flash memory (PROGMEM) with dedicated functions.
- Completely independent of FreeRTOS or any other RTOS.

##### I2C (TWI) Master Driver

- Implements a master-mode I2C driver for ATmega2560 / ATmega2561.
- Supports both blocking and non-blocking operations.
- Uses a transmit buffer for queued data packets including addresses, read/write flags, and data.
- Managed by ISRs for TWI events (`TWI_vect`) and a timer interrupt (`TIMER4_COMPA_vect`) for precise timing.
- Timer4 is configured to generate interrupts approximately every 10 microseconds (prescaler 64, 16 MHz CPU clock).
- Configurable buffer sizes for transmission and reception.
- RTOS independent, allowing use in bare-metal or FreeRTOS environments.

These drivers ensure deterministic timing and minimal CPU blocking, crucial for reliable real-time embedded operation.

#### 12.2 System Initialization

The project includes a dedicated initialization module responsible for preparing the system hardware and RTOS environment before normal operation begins.
- Configures all I/O pins according to the custom hardware design.
- Allocates memory statically for FreeRTOS objects such as tasks, queues, semaphores, and mutexes.
- Initializes key hardware peripherals including UART, I²C devices (e.g., LCD, keypad expanders), and shift registers for pump control.
- Implements a system startup routine that runs early during boot (placed in the `.init8` linker section), ensuring all components are ready before the scheduler starts.
- The startup routine is marked with GCC `naked` and `used` attributes to control exact placement and prevent unwanted optimizations or removal.

This careful initialization sequence ensures reliable and deterministic system behavior from power-up.



---

### 13. 🚀 How to build

This project **was originally built and uploaded using the Arduino IDE**, which allowed for quick prototyping and development. However, due to the limitations of the Arduino environment — such as lack of build transparency and limited control over the toolchain — the project has been successfully **migrated to Visual Studio Code with a CMake-based build system**.

The new build process uses **CMake** along with **Ninja** as the build tool, offering improved portability, customization, and integration capabilities. The migration to a professional toolchain provides full control over compilation and flashing, replacing the previous Arduino workflow.

Although the project still compiles and uploads successfully through the Arduino IDE for legacy support, the primary development and deployment now rely on the VS Code + CMake + Ninja environment.

In hindsight, Arduino is a platform primarily designed for hobbyists. Had I been fully aware of its limitations earlier, I would have chosen a professional environment like Atmel Studio or a similar build system from the start — which would have saved a lot of time.


The build steps are outlined below and include:
- Installing VS Code
- Installing CMake
- Installing Ninja as the build tool
- Installing AVR-GCC toolchain
- Installing AVRDUDE for uploading the compiled firmware
- Building and uploading the project

#### 12.1 Installing VS Code

#### 12.2 Installing CMake

#### 12.3 Installing Ninja as the build tool

#### 12.4 Installing AVR-GCC toolchain

#### 12.5 Installing AVRDUDE for uploading the compiled firmware

#### 12.6 Building and uploading the project


---
