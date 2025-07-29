/**
 * @file iomap.h
 * @brief Pin mapping definitions for sensors and communication interfaces.
 *
 * This file defines constants for SPI and I2C pin assignments.
 * These are used to ensure consistent hardware connections across the project.
 */
#ifndef IOMAP_H
#define IOMAP_H

//#include <Adafruit_MAX6675.h>


// =================== SPI Communication Pins ===================

/**
 * @brief SPI Clock Pin and SPI Master-In-Slave-Out Pin (MISO) shared by all SPI devices.
 */
#define THERMO_SCK   8  // Common SCK pin for all the Temperature sensors 
#define THERMO_SO    9 // Common MISO pin for all the Temperature sensors

/**
 * @brief Chip Select pin for Temperature Sensors (MAX6675).
 */
#define TEMP_1_CS   11 //Chip select pin for Temperature sensor 1
#define TEMP_2_CS   12 //Chip select pin for Temperature sensor 2
#define TEMP_3_CS   13 //Chip select pin for Temperature sensor 3

// =================== I2C Pins ===================

/**
 * @note Arduino Due default I2C pins:
 * - SDA (Data line) → pin 20
 * - SCL (Clock line) → pin 21
 * These are used for communication with I2C devices such as RTCs.
 */

// =================== SD card Pins ===================
/**
 * @brief Chip Select pin for SD card module.
 */
#define SD_CARD_CS  10 //Chip select pin for SD card module
// ================== Ultrasonics Sensors ==================
  #define MILK_TRIG_PIN        2
  #define MILK_ECHO_PIN        3
  #define WATER_TRIG_PIN       4
  #define WATER_ECHO_PIN       5
// ================= Heating rods ====================
#define HEATING_RODS_PIN    A6
// ================= Water Pump ====================
#define WATER_PUMP_PIN      A7
// ================= Water IN / OUT valve ====================
#define INLET_VALVE_WATER_PIN      16
#define OUTLET_VALVE_WATER_PIN     17
// ================= Milk Pump ====================
#define MILK_PUMP_PIN             14
// ================= Milk Inlet valve ====================
#define INLET_VALVE_MILK_PIN      15

#endif //IOMAP_H