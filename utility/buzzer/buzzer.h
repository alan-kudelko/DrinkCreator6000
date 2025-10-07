//doxy

#ifndef BUZZER_H_
	#define BUZZER_H_

#include <stdint.h>

#define BUZZER_SUCCESS 0
#define BUZZER_ERROR   1

#ifdef __cplusplus
	extern "C"{
#endif // __cplusplus

extern void activateBuzzer(uint8_t tone);

extern void deactivateBuzzer(void);

#ifdef __cplusplus
	}
#endif // __cplusplus

#endif // BUZZER_H_