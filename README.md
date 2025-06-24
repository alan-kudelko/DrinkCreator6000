# DrinkCreator6000 – RTOS System on Custom AVR Board

Welcome to `DrinkCreator6000`, an embedded real-time operating system project designed to run on a custom-made PCB using AVR microcontrollers, FreeRTOS, and static memory allocation. The system is used in a smart drink machine with modular tasks handling temperature control, system monitoring, user interaction, and error recovery.

> 🔧 Status: In development  
> 🧪 Goal: Create functional drink machine, Explore FreeRTOS and AVR integration using a structured, real-time multi-tasking approach

---

## 🧱 Project Structure

All logic is implemented in statically allocated FreeRTOS tasks running on a custom PCB with AVR hardware. The system is modular and designed to support precise debugging and future expansion.

### 🧵 Task Overview

| Task Name | Description |
|-----------|-------------|
| `TaskMain` | Coordinates the system, managing high-level logic and task activation. |
| `TaskReadInput` | Handles button presses, rotary encoders, or other input peripherals. |
| `TaskControlTemperature` | Regulates fluid temperature via heating/cooling systems. |
| `TaskDisplaySystemInfo` | Displays system data such as free RAM, uptime, and diagnostics. |
| `TaskDisplayTemperature` | Shows current measured temperature on LCD. |
| `TaskDisplaySelectedDrink` | Displays the selected drink configuration and status. |
| `TaskErrorHandler` | Handles critical faults, overflows, and writes error logs to EEPROM. |
| `TaskCrashDisplay` | Displays error info on LCD after a system reset due to crash. |
| `TaskDebugMemory` | Monitors stack/RAM usage of all tasks. |
| `TaskDebugTasks` | Displays task priorities, names, and runtime state. |

---

## 🧠 Design Goals

- 🎯 Demonstrate real-time scheduling and modular task separation  
- 💾 Use **100% static memory allocation** (no dynamic malloc)  
- 🧰 Track system stability via runtime task/memory debug tools  
- 🔁 Ensure recovery after failure using EEPROM fault logging  
- 📟 Provide full system visibility through LCD diagnostics  
- 🧪 Serve as a practical testbed for AVR and RTOS learning

---

## 🗺️ Roadmap

- [X] Implement custom PCB with AVR MCU
- [ ] Add core system and debug tasks
- [ ] Integrate LCD output for live system diagnostics
- [ ] Enable crash detection and EEPROM fault logging
- [ ] Finalize temperature regulation loop
- [ ] Add drink selection logic
- [ ] Create EEPROM-based config loader
- [ ] Support persistent fault history
- [ ] Future: Serial interface for external control
- [ ] Future: Upgrade to cooperative multitasking model

---
