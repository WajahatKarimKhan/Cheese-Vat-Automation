#ifndef MANUAL_DATA_H
#define MANUAL_DATA_H

#include <Arduino.h>

struct Manual{
  const char* man_name;
  char man_targetTemp[3]; //  (2 digits) of temperature + null
  bool man_tempRegulation;
  char man_holdTimeMinutes[3]; //(2 digits) for minutes + null

  char man_stirSpeedHeating[5]; // 4 digits for rpm + null
  bool man_stirDirHeating;  // true = CW, false = CCW

  char man_stirSpeedHolding[5]; // 4 digits for rpm + null
  bool man_stirDirHolding;
  
  char man_milkinltrs[4]; // 4 digits for no. of ltrs + null

  char man_targetCoolTemp[3]; //  (2 digits) of temperature + null 
  //can add more parameters
};

void checkTouchInputMan();
void updateDisplayFieldsMan();
void handleKeypadDataMan(const char* key);

extern Manual pasteurizationManual;
extern Manual cheeseMakingManual;
extern Manual coolingManual;
extern Manual milkRefillManual;

extern Manual* activeManual;



#endif