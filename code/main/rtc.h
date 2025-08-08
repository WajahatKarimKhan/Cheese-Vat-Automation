#ifndef RTC_H
#define RTC_H
#include <uRTCLib.h>
#include "constant.h"

extern uRTCLib rtc;

const char* currentDOW();

bool checkRTC();
bool isRTCConnected();

#endif