#ifndef PRG_DATA_H
#define PRG_DATA_H

#include <Arduino.h>

struct Program{
  const char* prg_name;
  char targetTemp[3]; //  (2 digits) of temperature + null
  bool tempRegulation;
  char holdTimeMinutes[3]; //(2 digits) for minutes + null

  char stirSpeedHeating[5]; // 4 digits for rpm + null
  bool stirDirHeating;  // true = CW, false = CCW

  char stirSpeedHolding[5]; // 4 digits for rpm + null
  bool stirDirHolding;

  char targetCoolTemp[3]; //  (2 digits) of temperature + null
  //can add more parameters
};

void checkTouchInputPRG();
void updateDisplayFieldsPRG();
void handleKeypadDataPRG(const char* key);


extern Program pasteurizationProgram;
extern Program cheeseMakingProgram;
extern Program coolingProgram;

extern Program* activeProgram;



#endif