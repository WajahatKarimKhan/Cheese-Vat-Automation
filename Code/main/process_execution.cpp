#include "process_execution.h"
#include "utils.h"
#include <Arduino.h>
#include "constant.h"
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

//PasteurizationParameters params;
Manual manualData;
Program programData;

//MODBUS node instance
ModbusMaster stirrer;

void exe_Pasteurization(Execute_Mode mode, Manual* m, Program* p) {
  PasteurizationParameters params;

if (mode == EXE_MANUAL && m != nullptr) {
        //params.name = m->man_name;
        params.targetTemp = atoi(m->man_targetTemp);
        params.tempReg = m->man_tempRegulation;
        params.holdTime = atoi(m->man_holdTimeMinutes);
        params.stirHeatRPM = atoi(m->man_stirSpeedHeating);
        params.stirHeatDir = m->man_stirDirHeating;
        params.stirHoldRPM = atoi(m->man_stirSpeedHolding);
        params.stirHoldDir = m->man_stirDirHolding;
    }
else if ((mode == EXE_AUTO_PROCESS || mode == EXE_AUTO_SCHEDULE) && p != nullptr) {
        //params.name = p->prg_name;
        params.targetTemp = atoi(p->targetTemp);
        params.tempReg = p->tempRegulation;
        params.holdTime = atoi(p->holdTimeMinutes);
        params.stirHeatRPM = atoi(p->stirSpeedHeating);
        params.stirHeatDir = p->stirDirHeating;
        params.stirHoldRPM = atoi(p->stirSpeedHolding);
        params.stirHoldDir = p->stirDirHolding;
        
    }
    
// verifying output
    println("Running Pasteurization"); 
    println("Target Heating Temp.: ", params.targetTemp); 
    print("Temp Regulation: "); Serial.println(params.tempReg ? "ON" : "OFF");
    println("Hold Time: ", params.holdTime); 
    println("Stir Heat RPM: ", params.stirHeatRPM); 
    print("Stir Dir Heat: "); Serial.println(params.stirHeatDir ? "CW" : "CCW");
    println("Stir Hold RPM: ", params.stirHoldRPM);
    print("Stir Dir Hold: "); Serial.println(params.stirHoldDir ? "CW" : "CCW");

// Actual logic to regulate temp, stir, hold, etc. 
  
    // === 1. Level check Phase ===
    if (!isMilkLevelHigh()) {
        println("Milk level too low");
        return;
    }
    if (!isWaterLevelHigh()) {
        println("Water level too low");
        return;
    }
    println("Level check passed");

    // === 2. Heating Phase ===
    println("Starting Heating...");
    //startStirrer(stirrer, params.stirHeatRPM, params.stirHeatDir); //commented as no vfd to respond yet
    while (readTEMP(TEMP_SENSOR_1) < params.targetTemp) {
        StopProcess();
        startHeater();
        //delay(500);  // simulate sensor update interval
    }
    //stopStirrer(stirrer);
    stopHeater(); // Switch to regulation
    println("Reached target temperature");
    //delay(3000);

    // === 3. Holding Phase ===
    println("Holding temperature phase started...");
    //startStirrer(stirrer, params.stirHoldRPM, params.stirHoldDir); 

    unsigned long holdStart = millis();
    const float tolerance = 0.25;     //   +-2 degree Celsius

    while ((millis() - holdStart) < (params.holdTime * 60000UL)) {
        float temp = readTEMP(TEMP_SENSOR_1);
        if (temp < params.targetTemp - tolerance) {
            startHeater();
            println("heater on");
        } else if (temp > params.targetTemp + tolerance) {
            stopHeater();
            println("heater off");
        }
        StopProcess();
        delay(500);  // update rate
    }

    stopHeater();
    //stopStirrer(stirrer);
    println("Holding temperature phase completed...");

    // === 4. Cooling Phase ===
    exe_Cooling(mode,  m,  p);
    // === Completion ===
    println("Pasteurization Complete");
    // setProcessDoneFlag(true);
}



