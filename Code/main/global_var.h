/**
 * @file global_variables.h
 * @brief Declares global variables used across the system.
 * @details
 * This header provides external access to global state variables.
 * 
 */

#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

#include "constant.h"

extern char Temp1_floattochar[10];
extern char Temp2_floattochar[10];
extern char Temp3_floattochar[10];

// Global variable to store the currently selected command.
extern uint8_t Run_cmd ;
extern uint8_t previousRunCmd;

extern ScreenMode* currentScreenMode;

extern char currentInput[12];
extern uint8_t inputPos;
extern InputFieldSchedule activeFieldSchedule;
// enum RepeatType { REPEAT_ONCE, REPEAT_DAILY, REPEAT_WEEKLY };

extern char currentInputPRG[12];
extern uint8_t inputPosPRG;
extern InputFieldPRG activeFieldPRG;

extern char currentInputMan[12];
extern uint8_t inputPosMan;
extern InputFieldMan activeFieldMan;

extern char currentInputLogs[12];
extern uint8_t inputPosLogs;
extern InputFieldLogs activeFieldLogs;

#endif  // GLOBAL_VARIABLES_H