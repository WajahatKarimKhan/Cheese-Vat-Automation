#include "schedule_data.h"
#include "prg_data.h"
#include "process_execution.h"
#include "UARTCom.h"
#include "uRTCLib.h"
#include "rtc.h"
#include "constant.h"
#include <UTFT.h>
#include <UTouch.h>
#include "gui.h"
#include "global_var.h"
#include "gui_fsm.h"

Schedule pasteurizationSchedule = {"Pasteurization", "00/00/25", "00:00", 0, 0};
Schedule cheeseMakingSchedule   = {"Cheese Making",  "00/00/25", "00:00", 0, 0};
Schedule coolingSchedule        = {"Cooling",        "00/00/25", "00:00", 0, 0};
Schedule cheeseDrainageSchedule = {"Cheese Drainage","00/00/25", "00:00", 0, 0};
Schedule waterDrainageSchedule  = {"Water Drain",    "00/00/25", "00:00", 0, 0};
Schedule waterRefillSchedule    = {"Water Refill",   "00/00/25", "00:00", 0, 0};
Schedule milkRefillSchedule     = {"Milk Refill",    "00/00/25", "00:00", 0, 0};
// Global touch and display objects
extern UTFT myGLCD;
extern UTouch myTouch;

// Generic pointer for active schedule
Schedule* activeSchedule = nullptr;

extern Program* activeProgram;

// Keep track of current input
char currentInput[12] = "";
uint8_t inputPos = 0;
InputFieldSchedule activeFieldSchedule = NONE;

void updateDisplayFieldsSchedule() {
  myGLCD.setFont(BigFont);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);

  if (activeSchedule != nullptr) {
  drawBoxCenteredText(activeSchedule->date, 200, 75, 440, 105);
  drawBoxCenteredText(activeSchedule->time, 200, 125, 330, 155);
  }
  
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(SmallFont);

  // Clear all markers
  myGLCD.setColor(60, 60, 60);  // background
  myGLCD.fillRect(131, 186, 149, 204);  // clear Once marker
  myGLCD.fillRect(131, 226, 149, 244);  // clear Daily marker
  myGLCD.fillRect(131, 266, 149, 284);  // clear Weekly marker

  // Highlight active repeat
  //myGLCD.setColor(0, 255, 0);
  if (activeSchedule->repeat == 0){
      myGLCD.setColor(0, 255, 0);  // red
      myGLCD.fillRect(130, 185, 150, 205);  // x1, y1, x2, y2 
  } else if (activeSchedule->repeat == 1) { 
      myGLCD.setColor(0, 255, 0);  // red
      myGLCD.fillRect(130, 225, 150, 245);  // x1, y1, x2, y2  
  } else if (activeSchedule->repeat == 2) {
      myGLCD.setColor(0, 255, 0);  // red
      myGLCD.fillRect(130, 265, 150, 285);  // x1, y1, x2, y2 
    }
}

void resetInputBuffer() {
  memset(currentInput, 0, sizeof(currentInput));
  inputPos = 0;
}

void handleKeypadDataSchedule(const char* key) {
  myGLCD.setFont(BigFont);
  if (strcmp(key, "DEL") == 0) {
    if (inputPos > 0) {
      // Don't delete format characters like '/' or ':'
      if (activeFieldSchedule == DATE && (inputPos == 3 || inputPos == 6)) {
        inputPos--;  // skip formatting character
      }
      currentInput[--inputPos] = '\0';
    }
  } 
 
  else if (strcmp(key, "OK") == 0) { 
  //drawFrame(670, 270, 760, 310);
  if (activeSchedule == nullptr) return;

  if (activeFieldSchedule == DATE) {
    strncpy(activeSchedule->date, currentInput, sizeof(activeSchedule->date));
  } else if (activeFieldSchedule == TIME) {
    strncpy(activeSchedule->time, currentInput, sizeof(activeSchedule->time));
    }
  //resetInputBuffer();
  }

 else {
    if (activeFieldSchedule == DATE && inputPos < 8) {
      if (inputPos == 2 || inputPos == 5) {
        currentInput[inputPos++] = '/';
        currentInput[inputPos] = '\0';
      }
    } else if (activeFieldSchedule == TIME && inputPos < 5) {
      if (inputPos == 2) {
        currentInput[inputPos++] = ':';
        currentInput[inputPos] = '\0';
      }
    }

    // Add digit only if within limit
    if ((activeFieldSchedule == DATE && inputPos < 8) || (activeFieldSchedule == TIME && inputPos < 5)) {
      strcat(currentInput, key);
      inputPos++;
    }
  }

  // Live update preview
  if (activeFieldSchedule == DATE) {
    myGLCD.setColor(60, 60, 60);  // Background color
    myGLCD.fillRect(250, 82, 420, 100);  // Adjust width as needed to cover full text area
    myGLCD.setColor(255, 255, 255);  // Text color
    myGLCD.print(currentInput, 250, 82);
    
    
  } else if (activeFieldSchedule == TIME) {
    myGLCD.setColor(60, 60, 60);  // Background color
    myGLCD.fillRect(225, 132, 305, 148);  // Adjust width as needed to cover full text area
    myGLCD.setColor(255, 255, 255);  // Text color
    myGLCD.print(currentInput, 225, 132);
    
  }
}
void checkTouchInputSchedule() {
  if (*currentScreenMode == MODE_SCHEDULING && activeSchedule != nullptr) {
  if (myTouch.dataAvailable()) {
    myTouch.read();
    int x = myTouch.getX();
    int y = myTouch.getY();
    //processTouchAt(x, y);
    //while (myTouch.dataAvailable());
    if (x > 200 && x < 440 && y > 75 && y < 105) {
      activeFieldSchedule = DATE;
      resetInputBuffer();
    } else if (x > 200 && x < 330 && y > 125 && y < 255) {
      activeFieldSchedule = TIME;
      resetInputBuffer();
    } else if (x > 180 && x < 280 && y > 360 && y < 410) {
      // Save button pressed --> just to print the saved values in serial monitor
      drawFrame(180, 360, 280, 410);
      print("Scheduled Process: ");
      println(activeSchedule->name);
      print("Scheduled Date: ");
      println(activeSchedule->date);
      print("Scheduled Time: ");
      println(activeSchedule->time);
      print("Repeat Mode: ");
      switch (activeSchedule->repeat) {
      case 0: println("Once"); break;
      case 1: println("Daily"); break;
      case 2: println("Weekly"); break;
      }
    } // Repeat button: Once
      else if (x > 130 && x < 230 && y > 180 && y < 210) {
      activeSchedule->repeat = 0;
      updateDisplayFieldsSchedule();
    } // Repeat button: Daily
      else if (x > 130 && x < 230 && y > 220 && y < 250) {
      activeSchedule->repeat = 1;
      updateDisplayFieldsSchedule();
    } // Repeat button: Weekly
      else if (x > 130 && x < 230 && y > 260 && y < 290) {
      activeSchedule->repeat = 2;
      updateDisplayFieldsSchedule();
    }
      else {
      handleKeypadPress( x, y);
      }
    }
  }
}


