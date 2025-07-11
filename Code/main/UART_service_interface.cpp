/**
 * @file UART_service_interface.cpp
 * @brief Provides a low-level UART menu interface for accessing hardware features.
 *
 * @details
 * This module handles serial-based user interaction for service-level debugging or control.
 * Supports viewing temperature sensor readings, ..
*/
#include <Arduino.h>
#include "hardwareAPI.h"
#include "UART_service_interface.h"
#include "command.h"
#include "global_var.h"
#include "UARTCom.h"
#include "log.h"
#include "rtc.h"
/// Global command variable updated based on UART input.
//uint8_t Run_cmd ;
//uint8_t Run_cmd = 0x00;

/// Flag to ensure the service menu is shown only once per session.
bool serviceMenuShown = false;
bool serviceLowlevelMenuShown = false;

void SerialServiceInterface() {
  displayServiceMenu();
  UARTlevelSelect();
}
void displayServiceMenu() {
  if (serviceMenuShown == false) {
    println("=== Main UART Service Menu ===");
    println("Press L for Low Level Menu:");
    println("Press M for Mid Level Menu:");
    println("Press H for High Level Menu:");
    println("Press U for User Level Menu:");
    println("-------------------------------------------");
    serviceMenuShown = true;
  }
}


// User can type one option in serial monitor input bar 
void UARTlevelSelect() {
  if (UARTisAvailable()) {
    char input = readUART();
    switch (input) {
      case 'L':
       processLowlevelServiceCommand();
        break;
 /*   case 'M':
       
        break;
      case 'H':
       
        break;
      case 'U':
       
        break;
 */   case 'R':
        UARTExittoServiceMenu();
        break;
    }
  }
}


// User can type one option in serial monitor input bar 
void processLowlevelServiceCommand() {
    while (1) {
    Run_cmd = CMD_LOW_LEVEL;
  // Displaying the Low Level menu once
   if (serviceLowlevelMenuShown == false){
    println("=== Low Level Service Menu ===");
    println("1. Press A for Temperature Sensors");
    println("2. Press B for Real-time clock(RTC)");
    println("3. Press C for SD card");
    // Options can be added here
    println("4. Press R to Exit to Main UART Service Menu");
    println("-------------------------------------------");
    serviceLowlevelMenuShown = true;
  }
  if (UARTisAvailable()) {
    char input = readUART();
    switch (input) {
      case 'A':
       testTempSensor();
        break;
      case 'B':
       testRTC();
        break;
      case 'C':
       testSD();
        break;
      case 'X':
        UARTExittolowlevel();
        break;
    }
  if (input=='R'){
      UARTExittoServiceMenu();  // return to low_level menu
      return; }
  }
}
}
/**
 * @brief Displays a menu to test temperature sensors via UART.
 *
 * Allows the user to select one of three temperature sensors
 * or exit back to the main service menu. Continuously reads
 * user input and updates `Run_cmd` accordingly.
 */
void testTempSensor() {
  bool tempMenuShown = false;
  while (1) {
  // Displaying the temperature menu once
   if (!tempMenuShown){
    println("Select Temperature Sensor:");
    println("1. Temperature Sensor 1");
    println("2. Temperature Sensor 2");
    println("3. Temperature Sensor 3");
    println("4. Press X to Exit");
    println("-------------------------------------------");
    tempMenuShown = true;
  }
   //Waits for the user input and updates the `Run_cmd`
    if(UARTisAvailable()) {    
    char input = readUART();

    if (input=='1'){ Run_cmd = CMD_TEMP_SENSOR_1; }
    else if (input=='2'){ Run_cmd = CMD_TEMP_SENSOR_2; }
    else if (input=='3'){ Run_cmd = CMD_TEMP_SENSOR_3; }    
    else if (input=='X'){
     UARTExittolowlevel();  // return to low_level menu
     return; 
      }
 
    }
  //  else { println("Invalid input. Try again."); }
  SIExecute();
  }
}

/**
 * Continuously displays RTC data until the user exits by pressing 'X'.
 * Updates the `Run_cmd` to indicate the RTC test command.
 */
void testRTC(){
    while(1){
      Run_cmd = CMD_TEST_RTC;   // updates the `Run_cmd`
      if(UARTisAvailable()) {   
      char input = readUART();
      if (input=='X'){
        UARTExittolowlevel();
      return;      }  
    }
      SIExecute();
  }   
}

/**
 * Executes the command set in `Run_cmd`.
 * Executes actions like reading and printing temperature sensor values
 */

void testSD(){
    while(1){
      Run_cmd = CMD_TEST_SD;   // updates the `Run_cmd`
      if(UARTisAvailable()) {   
      char input = readUART();
      if (input=='X'){
        UARTExittolowlevel();
      return;      }  
    }
      SIExecute();
  } 
}


void SIExecute(){
  switch (Run_cmd) {
    case CMD_TEMP_SENSOR_1:
      print("Temp sensor 1: ", readTEMP(TEMP_SENSOR_1));
      println(" °C");
      delay1s();
      break;
    case CMD_TEMP_SENSOR_2:
      print("Temp sensor 2: ", readTEMP(TEMP_SENSOR_2));
      println(" °C");
      delay1s();
      break;
    case CMD_TEMP_SENSOR_3:
      print("Temp sensor 3: ", readTEMP(TEMP_SENSOR_3));
      println(" °C");
      delay1s();
      break;
    case CMD_TEST_RTC:
      printRTC();
      delay1s();
      break;
    case CMD_TEST_SD:
      printSD();
      delay1s();
      break;
  }
}

 // Reset to UART main menu
void UARTExittoServiceMenu(){
      Run_cmd = CMD_SI_MAIN;   
      println("'R' is pressed. Returned to Main Menu.");      
      println();
      serviceMenuShown = false;  // Resets the flag so main menu is shown 
}

 // Reset to low level menu
void UARTExittolowlevel(){
      Run_cmd = CMD_LOW_LEVEL;   
      println("'X' is pressed. Returned to Low Level Menu.");      
      println();
      serviceLowlevelMenuShown = false;  // Resets the flag so low level menu is shown 
}
