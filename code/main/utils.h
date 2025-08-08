#ifndef UTILS_H
#define UTILS_H

#include <ModbusMaster.h>

float readMilkLevelPercentage();
float readwaterLevelPercentage();

bool isMilkLevelHigh();
bool isWaterLevelHigh();

bool isMilkLevelLow();
bool isWaterLevelLow();

void startHeater();
void stopHeater();

void startStirrer(ModbusMaster& node, int rpm, bool reverse);
void stopStirrer(ModbusMaster& node);

void startPump();
void stopPump();

void startMilkPump();
void stopMilkPump();

void inletvalveMilkON();
void inletvalveMilkOFF();


void inletvalveWaterON();
void inletvalveWaterOFF();

void outletvalveWaterON();
void outletvalveWaterOFF();

void StopProcess();
void pauseProcess();
void resumeProcess();

extern bool StopButtonPressed;
extern bool PauseButtonPressed;
extern unsigned long totalHeaterOnDuration;
extern unsigned long heaterMinutes;
#endif