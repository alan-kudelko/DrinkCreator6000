//Doxy

#ifndef BOARD_IO_H_
	#define BOARD_IO_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
	extern "C"{
#endif // __cplusplus

void circulation_on(void);

void circulation_off(void);

void cooler_on(void);

void cooler_off(void);

void fans_on(void);

void fans_off(void);

void pumps_enable(void);

void pumps_disable(void);

bool read_keyboard_int(void);

bool is_temp_ready(void);

void shiftOut(uint8_t value);

#ifdef __cplusplus
	}
#endif // __cplusplus
	

#endif // BOARD_IO_H_