void exe_CheeseMaking(Execute_Mode mode, Manual* m, Program* p) {
  CheeseMakingParameters params;

if (mode == EXE_MANUAL && m != nullptr) {
        //params.name = m->man_name;
        params.targetTemp = atoi(m->man_targetTemp);
        params.tempReg = m->man_tempRegulation;
        params.holdTime = atoi(m->man_holdTimeMinutes);
        params.stirHeatRPM = atoi(m->man_stirSpeedHeating);
        params.stirHeatDir = m->man_stirDirHeating;
        params.stirHoldRPM = atoi(m->man_stirSpeedHolding);
        params.stirHoldDir = m->man_stirDirHolding;
    }
else if ((mode == EXE_AUTO_PROCESS || mode == EXE_AUTO_SCHEDULE) && p != nullptr) {
        //params.name = p->prg_name;
        params.targetTemp = atoi(p->targetTemp);
        params.tempReg = p->tempRegulation;
        params.holdTime = atoi(p->holdTimeMinutes);
        params.stirHeatRPM = atoi(p->stirSpeedHeating);
        params.stirHeatDir = p->stirDirHeating;
        params.stirHoldRPM = atoi(p->stirSpeedHolding);
        params.stirHoldDir = p->stirDirHolding;
    }
    
// verifying output
    println("Running Cheese Making...."); 
    println("Target Heating Temp.: ", params.targetTemp); 
    print("Temp Regulation: "); Serial.println(params.tempReg ? "ON" : "OFF");
    println("Hold Time: ", params.holdTime); 
    println("Stir Heat RPM: ", params.stirHeatRPM); 
    print("Stir Dir Heat: "); Serial.println(params.stirHeatDir ? "CW" : "CCW");
    println("Stir Hold RPM: ", params.stirHoldRPM);
    print("Stir Dir Hold: "); Serial.println(params.stirHoldDir ? "CW" : "CCW");

// Actual logic to regulate temp, stir, hold, etc. goes here
    // === 1. Level check Phase ===
    if (!isMilkLevelHigh()) {
        println("Milk level too low");
        return;
    }
    if (!isWaterLevelHigh()) {
        println("Water level too low");
        return;
    }
    println("Level check passed");

    // === 2. Heating Phase ===
    println("Starting Heating...");
    //startStirrer(stirrer, params.stirHeatRPM, params.stirHeatDir); //commented as no vfd to respond yet
    while (readTEMP(TEMP_SENSOR_1) < params.targetTemp) {
        StopProcess();
        startHeater();
        //delay(500);  // simulate sensor update interval
    }
    //stopStirrer(stirrer);
    stopHeater(); // Switch to regulation
    println("Reached target temperature");
    //delay(3000);

    // === 3. Holding Phase ===
    println("Holding temperature phase started...");
    //startStirrer(stirrer, params.stirHoldRPM, params.stirHoldDir); 

    unsigned long holdStart = millis();
    const float tolerance = 1.0;     //   +-2 degree Celsius

    while ((millis() - holdStart) < (params.holdTime * 60000UL)) {
        float temp = readTEMP(TEMP_SENSOR_1);
        if (temp < params.targetTemp - tolerance) {
            startHeater();
            println("heater on");
        } else if (temp > params.targetTemp + tolerance) {
            stopHeater();
            println("heater off");
        }
        StopProcess();
        delay(500);  // update rate
    }

    stopHeater();
    //stopStirrer(stirrer);
    println("Holding temperature phase completed...");

    // === 4. Cooling Phase ===
    exe_Cooling(mode,  m,  p);
    // === Completion ===
    println("Cheese Making Completed");
    // setProcessDoneFlag(true);
    
}
void exe_Cooling(Execute_Mode mode, Manual* m, Program* p) {
  CoolingParameters params;

  if (mode == EXE_MANUAL && m != nullptr) {
        params.targetCoolingTemp = atoi(m->man_targetCoolTemp);
        println("Cooling using MANUAL data");
    }
    else if ((mode == EXE_AUTO_PROCESS || mode == EXE_AUTO_SCHEDULE) && p != nullptr) {
        params.targetCoolingTemp = atoi(p->targetCoolTemp);
        println("Cooling using PROGRAMMED data");
    }  
    // verifying output
    println("Running COOLING ...");
    println("Target Cooling Temp: ", params.targetCoolingTemp);
     
  // Actual logic to  stir, hold, etc. goes here
    //startStirrer(stirrer, params.stirHeatRPM, params.stirHeatDir); //commented as no vfd to respond yet

    inletvalveWaterON();
    outletvalveWaterON();

    while (readTEMP(TEMP_SENSOR_1) > params.targetCoolingTemp) {
        StopProcess(); //WILL be triggered if stop is pressed
        startPump();    
        }
    stopPump();
    //stopStirrer(stirrer);
    inletvalveWaterOFF();
    outletvalveWaterOFF();
    println("Cooling Completed...");
}
void exe_MilkRefill(Execute_Mode mode, Manual* m, Program* p) {
  MilkRefillParameters params;

  if (mode == EXE_MANUAL && m != nullptr) {
        //params.name = m->man_name;
        //params.targetTemp = atoi(m->man_targetTemp);
    }
    else if ((mode == EXE_AUTO_PROCESS || mode == EXE_AUTO_SCHEDULE) && p != nullptr) {
        //params.name = p->prg_name;
        //params.targetTemp = atoi(p->targetTemp);
    }  
    // verifying output
    println("Running Milk Refill ...");
    //print("Target Temp: "); println(params.targetTemp); 
  // Actual logic  goes here
}
void exe_WaterRefill() {
 
    println("Running Water refill ...");
  // Actual logic HERE
    startPump();
    inletvalveWaterON();
    outletvalveWaterON();

    // Wait until level is low enough ===
    while (!isWaterLevelHigh()) {  // Keep draining until low level
        StopProcess();

        println("Filling... Water level still low");
    }
    inletvalveWaterOFF();
    outletvalveWaterOFF();
    println("Water Filling Completed...");
}
void exe_WaterDrainage() {
 
    println("Running Water Drain ...");
  // Actual logic HERE
    stopPump();
    inletvalveWaterON();
    outletvalveWaterON();

    // Wait until level is low enough ===
    while (!isWaterLevelLow()) {  // Keep draining until low level
        StopProcess();

        println("Draining... Water level still high");
    }
    inletvalveWaterOFF();
    outletvalveWaterOFF();
    println("Water Drainage Completed...");
}