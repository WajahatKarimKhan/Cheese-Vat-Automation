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
  int targetCoolingTemp;
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
  int targetCoolingTemp;
}; 
struct CoolingParameters{  int targetCoolingTemp; };
struct MilkRefillParameters{  int milkLitres; }; 

//extern PasteurizationParameters params;
extern Manual manualData;
extern Program programData;

extern PasteurizationPhase pastPhase;
extern CheeseMakingPhase cmPhase;
extern CoolingStage coolStage;
extern MilkRefillingStage milkStage;
extern WaterRefillingStage WRStage;
extern WaterDrainageStage  WDStage;

extern bool isPasteurizing;
extern bool isCheeseMaking;
extern bool isMilkRefilling;
extern bool isCooling;
extern bool isWaterRefilling;
extern bool isWaterDrainage;

void exe_Pasteurization(Execute_Mode mode, Manual* m = nullptr, Program* p = nullptr );
void exe_CheeseMaking(Execute_Mode mode, Manual* m = nullptr, Program* p = nullptr );
void exe_Cooling(Execute_Mode mode, Manual* m = nullptr, Program* p = nullptr );
void exe_Cooling(int targetCoolingTemp);
void exe_MilkRefill(Execute_Mode mode, Manual* m, Program* p);
void exe_WaterDrainage();
void exe_WaterRefill();

void fsm_Pasteurization();
void fsm_CheeseMaking();

extern Execute_Mode mode;
extern Manual m;
extern Program p;


#endif