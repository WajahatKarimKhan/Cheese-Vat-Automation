#include "process_execution.h"
#include "utils.h"
#include <Arduino.h>
#include "constant.h"
#include "command.h"
#include "vfd.h"
#include "schedule_data.h"
#include "PRG_data.h"
#include "manual_data.h"
#include "view_logs_data.h"
#include "hardwareAPI.h"
#include "UARTCom.h"


// Declare global execution mode instances
Execute_Mode exe_None = EXE_NONE;
Execute_Mode exe_Auto_Schedule = EXE_AUTO_SCHEDULE;
Execute_Mode exe_Auto_Process = EXE_AUTO_PROCESS;
Execute_Mode exe_Manual = EXE_MANUAL;

Execute_Mode mode;
Manual m;
Program p;

Manual manualData;
Program programData;

//MODBUS node instance
ModbusMaster stirrer;

PasteurizationPhase pastPhase = PAST_PHASE_IDLE;
CheeseMakingPhase cmPhase = CM_PHASE_IDLE;
CoolingStage coolStage = COOL_STAGE_IDLE;
MilkRefillingStage milkStage = MILK_STAGE_IDLE;
WaterRefillingStage WRStage = WR_STAGE_IDLE;
WaterDrainageStage WDStage = WD_STAGE_IDLE;

bool isPasteurizing  = false;  // Controls whether FSM should run
bool isCheeseMaking  = false;
bool isMilkRefilling = false; 
bool isCooling       = false;
bool isWaterRefilling= false;
bool isWaterDrainage = false; 

PasteurizationParameters past_params;
CheeseMakingParameters cm_params;

unsigned long past_holdStart = 0;
unsigned long cm_holdStart = 0;


