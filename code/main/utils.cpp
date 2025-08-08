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
bool PauseButtonPressed= false ;

//to calculate total rods on time 
unsigned long heaterStartTime = 0;
unsigned long totalHeaterOnDuration = 0;
bool heaterIsOn = false;
unsigned long heaterMinutes = totalHeaterOnDuration / 60000;



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
bool isWaterLevelLow()  {  return (readWaterLevelPercentage() <= WATER_EMPTY_THRESHOLD);  }



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

       unsigned long past_holdStart = 0;
       unsigned long   cm_holdStart = 0;

       //stopStirrer(stirrer);
       stopPump();
       stopMilkPump();
       stopHeater();
       stopPump();
       stopMilkPump();
       //stopStirrer(stirrer);
       inletvalveMilkOFF();
       inletvalveWaterOFF();
       outletvalveWaterOFF();

       StopButtonPressed = false;
    }
}

void pauseProcess(){
  if (!PauseButtonPressed){
    PauseButtonPressed= true ;
    println("Process Paused.");
  }
  stopHeater();
  stopPump();
  stopMilkPump();
  //stopStirrer(stirrer);
  inletvalveMilkOFF();
  inletvalveWaterOFF();
  outletvalveWaterOFF();
  
}
void resumeProcess(){
  //if(pauseORresume == BTN_PAUSE_PRESSED){
  //  pauseORresume == BTN_RESUME_PRESSED;
  if (PauseButtonPressed){
    PauseButtonPressed= false ;
    println("Process Resumed.");
  }
}

void startStirrer(ModbusMaster& node, int rpm, bool stirrerDirection) {
  //currentStirrerDirection = stirrerDirection;
  VFD_setRPM(node, rpm, maxRPM);     // Map to Hz and send to VFD // can change 1800 to the max rpm the motor offers
  VFD_start(node, stirrerDirection); // Start stirrer in given direction
}

void stopStirrer(ModbusMaster& node) {
  VFD_stop(node);  // Sends STOP command over Modbus to VFD
}
/* //------When device is connected on the NC -- HIGH means closed circuit at NC ---

void startHeater() {  
  if (!heaterIsOn) {
    heaterStartTime = millis();
    heaterIsOn = true;
  }
  //Serial.println(heaterMinutes);
  digitalWrite(HEATING_RODS_PIN, HIGH); 
}

void stopHeater()  {  
  if (heaterIsOn) {
    totalHeaterOnDuration += millis() - heaterStartTime;
    heaterIsOn = false;
  }
  //Serial.println(heaterMinutes);
  digitalWrite(HEATING_RODS_PIN, LOW);  
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
*/

///*
//------When device is connected on the NO -- LOW means closed circuit at NO ---
void startHeater() {  
  if (!heaterIsOn) {
    heaterStartTime = millis();
    heaterIsOn = true;
  }
  //Serial.println(heaterMinutes);
  digitalWrite(HEATING_RODS_PIN, LOW); 
}

void stopHeater()  {  
  if (heaterIsOn) {
    totalHeaterOnDuration += millis() - heaterStartTime;
    heaterIsOn = false;
  }
  //Serial.println(heaterMinutes);
  digitalWrite(HEATING_RODS_PIN, HIGH);  
}

void startPump(){ digitalWrite(WATER_PUMP_PIN, LOW); }
void stopPump() { digitalWrite(WATER_PUMP_PIN, HIGH);  }

void startMilkPump() { digitalWrite(MILK_PUMP_PIN, LOW); }
void stopMilkPump()  { digitalWrite(MILK_PUMP_PIN, HIGH);  }

void inletvalveMilkON()  { digitalWrite(INLET_VALVE_MILK_PIN, LOW); }
void inletvalveMilkOFF() { digitalWrite(INLET_VALVE_MILK_PIN, HIGH);  }

void inletvalveWaterON() { digitalWrite(INLET_VALVE_WATER_PIN, LOW); }
void inletvalveWaterOFF(){ digitalWrite(INLET_VALVE_WATER_PIN, HIGH);  }

void outletvalveWaterON() { digitalWrite(OUTLET_VALVE_WATER_PIN, LOW); }
void outletvalveWaterOFF(){ digitalWrite(OUTLET_VALVE_WATER_PIN, HIGH);  }
//  */

