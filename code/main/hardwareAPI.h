#ifndef HARDWARE_H
#define HARDWARE_H

#include <max6675.h>
#include "temp_sensors.h"

double readTEMP(int sensor);

void delay1s(); // Custom delay function

//=============Pin Control Functions===============
void setPin();
void readDigital();
void writeDigital();
void readAnalogue();
void writeAnalogue();

#endif 