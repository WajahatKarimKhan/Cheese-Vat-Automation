#ifndef SD_CARD_H
#define SD_CARD_H


#include <Arduino.h>

bool checkSDcard();     // check if there is SD card
bool isSDInitialized();  // Check if SD is ready

void prepareLogFile();

extern bool sd_initialized;

void logToSD(float temp1, float temp2, float temp3, const char* status);
//float filledSpace_SD(); // gives us the percentage of the storage filled
const char* getCurrentProcessStatus();


#endif