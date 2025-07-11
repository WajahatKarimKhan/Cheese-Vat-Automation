#ifndef PROCESS_EXECUTION_H
#define PROCESS_EXECUTION_H

#include "constant.h"
#include "prg_data.h"
#include "manual_data.h"


struct PasteurizationParameters{
  int targetTemp;
  bool tempReg;
  int holdTime;
  int stirHeatRPM;
  bool stirHeatDir;
  int stirHoldRPM;
  bool stirHoldDir;
  int milkLitres;
}; 

//PasteurizationParameters params;

struct CheeseMakingParameters{
  int targetTemp;
  bool tempReg;
  int holdTime;
  int stirHeatRPM;
  bool stirHeatDir;
  int stirHoldRPM;
  bool stirHoldDir;
  int milkLitres;
}; 
struct CoolingParameters{
  int targetCoolingTemp; 
};
struct MilkRefillParameters{
  int milkLitres;
}; 

//extern PasteurizationParameters params;
extern Manual manualData;
extern Program programData;


void exe_Pasteurization(Execute_Mode mode, Manual* m = nullptr, Program* p = nullptr );
void exe_CheeseMaking(Execute_Mode mode, Manual* m = nullptr, Program* p = nullptr );
void exe_Cooling(Execute_Mode mode, Manual* m = nullptr, Program* p = nullptr );
void exe_MilkRefill(Execute_Mode mode, Manual* m, Program* p);
void exe_WaterDrainage();
void exe_WaterRefill();


#endif