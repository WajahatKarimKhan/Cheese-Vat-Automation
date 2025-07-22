/**
 * @file sys_init.h
 * @brief System initialization header for temperature sensors and RTC.
 *
 * Declares external MAX6675 temperature sensor objects and
 * the RTC initialization function.
 */

#ifndef SYS_INIT_H
#define SYS_INIT_H

#include <max6675.h>
#include "iomap.h"
#include "uRTCLib.h"

// =================== Temperature Sensor Objects ===================
/**
 * @brief External declaration for Temperature Sensors(MAX6675).
 */
//void initTempSensors();
extern MAX6675 TEMP_1;
extern MAX6675 TEMP_2;
extern MAX6675 TEMP_3;

/**
 * @brief Initializes the RTC with default values defined in constant.h.
 */
void initRTC();

void initGUI();

void initTempSensors();

void initValvesandRelays();

#endif  // SYS_INIT_H