/**
*@file sys_init.cpp
*@brief Purpose of this file is to initialize all the functions
*
*
*@author
*@date
*@version 1.0
*
*/

#include <max6675.h>
#include <Arduino.h>
#include "iomap.h"
#include "sys_init.h"
#include "uRTCLib.h"

/**
*@brief Class for communicating with thermocouple sensor
*@note THERMO_SCK and THERMO_SO are predefined and common pins for SPI communication
*where TEMP_1_CS, TEMP_2_CS and TEMP_3_CS are custom selected digital pins
*/
MAX6675 TEMP_1(THERMO_SCK, TEMP_1_CS, THERMO_SO);
MAX6675 TEMP_2(THERMO_SCK, TEMP_2_CS, THERMO_SO);
MAX6675 TEMP_3(THERMO_SCK, TEMP_3_CS, THERMO_SO);

void initValvesandRelays(){
  pinMode(HEATING_RODS_PIN, OUTPUT);
  pinMode(WATER_PUMP_PIN, OUTPUT);
  pinMode(INLET_VALVE_WATER_PIN, OUTPUT);
  pinMode(OUTLET_VALVE_WATER_PIN, OUTPUT);
}
uint8_t previousRunCmd = 0xFF;