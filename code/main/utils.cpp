#include "utils.h"
#include "process_execution.h"
#include "gui_fsm.h"
#include "UARTCom.h"
#include <UTFT.h>
#include <UTouch.h>
#include "global_var.h"
#include "command.h"
#include "sd_card.h"
#include "rtc.h"
#include "iomap.h"
#include "vfd.h"  
#include <Arduino.h>
#include "hardwareAPI.h"

bool StopButtonPressed = false ;


float readMilkLevelPercentage() {
    // Trigger ultrasonic pulse
    digitalWrite(MILK_TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(MILK_TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(MILK_TRIG_PIN, LOW);

    // Read echo time
    long duration = pulseIn(MILK_ECHO_PIN, HIGH, 30000); // timeout in μs
    float distance = duration * 0.034 / 2.0;  // cm

    // Compute level %
    float level = MILK_TANK_HEIGHT_CM - distance;
    if (level < 0) level = 0;
    if (level > MILK_TANK_HEIGHT_CM) level = MILK_TANK_HEIGHT_CM;
    return (100 / 100) * 100.0; // just to check the code 
    //return (level / MILK_TANK_HEIGHT_CM) * 100.0;
}
bool isMilkLevelHigh() {  return (readMilkLevelPercentage() >= MILK_MIN_THRESHOLD);   }
bool isMilkLevelLow()  {  return (readMilkLevelPercentage() <= MILK_EMPTY_THRESHOLD); }

float readWaterLevelPercentage() {
    // Trigger ultrasonic pulse
    digitalWrite(WATER_TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(WATER_TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(WATER_TRIG_PIN, LOW);

    // Read echo time
    long duration = pulseIn(WATER_ECHO_PIN, HIGH, 30000);
    float distance = duration * 0.034 / 2.0;

    float level = WATER_TANK_HEIGHT_CM - distance;
    if (level < 0) level = 0; ///just to avoid the negative values
    if (level > WATER_TANK_HEIGHT_CM) level = WATER_TANK_HEIGHT_CM;
    return (100 / 100) * 100.0; // just to check the code 
    //return (level / WATER_TANK_HEIGHT_CM) * 100.0;
}
bool isWaterLevelHigh() {  return (readWaterLevelPercentage() >= WATER_MIN_THRESHOLD);    }
bool isWaterLevelLow()  {  return (readWaterLevelPercentage() <= WATER_EMPTY_THRESHOLD ); }

void startHeater() {  digitalWrite(HEATING_RODS_PIN, HIGH); }
void stopHeater()  {  digitalWrite(HEATING_RODS_PIN, LOW);  }

void StopProcess() {
    if (StopButtonPressed) {
        println("User stopped the process.");
       // abortProcess();  // Call your abort logic
       isPasteurizing = false;
       pastPhase = PAST_PHASE_IDLE;
       
       isCheeseMaking = false;
       cmPhase = CM_PHASE_IDLE;

       isMilkRefilling = false;
       milkStage = MILK_STAGE_IDLE;

       isCooling = false;
       coolStage = COOL_STAGE_IDLE;

       isWaterRefilling = false;
       WRStage = WR_STAGE_IDLE;
      
       isWaterDrainage = false;
       WDStage = WD_STAGE_IDLE;

       StopButtonPressed = false;
    }
}

void startStirrer(ModbusMaster& node, int rpm, bool reverse) {
  VFD_setRPM(node, rpm, 1800);     // Map to Hz and send to VFD
  VFD_start(node, reverse);        // Start stirrer in given direction
}
void stopStirrer(ModbusMaster& node) {
  VFD_stop(node);  // Sends STOP command over Modbus to VFD
}

void startPump(){ digitalWrite(WATER_PUMP_PIN, HIGH); }
void stopPump() { digitalWrite(WATER_PUMP_PIN, LOW);  }

void startMilkPump() { digitalWrite(MILK_PUMP_PIN, HIGH); }
void stopMilkPump()  { digitalWrite(MILK_PUMP_PIN, LOW);  }

void inletvalveMilkON()  { digitalWrite(INLET_VALVE_MILK_PIN, HIGH); }
void inletvalveMilkOFF() { digitalWrite(INLET_VALVE_MILK_PIN, LOW);  }

void inletvalveWaterON() { digitalWrite(INLET_VALVE_WATER_PIN, HIGH); }
void inletvalveWaterOFF(){ digitalWrite(INLET_VALVE_WATER_PIN, LOW);  }

void outletvalveWaterON() { digitalWrite(OUTLET_VALVE_WATER_PIN, HIGH); }
void outletvalveWaterOFF(){ digitalWrite(OUTLET_VALVE_WATER_PIN, LOW);  }