void exe_Pasteurization(Execute_Mode mode, Manual* m, Program* p) {   
    if (mode == EXE_MANUAL && m != nullptr) {
        //params.name = m->man_name;
        past_params.targetTemp = atoi(m->man_targetTemp);
        past_params.tempReg = m->man_tempRegulation;
        past_params.holdTime = atoi(m->man_holdTimeMinutes);
        past_params.stirHeatRPM = atoi(m->man_stirSpeedHeating);
        past_params.stirHeatDir = m->man_stirDirHeating;
        past_params.stirHoldRPM = atoi(m->man_stirSpeedHolding);
        past_params.stirHoldDir = m->man_stirDirHolding;
        past_params.targetCoolingTemp = atoi(m->man_targetCoolTemp);
    }
    else if ((mode == EXE_AUTO_PROCESS || mode == EXE_AUTO_SCHEDULE) && p != nullptr) {
        //params.name = p->prg_name;
        past_params.targetTemp = atoi(p->targetTemp);
        past_params.tempReg = p->tempRegulation;
        past_params.holdTime = atoi(p->holdTimeMinutes);
        past_params.stirHeatRPM = atoi(p->stirSpeedHeating);
        past_params.stirHeatDir = p->stirDirHeating;
        past_params.stirHoldRPM = atoi(p->stirSpeedHolding);
        past_params.stirHoldDir = p->stirDirHolding;
        past_params.targetCoolingTemp = atoi(p->targetCoolTemp);
        
    }
    
    // verifying output
    println("Running Pasteurization"); 
    println("Target Heating Temp.: ", past_params.targetTemp); 
    print("Temp Regulation: "); Serial.println(past_params.tempReg ? "ON" : "OFF");
    println("Hold Time: ", past_params.holdTime); 
    println("Stir Heat RPM: ", past_params.stirHeatRPM); 
    print("Stir Dir Heat: "); Serial.println(past_params.stirHeatDir ? "CW" : "CCW");
    println("Stir Hold RPM: ", past_params.stirHoldRPM);
    print("Stir Dir Hold: "); Serial.println(past_params.stirHoldDir ? "CW" : "CCW");

}
void fsm_Pasteurization(){
    if(PauseButtonPressed){ return ;} //system pause if pause is pressed
    float temp = readTEMP(TEMP_SENSOR_1);
    switch (pastPhase){
        /*
        case PAST_PHASE_IDLE:
            if (isPasteurizing) {
                pastPhase = PAST_PHASE_LEVEL_CHECK;
                println("Starting Pasteurization ... Level Check");
            }
            break;
        */
        case PAST_PHASE_LEVEL_CHECK:
        if(isPasteurizing){
            if (!isMilkLevelHigh() || !isWaterLevelHigh()) {
                println("Level check failed.");
                isPasteurizing = false;
                //pastPhase = PAST_PHASE_IDLE;
                break;
            }
            println("Level check passed.");
            pastPhase = PAST_PHASE_HEATING;
            println("Heating Phase started........");
            }
            break;

        case PAST_PHASE_HEATING: 
            //println("Heating Phase started........");
            //startStirrer(stirrer, past_params.stirHeatRPM, past_params.stirHeatDir);
            Serial.print("Temp: "); Serial.println(temp);
            Serial.print("Target: "); Serial.println(past_params.targetTemp);

            if (readTEMP(TEMP_SENSOR_1) >= past_params.targetTemp) {
                println("Target temperature reached.");
                StopProcess(); //if stop is pressed
                stopHeater();
                //stopStirrer(stirrer);
                pastPhase = PAST_PHASE_HOLDING;
                
            } else {
                startHeater();
                println("Rods on ---");
                //delay(1000);
            }
            break;
        
        case PAST_PHASE_HOLDING: {
            static bool holdStarted = false;
            const float tolerance = 0.25;

            if (!holdStarted) {
                past_holdStart = millis();
                holdStarted = true;
                println("Holding Phase started........");
            }
            // Check if holding time is complete FIRST
            if ((millis() - past_holdStart) >= (past_params.holdTime * 60000UL)) {
                println("Holding done. Start cooling...");
                stopHeater();
                pastPhase = PAST_PHASE_COOLING;
                holdStarted = false;
                unsigned long past_holdStart = 0;
                break;
            }
            //float temp = readTEMP(TEMP_SENSOR_1);
            if (temp < past_params.targetTemp - tolerance) {
                startHeater();
                println("heating rods ON");
                //delay(1000);
            } else if (temp > past_params.targetTemp + tolerance) {
                stopHeater();
                println("heating rods OFF");
                //delay(1000);
            }    
            break;
            println("Holding done. Start cooling...");    
        }
            case PAST_PHASE_COOLING:
            //println("Cooling Phase started........");
            stopHeater();
            isCooling = true ;
            if (isCooling){ exe_Cooling(past_params.targetCoolingTemp); } 
            break;
        }  

}
void exe_CheeseMaking(Execute_Mode mode, Manual* m, Program* p) {
    if (mode == EXE_MANUAL && m != nullptr) {
        //params.name = m->man_name;
        cm_params.targetTemp = atoi(m->man_targetTemp);
        cm_params.tempReg = m->man_tempRegulation;
        cm_params.holdTime = atoi(m->man_holdTimeMinutes);
        cm_params.stirHeatRPM = atoi(m->man_stirSpeedHeating);
        cm_params.stirHeatDir = m->man_stirDirHeating;
        cm_params.stirHoldRPM = atoi(m->man_stirSpeedHolding);
        cm_params.stirHoldDir = m->man_stirDirHolding;
        cm_params.targetCoolingTemp = atoi(m->man_targetCoolTemp);
    }
    else if ((mode == EXE_AUTO_PROCESS || mode == EXE_AUTO_SCHEDULE) && p != nullptr) {
        //params.name = p->prg_name;
        cm_params.targetTemp = atoi(p->targetTemp);
        cm_params.tempReg = p->tempRegulation;
        cm_params.holdTime = atoi(p->holdTimeMinutes);
        cm_params.stirHeatRPM = atoi(p->stirSpeedHeating);
        cm_params.stirHeatDir = p->stirDirHeating;
        cm_params.stirHoldRPM = atoi(p->stirSpeedHolding);
        cm_params.stirHoldDir = p->stirDirHolding;
        cm_params.targetCoolingTemp = atoi(p->targetCoolTemp);
    }
    
    // verifying output
    println("Running Cheese Making...."); 
    println("Target Heating Temp.: ", cm_params.targetTemp); 
    print("Temp Regulation: "); Serial.println(cm_params.tempReg ? "ON" : "OFF");
    println("Hold Time: ", cm_params.holdTime); 
    println("Stir Heat RPM: ", cm_params.stirHeatRPM); 
    print("Stir Dir Heat: "); Serial.println(cm_params.stirHeatDir ? "CW" : "CCW");
    println("Stir Hold RPM: ", cm_params.stirHoldRPM);
    print("Stir Dir Hold: "); Serial.println(cm_params.stirHoldDir ? "CW" : "CCW");

}
void fsm_CheeseMaking(){
    if(PauseButtonPressed){ return ;} //system pause if pause is pressed
    // Actual logic to regulate temp, stir, hold, etc. goes here
    switch (cmPhase){
        /*
        case CM_PHASE_IDLE:
            if (isPasteurizing) {
                pastPhase = CM_PHASE_LEVEL_CHECK;
                println("Starting CheeseMaking ... Level Check");
            }
            break;
        */
        case CM_PHASE_LEVEL_CHECK:
        if(isCheeseMaking){
            if (!isMilkLevelHigh() || !isWaterLevelHigh()) {
                println("Level check failed.");
                isCheeseMaking = false;
                //cmPhase = CM_PHASE_IDLE;
                break;
            }
            println("Level check passed.");
            cmPhase = CM_PHASE_HEATING;
            println("Heating Phase started........");
            }
            break;

        case CM_PHASE_HEATING: 
            
            //startStirrer(stirrer, params.stirHeatRPM, params.stirHeatDir);
            if (readTEMP(TEMP_SENSOR_1) >= cm_params.targetTemp) {
                println("Target temperature reached.");
                StopProcess(); //if stop is pressed
                stopHeater();
                //stopStirrer(stirrer);
                cmPhase = CM_PHASE_HOLDING;
            } else {
                startHeater();
                println("Rods on ---");
                //delay(1000);
            }
            break;
        
        case CM_PHASE_HOLDING: {
            static bool holdStarted = false;
            const float tolerance = 0.25;

            if (!holdStarted) {
                cm_holdStart = millis();
                holdStarted = true;
                println("Holding Phase started........");
            }
            // Check if holding time is complete FIRST
            if ((millis() - cm_holdStart) >= (cm_params.holdTime * 60000UL)) {
                println("Holding done. Start cooling...");
                stopHeater();
                cmPhase = CM_PHASE_COOLING;
                holdStarted = false;
                unsigned long cm_holdStart = 0;
                break;
            }
            float temp = readTEMP(TEMP_SENSOR_1);
            if (temp < cm_params.targetTemp - tolerance) {
                startHeater();
                println("heating rods ON");
                //delay(1000);
            } else if (temp > cm_params.targetTemp + tolerance) {
                stopHeater();
                println("heating rods OFF");
                //delay(1000);
            }    
            break;
            println("Holding done. Start cooling...");    
        }
            case CM_PHASE_COOLING:
            stopHeater();
            //println("Cooling Phase started........");
            isCooling = true ;
            //coolStage = COOL_STAGE_IDLE;
            if (isCooling){ exe_Cooling(cm_params.targetCoolingTemp); } 
            //println("Running COOLING ...");
            break;
        }  
}
void exe_Cooling(int targetCoolingTemp) {
    if(PauseButtonPressed){ return ;} //system pause if pause is pressed
    //println("Running COOLING ...");
    // Actual logic to  stir, hold, etc. goes here
    switch(coolStage){

    case COOL_STAGE_IDLE:
      coolStage = COOL_STAGE_START;
    break;

    case COOL_STAGE_START:
    
      //startStirrer(stirrer, params.stirHeatRPM, params.stirHeatDir);
      StopProcess(); // allows user to interrupt
        if (readTEMP(TEMP_SENSOR_1) > targetCoolingTemp) {
          inletvalveWaterON();
          outletvalveWaterON();
          startPump();
          println("Cooling to target temp: ", targetCoolingTemp);
        } else {
          stopPump();
          inletvalveWaterOFF();
          outletvalveWaterOFF();
          //stopStirrer(stirrer);
          coolStage = COOL_STAGE_DONE;
          delay1s();
        }
      
    break;

    case COOL_STAGE_DONE:
      println("Cooling done.");
      pastPhase = PAST_PHASE_IDLE;
      cmPhase = CM_PHASE_IDLE;
      coolStage = COOL_STAGE_IDLE;
      
      isPasteurizing = false;
      isCheeseMaking = false;
      isCooling = false;
    break;
    
    } 
}
void exe_MilkRefill(Execute_Mode mode, Manual* m, Program* p) {
    if(PauseButtonPressed){ return ;} //system pause if pause is pressed

    MilkRefillParameters params;

    if (mode == EXE_MANUAL && m != nullptr) {
        //milk data - now configured to be filled to a certain level instead of user defined Volume in liters
    }
    else if ((mode == EXE_AUTO_PROCESS || mode == EXE_AUTO_SCHEDULE) && p != nullptr) {
        //milk data - now configured to be filled to a certain level instead of user defined Volume in liters
    }  
    
    // Actual logic  goes here
    switch(milkStage){

        case MILK_STAGE_START:
        
        StopProcess(); // allows user to interrupt
            if (!isMilkLevelHigh()) {
            inletvalveMilkON();
                startMilkPump();
                println("Filling... Milk level still low");
                //delay(1000);
            } else {
            inletvalveMilkOFF();
            stopMilkPump();
            milkStage = MILK_STAGE_DONE;    
            //println("Milk Refill Completed.");
            //isMilkRefilling = false;
            //delay1s();
            }
        break;

        case MILK_STAGE_DONE:
        println("Milk Refilling done.");
        milkStage = MILK_STAGE_IDLE;
        isMilkRefilling = false;
        break;
        
    }
}
void exe_WaterRefill() {
    if(PauseButtonPressed){ return ;} //system pause if pause is pressed
    switch(WRStage){

        case WR_STAGE_START:
        
        StopProcess(); // allows user to interrupt
        if (!isWaterLevelHigh()) {
            startPump();
            inletvalveWaterON();
            outletvalveWaterON();
            println("Filling... Water level still low");
            //delay1s();
        } else {
            stopPump();
            inletvalveWaterOFF();
            outletvalveWaterOFF();
                
            WRStage = WR_STAGE_DONE;    
            //println("Water Refill Completed.");
            //isWaterRefilling = false;
            //delay1s();
        }
        
        break;

        case WR_STAGE_DONE:
        println("Water Refilling done.");
        WRStage = WR_STAGE_IDLE;
        isWaterRefilling = false;
        break;   
    }
}
void exe_WaterDrainage() {
    if(PauseButtonPressed){ return ;} //system pause if pause is pressed
    switch(WDStage){

        case WD_STAGE_START:
        StopProcess(); // allows user to interrupt
        if (!isWaterLevelLow()) {
            stopPump(); //if running
            inletvalveWaterON();
            outletvalveWaterON();
            println("Draining water... Water level still not low enough");
            //delay1s();
        } else {
            inletvalveWaterOFF();
            outletvalveWaterOFF(); 
            WDStage = WD_STAGE_DONE;    
            //println("Water Drainage Completed.");
            //isWaterDrainage = false;
            //delay1s();
            }
        
        break;

        case WD_STAGE_DONE:
        println("Water Drainage done.");
        WDStage = WD_STAGE_IDLE;
        isWaterDrainage = false;
        break;   
    }



}