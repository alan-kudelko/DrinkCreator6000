# CMake project for DrinkCreator6000
#
#
cmake_minimum_required (VERSION 3.24)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Setting up C, C++ standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

message(STATUS "C/C++ standards: ${CMAKE_C_STANDARD}/${CMAKE_CXX_STANDARD}")
# Setting up output folders
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)
set(CMAKE_CROSSCOMPILING TRUE)

set(AVR_ROOT "${CMAKE_SOURCE_DIR}/tools")
message(STATUS "AVR toolchain set (local toolchain): ${AVR_ROOT}")
# Setting up compilers
set(CMAKE_C_COMPILER "${AVR_ROOT}/bin/avr-gcc.exe")
set(CMAKE_CXX_COMPILER "${AVR_ROOT}/bin/avr-g++.exe")
set(CMAKE_ASM_COMPILER "${AVR_ROOT}/bin/avr-gcc.exe")
set(CMAKE_RC_COMPILER " ")

project (CMake_AVR C CXX ASM)

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)
set(CMAKE_CROSSCOMPILING TRUE)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
# Setting up compiler flags
set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/linker/Custom_Linker.ld")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-T,${LINKER_SCRIPT}")
set(CMAKE_SHARED_LINKER_FLAGS "")
set(CMAKE_MODULE_LINKER_FLAGS "")

set(CMAKE_C_FLAGS "-mmcu=atmega2560 -Os -DF_CPU=8000000UL -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-exceptions -fno-rtti")

message(STATUS "Compiler flags C:   ${CMAKE_C_FLAGS}")
message(STATUS "Compiler flags CXX: ${CMAKE_CXX_FLAGS}")
# Include directories
message(STATUS "Adding project sources")
file(GLOB PROJECT_SOURCES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.c"
    "${CMAKE_SOURCE_DIR}/utility/debug/*.c"
    "${CMAKE_SOURCE_DIR}/utility/debug/*.cpp"
    "${CMAKE_SOURCE_DIR}/utility/buzzer/*.c"
)
message(STATUS "Adding FreeRTOS sources")
file(GLOB RTOS_SOURCES
    "${CMAKE_SOURCE_DIR}/FreeRTOS/FreeRTOS-Kernel/*.c"
    "${CMAKE_SOURCE_DIR}/FreeRTOS/FreeRTOS-Kernel/portable/ThirdParty/GCC/ATmega/*.c"
)
message(STATUS "Adding AVR Peripherals sources")
file(GLOB AVR_CORE_SOURCES
    "${CMAKE_SOURCE_DIR}/avr_peripherals/uart/*.c"
    "${CMAKE_SOURCE_DIR}/avr_peripherals/i2c/*.c"
    "${CMAKE_SOURCE_DIR}/avr_peripherals/board_io/*.c"
    "${CMAKE_SOURCE_DIR}/drivers/*.cpp"
)
message(STATUS "Adding project headers")
file(GLOB PROJECT_HEADERS
    "${CMAKE_SOURCE_DIR}/include/*.h"
    "${CMAKE_SOURCE_DIR}/include/*.hpp"
    "${CMAKE_SOURCE_DIR}/utility/debug/*.h"
    "${CMAKE_SOURCE_DIR}/utility/debug/*.hpp"
    "${CMAKE_SOURCE_DIR}/utility/buzzer/*.h"
)
message(STATUS "Adding FreeRTOS headers")
file(GLOB RTOS_HEADERS
    "${CMAKE_SOURCE_DIR}/FreeRTOS/FreeRTOS-Kernel/include*.h"
    "${CMAKE_SOURCE_DIR}/FreeRTOS/FreeRTOS-Kernel/include*.hpp"
    "${CMAKE_SOURCE_DIR}/FreeRTOS/FreeRTOS-Kernel/portable/ThirdParty/GCC/ATmega/*.h"
)
message(STATUS "Adding AVR Peripherals headers")
file(GLOB AVR_CORE_HEADERS
    "${CMAKE_SOURCE_DIR}/avr_peripherals/uart/*.h"
    "${CMAKE_SOURCE_DIR}/avr_peripherals/i2c/*.h"
    "${CMAKE_SOURCE_DIR}/avr_peripherals/board_io/*.h"
    "${CMAKE_SOURCE_DIR}/drivers/*.h"
)
include_directories(
    "${AVR_ROOT}/avr/include"
	"${AVR_ROOT}/avr/include/avr"
	"${AVR_ROOT}/avr/include/compat"
	"${AVR_ROOT}/avr/include/sys"
	"${AVR_ROOT}/avr/include/util"
	"${AVR_ROOT}/tools/lib/gcc/avr/7.3.0/include"
    "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/FreeRTOS/FreeRTOS-Kernel/include"
    "${CMAKE_SOURCE_DIR}/FreeRTOS/FreeRTOS-Kernel/portable/ThirdParty/GCC/ATmega"
    "${CMAKE_SOURCE_DIR}/avr_peripherals/uart"
    "${CMAKE_SOURCE_DIR}/avr_peripherals/i2c"
    "${CMAKE_SOURCE_DIR}/drivers"
    "${CMAKE_SOURCE_DIR}/utility/debug"
    "${CMAKE_SOURCE_DIR}/utility/buzzer"
    "${CMAKE_SOURCE_DIR}/avr_peripherals/board_io"
)
# Disable MSVC
set(CMAKE_FIND_ROOT_PATH "${AVR_ROOT}")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

message(STATUS "C   Compiler path: ${CMAKE_C_COMPILER}")
message(STATUS "CXX Compiler path: ${CMAKE_CXX_COMPILER}")
message(STATUS "ASM Compiler path: ${CMAKE_ASM_COMPILER}")

# Disable Windows default linker flags
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "")
set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "")
set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "")

add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
    ${RTOS_SOURCES}
    ${RTOS_HEADERS}
    ${AVR_CORE_SOURCES}
    ${AVR_CORE_HEADERS}
)
# Generate map file
set_target_properties(${PROJECT_NAME} PROPERTIES
    LINK_FLAGS "-Wl,-Map=${PROJECT_NAME}.map"
)
message(STATUS "Final sources for ${PROJECT_NAME}:")
foreach(src ${PROJECT_SOURCES} ${PROJECT_HEADERS} ${RTOS_SOURCES} ${RTOS_HEADERS} ${AVR_CORE_SOURCES} ${AVR_CORE_HEADERS})
    message(STATUS "${src}")
endforeach()
# Generate hex file
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${AVR_ROOT}/bin/avr-objcopy -O ihex -R .eeprom $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.hex
  COMMENT "Generating HEX and BIN files"
)