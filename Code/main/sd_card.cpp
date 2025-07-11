#include "sd_card.h"
#include <SD.h>
//#include <SdFat.h>
#include <SPI.h>
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
      logFile.println("Date(d/m/y),Time,Temp1,Temp2,Temp3");
      logFile.close();
    }
  }
}
// ==== logging to sd card ====
void logToSD(float temp1,float temp2,float temp3) {
  
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

  char buffer[64];
  snprintf(buffer, sizeof(buffer), "%02d/%02d/%02d,%02d:%02d:%02d,%.2f,%.2f,%.2f",
           rtc.day(), rtc.month(), rtc.year(), 
           rtc.hour(), rtc.minute(), rtc.second(),
           temp1, temp2, temp3);

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
