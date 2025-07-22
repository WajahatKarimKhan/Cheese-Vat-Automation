/**
 * @file UARTCom.h
 * @brief Interface for serial communication utilities.
 *
 * Provides initialization and overloaded print/println functions for easy serial output
 * of various data types.
 */

#ifndef UART_COM_H
#define UART_COM_H

//=============Serial Functions===============
void initSerial();
//=============Serial Print Functions===============
//function overloading with multiple parameters
void print(const char* message);
void print(char character);
void print(float value);
void print(int value);
void print(const char* message, float value);
//Printing a message or value or both and move to new line
void println();
void println(const char* message);
void println(char character);
void println(float value); // for printing decimal numbers 
void println(int value);
void println(const char* message, float value); // function for printing both the characters and numbers
////=============Serial Read Function===============
/**
 * @brief Reads a character from the serial.
 * This function should be called only if serial data is available.
 * @return The character read from the serial buffer.
 */
char readUART();
////=============Serial Available Function===============
/**
 * @brief Checks if serial data is available to read.
 * This function checks if any bytes are available in the serial 
 * before attempting to read them.
 * @return true if data is available; false otherwise.
 */
bool UARTisAvailable();
#endif 