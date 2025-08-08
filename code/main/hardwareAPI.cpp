/**
 * @file hardwareAPI.cpp
 * @brief Core hardware abstraction functions
 *
 * 
 * This file forms part of the hardware abstraction layer to simplify higher-level logic.
 */

#include "hardwareAPI.h"
#include "UARTCom.h"
#include "Arduino.h"
#include <max6675.h>
#include "temp_sensors.h"
#include "sys_init.h"
#include "constant.h"
#include "utils.h"

void delay1s(){ delay(_DELAY_1S); } // Delays the program for 1 second.

void initTempSensors() {  }          //Initializes temperature sensors.
/**
 * @brief Reads temperature in Celsius from a specified MAX6675 sensor.
 *
 * @param sensor Index of the temperature sensor to read.
 * Use values from the `Temp_Sensors` enum (e.g., `TEMP_SENSOR_1`).
 * @return Temperature in Celsius, or `255.0` if the sensor index is invalid.
 */
double readTEMP(int sensor){
    if (sensor == TEMP_SENSOR_1){return TEMP_1.readCelsius();}
    else if (sensor == TEMP_SENSOR_2){return TEMP_2.readCelsius();}
    else if (sensor == TEMP_SENSOR_3){return TEMP_3.readCelsius();}
    else { return 255; } // Invalid sensor index
}
