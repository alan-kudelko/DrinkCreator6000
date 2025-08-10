/**
 * @file taskRegulateTemp.h
 * @brief Temperature regulation task for DrinkCreator6000.
 *
 * This task uses global temperature variables to control Peltier elements
 * by switching their pins HIGH or LOW based on hysteresis thresholds.
 */
#ifndef _TASK_REGULATE_TEMP_H_
    #define _TASK_REGULATE_TEMP_H_

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

void taskRegulateTemp(void*pvParameters);

#ifdef __cplusplus
    }
#endif // __cplusplus

#endif // _TASK_REGULATE_TEMP_H_