bool hasRunToday = false; 

bool checkSchedule(Schedule* sch) {
  if (sch->triggered) return false;

  rtc.refresh();
  int currHour = rtc.hour();
  int currMin  = rtc.minute();
  int currDay  = rtc.day();
  int currMonth = rtc.month();
  int currYear = rtc.year();

  static int lastDay = -1;
  if (currDay != lastDay) {
    hasRunToday = false;
    lastDay = currDay;
  }

  // Use local copies for strtok
  char timeCopy[10];
  char dateCopy[12];
  strcpy(timeCopy, sch->time);
  strcpy(dateCopy, sch->date);
  //strtok = string token = divides a string into multiple sections
  //atoi = ASCII to integer
  int schHour = atoi(strtok(timeCopy, ":"));
  int schMin  = atoi(strtok(NULL, ":"));

  int schDay = atoi(strtok(dateCopy, "/"));
  int schMonth = atoi(strtok(NULL, "/"));
  int schYear = atoi(strtok(NULL, "/"));

  if (currHour == schHour && currMin == schMin) {
    if (sch->repeat == 0) {
      if (currDay == schDay && currMonth == schMonth && currYear == schYear && !hasRunToday) {
        Serial.println("Schedule matched!.........");
        hasRunToday = true;
        return true;
      }
    } else if (sch->repeat == 1 || sch->repeat == 2) {
      if (!hasRunToday) {
        //Serial.println("Schedule matched!.........");
        hasRunToday = true;
        return true;
      }
    }
  }

  return false;
}


void ifScheduleArrived(){
  //Checking if the scheduled time/date for PASTEURIZATION has arrived
  if (checkSchedule(&pasteurizationSchedule)) {
    programData = pasteurizationProgram;
    isPasteurizing = true;
    pastPhase = PAST_PHASE_LEVEL_CHECK;
    exe_Pasteurization(EXE_AUTO_SCHEDULE, nullptr, &programData);
  } 
  //Checking if the scheduled time/date for CHEESE MAKING has arrived
  else if (checkSchedule(&cheeseMakingSchedule)) {
    programData = cheeseMakingProgram;
    isCheeseMaking = true;
    cmPhase = CM_PHASE_LEVEL_CHECK;
    exe_CheeseMaking(EXE_AUTO_SCHEDULE, nullptr, &programData);
  }
  //Checking if the scheduled time/date for MILK REFILLING has arrived 
  else if (checkSchedule(&milkRefillSchedule)) { 
    //programData = milkRefillProgram;
    isMilkRefilling = true;
    milkStage = MILK_STAGE_START;
    println("Running Milk Refill ...");
    exe_MilkRefill(EXE_AUTO_SCHEDULE, nullptr, &programData);
  } 
  //Checking if the scheduled time/date for WATER DRAINAGE has arrived
  else if (checkSchedule(&waterDrainageSchedule)) {
    isWaterDrainage = true;
    WDStage = WD_STAGE_START;
    println("Running Water Drainage ...");
  } 
  //Checking if the scheduled time/date for WATER REFILL has arrived
  else if (checkSchedule(&waterRefillSchedule)) {
    isWaterRefilling = true;
    WRStage = WR_STAGE_START;
    println("Running Water refill ...");
  } // cheese drainage left

}


