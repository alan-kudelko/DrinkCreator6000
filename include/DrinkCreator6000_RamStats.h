/**
 * @file DrinkCreator6000_RamStats.h
 * @brief RAM usage monitoring and runtime memory boundaries
 *
 * This header declares global memory region symbols and RAM usage tracking 
 * variables used across the project.
 *
 * Memory regions (defined by linker):
 * - `.data`, `.bss`, `.tdat`: static data segments
 * - heap: managed by `malloc`
 * - stack: top-down region ending at `__stack_ptr`
 *
 * Tracked values (updated at runtime):
 * - total RAM size and usage
 * - heap/stack size and free space
 *
 * Initialization:
 * - `initRamSize()` runs during `.init8` (startup)
 * - `updateMemoryUsage()` updates usage values
 */
#ifndef _DRINK_CREATOR6000_RAM_STATS_H_
    #define _DRINK_CREATOR6000_RAM_STATS_H_

#include <stdint.h>

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

/** @name Linker-provided memory region symbols
 *  @{
 */
extern uint16_t __data_start;   /**< Start of .data segment */
extern uint16_t __data_end;     /**< End of .data segment */

extern uint16_t __bss_start;    /**< Start of .bss segment */
extern uint16_t __bss_end;      /**< End of .bss segment */

extern uint16_t __tdat_start;   /**< Start of .tdat (custom section) */
extern uint16_t __tdat_end;     /**< End of .tdat section */
extern uint16_t __tdat_size;    /**< Size of .tdat (calculated at runtime) */

extern uint16_t __heap_start;   /**< Start of heap (typically after .bss) */
extern void*    __brkval;       /**< Current break pointer (malloc-managed) */

extern void*    __heap_end;     /**< End of heap (calculated) */
extern uint8_t* __stack_ptr;    /**< Current stack pointer (from SP) */
/** @} */

/** @name Runtime-calculated memory usage statistics
 *  @{
 */
extern uint16_t __heap_size;    /**< Size of heap in bytes */
extern uint16_t __stack_size;   /**< Size of stack in bytes */

extern uint16_t ram_total_free; /**< Total free RAM between heap and stack */
extern uint16_t ram_in_use;     /**< Total used RAM: data + heap + stack */
extern uint16_t ram_size;       /**< Total usable SRAM size */
/** @} */

/** @name Initialization functions
 *  @{
 */

/**
 * @brief Initializes the RAM size value.
 *
 */
extern void initRamSize(void);
/**
 * @brief Updates all RAM usage tracking variables.
 *
 * Call this function at runtime to refresh memory usage stats.
 * Calculates heap, stack, tdata usage and updates global RAM stats.
 */
extern void updateMemoryUsage(void);

/** @} */

#ifdef __cplusplus
    }
#endif // __cplusplus

#endif // _DRINK_CREATOR6000_RAM_STATS_H_