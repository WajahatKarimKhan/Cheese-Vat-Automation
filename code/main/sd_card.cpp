#include "sd_card.h"
#include "utils.h"
#include <SD.h>
//#include <SdFat.h>
#include <SPI.h>
#include "process_execution.h"
#include "hardwareAPI.h"
#include "UARTCom.h"
#include "iomap.h"
#include "gui.h"
#include "rtc.h"
#include <uRTCLib.h>


extern uRTCLib rtc;

bool sd_initialized = false;

bool checkSDcard() {
    if (SD.begin(SD_CARD_CS)) {
        sd_initialized = true;
        //println("SD card initialized successfully.");
    } else {
        sd_initialized = false;
        //println("SD card initialization failed!");
    }
    prepareLogFile();
    
    return sd_initialized;
}

bool isSDInitialized() { return sd_initialized; }

// ==== print header for Log file ====
void prepareLogFile() {
  if (!SD.exists("log.csv")) {
    File logFile = SD.open("log.csv", FILE_WRITE);
    if (logFile) {
      logFile.println("Date(d/m/y),Time,Temp1,Temp2,Temp3,Process Status,RodsOn Time");
      logFile.close();
    }
  }
}
// ==== logging to sd card ====
void logToSD(float temp1,float temp2,float temp3, const char* status) {
  
  static unsigned long lastLogTime = 0;
  const unsigned long logInterval = 1000; // Log every 1000 ms (1 sec)

  unsigned long currentMillis = millis();
  if (currentMillis - lastLogTime < logInterval) return;  // Too soon to log again
  lastLogTime = currentMillis;  // Update timer
  
  if (!isSDInitialized()) return;  // Don't attempt write if SD not ready

  rtc.refresh();  // Update RTC values
    
    temp1 = readTEMP(TEMP_SENSOR_1);
    temp2 = readTEMP(TEMP_SENSOR_2);
    temp3 = readTEMP(TEMP_SENSOR_3);


  char buffer[128];
  snprintf(buffer, sizeof(buffer), "%02d/%02d/%02d,%02d:%02d:%02d,%.2f,%.2f,%.2f,%s,%03d",
           rtc.day(), rtc.month(), rtc.year(), 
           rtc.hour(), rtc.minute(), rtc.second(),
           temp1, temp2, temp3, status, heaterMinutes);

  File logFile = SD.open("log.csv", FILE_WRITE);
  if (logFile) {
    logFile.println(buffer);
    logFile.close();
  }

}

// ==== Used space percentage ====

// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;
/*float filledSpace_SD(){
    uint32_t totalClusters = volume.clusterCount() ; 
    uint32_t freeClusters = volume.freeClusterCount() ;

    if (totalClusters == 0) return -1.0;

    float usedClustersPercentage = ((totalClusters - freeClusters)*100) / totalClusters;

    return usedClustersPercentage;

}
*/

const char* getCurrentProcessStatus() {
  static char status[64];

  if (isPasteurizing) {
    switch (pastPhase) {
      //case PAST_PHASE_IDLE:      return "Pasteurization: Idle";
      case PAST_PHASE_LEVEL_CHECK: return "Pasteurization: LevelCheck";
      case PAST_PHASE_HEATING:     return "Pasteurization: Heating";
      case PAST_PHASE_HOLDING:     return "Pasteurization: Holding";
      case PAST_PHASE_COOLING:     return "Pasteurization: Cooling";
      case PAST_PHASE_DONE:        return "Pasteurization is Completed";
    }
  }

  if (isCheeseMaking) {
    switch (cmPhase) {
      //case CM_PHASE_IDLE:  return "CheeseMaking: Idle";
      case CM_PHASE_LEVEL_CHECK: return "CheeseMaking: LevelCheck";
      case CM_PHASE_HEATING:     return "CheeseMaking: Heating";
      case CM_PHASE_HOLDING:     return "CheeseMaking: Holding";
      case CM_PHASE_COOLING:     return "CheeseMaking: Cooling";
      case CM_PHASE_DONE:        return "CheeseMaking is Completed";
    }
  }

  if (isCooling) {
    switch (coolStage) {
      case COOL_STAGE_START:     return "Cooling started";
      case COOL_STAGE_DONE:      return "Cooling completed";
    }
  }
  if (isMilkRefilling) { 
    switch (milkStage) {
      case MILK_STAGE_START:     return "Milk Refilling started";
      case MILK_STAGE_DONE:      return "Milk Refilling completed";
    }
  }
  if (isWaterRefilling){ 
    switch (WRStage) {
      case WR_STAGE_START:     return "Water Refilling started";
      case WR_STAGE_DONE:      return "Water Refilling completed";
    }
  }
  if (isWaterDrainage) { 
    switch (WDStage) {
      case WD_STAGE_START:     return "Water Drainage started";
      case WD_STAGE_DONE:      return "Water Drainage completed";
   }
  }
  if (StopButtonPressed) {
      return "User stopped the process.";}

  return "System Idle";
}
