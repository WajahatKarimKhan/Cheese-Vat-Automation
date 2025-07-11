#ifndef UART_SERVICE_INTERFACE_H
#define UART_SERVICE_INTERFACE_H

/**
 * @brief Encapsulating function for all UART service interface functions
 */
void SerialServiceInterface();

/**
 * @brief Displays the UART-based low-level service menu.
 * Prints a list of available hardware-level features accessible via serial commands.
 */

void UARTExittoServiceMenu();
void UARTlevelSelect();

void displayServiceMenu();

/**
 * @brief Processes user commands received via the serial monitor.
 * Waits for input and updates `Run_cmd` to indicate which command should be executed.
 * Based on the input, it invokes relevant testing or display functions (e.g., temperature sensors, RTC).
 */
void processLowlevelServiceCommand();

/**
 * @brief Temperature sensor testing submenu.
 * Allows user to select from multiple temperature sensors and view their readings.
 * Also provides an option to return to the main menu.
 */
void testTempSensor();

/**
 * @brief Real-time clock (RTC) testing function.
 * Displays current date/time or allows interaction with the RTC module.
 */
void testRTC();


void testSD();

/**
 * @brief Exits from current submenu back to the low-level service interface.
 * Resets menu flags and command state to allow re-entry into the low-level menu.
 */
void UARTExittolowlevel();

/**
 * @brief Executes hardware-level actions based on current `Run_cmd`.
 * Maps command codes to specific sensor readings.
 */
void SIExecute();

#endif