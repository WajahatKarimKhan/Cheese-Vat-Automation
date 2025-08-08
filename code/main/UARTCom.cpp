/**
 * @file UARTCom.cpp
 * @brief Implements serial communication utilities including overloaded print and println functions.
 *
 * Provides functions to simplify and standardize serial output across the application,
 * supporting different data types.
 */

#include "UARTCom.h"
#include <Arduino.h>
#include "constant.h"


void initSerial(){ Serial.begin(_BAUD_RATE);}   //Initializes the serial communication with the predefined baud rate.
//=============Print Functions===============

void print(const char* message){ Serial.print(message); }
void print(char character){ Serial.print(character); }
void print(float value){ Serial.print(value); }
void print(int value){ Serial.print(value); }
void print(const char* message, float value){
  Serial.print(message);
  Serial.print(value);
}

void println(){ Serial.println();}
void println(const char* message){ Serial.println(message); }
void println(char character){ Serial.print(character); }
void println(float value){ Serial.println(value); }
void println(int value){ Serial.print(value); }
void println(const char* message, float value){
  Serial.print(message);
  Serial.println(value);
}

char readUART() { char input = Serial.read();}
//char readUART() { char subinput = Serial.read(); }
bool UARTisAvailable(){
  return Serial.available() > 0 ;
}


