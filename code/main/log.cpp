/**
 * @file log.cpp
 * @brief Implements functions to print data over serial.
 *
 * Uses temperature sensor readings and real-time clock (RTC) information
 * to display current status over UART.
 */
#include <Arduino.h>
#include "uRTCLib.h"
#include "rtc.h"
#include "sd_card.h"
#include "log.h"
#include "temp_sensors.h"
#include "sys_init.h"
#include "constant.h"
#include "hardwareAPI.h"
#include "UARTCom.h"

/**
 * @brief Prints the temperature readings from all configured sensors.
 *
 * Currently, only TEMP_SENSOR_1 is active. Others are commented out.
 * Each value is displayed in degrees Celsius.
 */

void printAllTemps(){
  print("Temp sensor 1: ",readTEMP(TEMP_SENSOR_1));
  println(" °C");         
/*
  print("Temp sensor 2: ",readTEMP(TEMP_SENSOR_2));
  println(" °C");

  print("Temp sensor 3: ",readTEMP(TEMP_SENSOR_3));
  println(" °C");
*/
}

/**
 * @brief Prints the current date and time from the RTC module.
 *
 * Outputs year/month/day and hour:minute:second along with the current day of the week.
 * Optionally, ambient temperature from the RTC chip can be printed.
 */
void printRTC(){
  rtc.refresh();

  print("Date : ");
  print(rtc.year());
  print('/');
  print(rtc.month());
  print('/');
  println(rtc.day());
  print(" ");

  print(" (");
  print(currentDOW());
  print(")  ");

  print("Time : ");
  print(rtc.hour());
  print(':');
  print(rtc.minute());
  print(':');
  println(rtc.second());
 /* print("  ");
  print("Ambient Temperature: ");
  print(rtc.temp() / 100);
  println("°C");
  
  */
  println();
}

void printSD(){
  if (!checkSDcard()) {
        println("Error: SD card not detected.");
        println("SD card initialized failed.");
        //while (1); // halt system or show error on screen
    } else{
        println("SD card initialized successfully.");
        //while (1); // halt system or show error on screen
    }

   // println("SD card is ready.");
}