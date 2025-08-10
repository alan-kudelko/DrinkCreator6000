/**
 * @file taskErrorHandler.h
 * @brief Task responsible for stack overflow diagnostics and system error handling.
 *
 * This task monitors guard zones to detect stack overflows in the project.
 * Upon detecting an overflow, it records the event time and error cause in EEPROM,
 * displays error information on the LCD and serial port, and restarts the system
 * after a predefined timeout.
 */
#ifndef _TASK_ERROR_HANDLER_H_
    #define _TASK_ERROR_HANDLER_H_
/**
 * @brief Monitors stack guard zones, logs errors, and manages system restarts.
 *
 * @param pvParameters Task parameters (unused).
 */
void taskErrorHandler(void*pvParameters);

#endif // _TASK_ERROR_HANDLER_H_
