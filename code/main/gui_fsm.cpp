#include "gui.h"
#include "gui_fsm.h"
#include "UARTCom.h"
#include "sys_init.h"
#include "utils.h"
#include <UTFT.h>
#include <UTouch.h>
#include "global_var.h"
#include "command.h"
#include "schedule_data.h"
#include "PRG_data.h"
#include "manual_data.h"
#include "view_logs_data.h"
#include "process_execution.h"

#include "sd_card.h"
#include "rtc.h"
#include <Arduino.h>

extern Schedule* activeSchedule;
extern Program* activeProgram;
//extern Logs* activeLogs;
int x, y ;

//=========== GUI FSM ============ 
void runGUI(){
//Main + Modes Screens
  switch(Run_cmd){
    case GUI_MAIN_SCREEN:
      handleMainScreen();
      break;
    //case GUI_SHUTDOWN_SCREEN:
     // handleShutDownScreen();
     // break;
    case GUI_MODE_SCREEN:
      handleModeScreen();
      break;
    //Mode Screens
    case GUI_AUTO_SCHEDULE_SCREEN:
      handleAutoScheduleScreen();
      break;
    case GUI_AUTO_PROCESS_SCREEN:
      handleAutoProcessScreen();
      break;
    case GUI_MANUAL_SCREEN:
      handleManualScreen();
      break;
    case GUI_PROGRAMMING_SCREEN:
      handleProgrammingScreen();
      break;
    case GUI_SERVICE_INTERFACE_SCREEN:
      handleServiceInterfaceScreen();
      break;
//Auto Schedule screens
    case GUI_AS_PASTEURIZATION_SCREEN:
      handleAS_PasteurizationScreen();
      break;
    case GUI_AS_CHEESE_MAKING_SCREEN:
      handleAS_CheeseMakingScreen();
      break;
    case GUI_AS_COOLING_SCREEN:
      handleAS_CoolingScreen();
      break;
    case GUI_AS_CHEESE_DRAINAGE_SCREEN:
      handleAS_CheeseDrainageScreen();
      break;
    case GUI_AS_WATER_DRAINAGE_SCREEN:
      handleAS_WaterDrainageScreen();
      break;
    case GUI_AS_WATER_REFILL_SCREEN:
      handleAS_WaterRefillScreen();
      break;
    case GUI_AS_MILK_REFILL_SCREEN:
      handleAS_MilkRefillScreen();
      break;
//Auto Process Screen
    case GUI_AP_PASTEURIZATION_SCREEN:
      handleAP_PasteurizationScreen();
      break;
    case GUI_AP_CHEESE_MAKING_SCREEN:
      handleAP_CheeseMakingScreen();
      break;
    case GUI_AP_COOLING_SCREEN:
      handleAP_CoolingScreen();
      break;
    case GUI_AP_CHEESE_DRAINAGE_SCREEN:
      handleAP_CheeseDrainageScreen();
      break;
    case GUI_AP_WATER_DRAINAGE_SCREEN:
      handleAP_WaterDrainageScreen();
      break;
    case GUI_AP_WATER_REFILL_SCREEN:
      handleAP_WaterRefillScreen();
      break;
    case GUI_AP_MILK_REFILL_SCREEN:
      handleAP_MilkRefillScreen();
      break;
//Programming mode Screens
    case GUI_PRG_PASTEURIZATION_SCREEN:
      handlePRG_PasteurizationScreen();
      break;
    case GUI_PRG_CHEESE_MAKING_SCREEN:
      handlePRG_CheeseMakingScreen();
      break;
    case GUI_PRG_COOLING_SCREEN:
      handlePRG_CoolingScreen();
      break;

//Manual Mode Screens
    case GUI_MAN_PASTEURIZATION_SCREEN:
      handleMan_PasteurizationScreen();
      break;
    case GUI_MAN_CHEESE_MAKING_SCREEN:
      handleMan_CheeseMakingScreen();
      break;
    case GUI_MAN_COOLING_SCREEN:
      handleMan_CoolingScreen();
      break;
    case GUI_MAN_CHEESE_DRAINAGE_SCREEN:
      handleMan_CheeseDrainageScreen();
      break;
    case GUI_MAN_WATER_DRAINAGE_SCREEN:
      handleMan_WaterDrainageScreen();
      break;
    case GUI_MAN_WATER_REFILL_SCREEN:
      handleMan_WaterRefillScreen();
      break;
    case GUI_MAN_MILK_REFILL_SCREEN:
      handleMan_MilkRefillScreen();
      break;
//Service Interface Screens
    case GUI_SI_RUN_TEST_SCREEN:
      handleSI_RunTestScreen();
      break;
    case GUI_SI_VIEW_LOGS_SCREEN:
      handleSI_ViewLogsScreen();
      break;
    case GUI_SI_RUN_DIAGNOSTICS_SCREEN:
      handleSI_RunDiagnosticsScreen();
      break;
    case GUI_SI_RUN_MONITOR_SENSORS_SCREEN:
      handleSI_RunMonitorSensorsScreen();
      break;

    case GUI_DIAGNOSTICS_TEMP_SCREEN:
      handleDiagnostics_TempScreen();
      break;
    case GUI_DIAGNOSTICS_RTC_SCREEN:
      handleDiagnostics_RTCScreen();
      break;
    case GUI_DIAGNOSTICS_RELAYS_SCREEN:
      handleDiagnostics_RelaysScreen();
      break;
    case GUI_DIAGNOSTICS_AGITATOR_SCREEN:
      handleDiagnostics_AgitatorScreen();
      break;
      case GUI_DIAGNOSTICS_VALVES_SCREEN:
      handleDiagnostics_ValvesScreen();
      break;
    case GUI_DIAGNOSTICS_SDCARD_SCREEN:
      handleDiagnostics_SDcardScreen();
      break;

    case GUI_SD_READ_SCREEN:
      handleSD_ReadScreen();
      break;    

    case GUI_BTN_VIEW_PRESSED:
      handleViewLogs_ViewScreen();
      break;
//Run screens
    case GUI_RUN_PASTEURIZATION_SCREEN:
      handleRun_PasteurizationScreen();
      break;
    case GUI_RUN_CHEESE_MAKING_SCREEN:
      handleRun_CheeseMakingScreen();
      break;
    case GUI_RUN_COOLING_SCREEN:
      handleRun_CoolingScreen();
      break;
    case GUI_RUN_CHEESE_DRAINAGE_SCREEN:
      handleRun_CheeseDrainageScreen();
      break;
    case GUI_RUN_WATER_DRAINAGE_SCREEN:
      handleRun_WaterDrainageScreen();
      break;
    case GUI_RUN_WATER_REFILL_SCREEN:
      handleRun_WaterRefillScreen();
      break;
    case GUI_RUN_MILK_REFILL_SCREEN:
      handleRun_MilkRefillScreen();
      break;

    case GUI_LIVE_DATA_SCREEN:
      handleLiveDataScreen();
      break;
    
      ///adding states
  }
}
//=========== Main + Mode Selection Screens ============ 
void handleMainScreen() {
  static bool initialized = false;
  if (!initialized) {
    drawMainScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
      
    //condition for start up button
    if ((x >= 150) && (x <= 350) && (y >= 110) && (y <= 210)) {
      drawFrame(150, 110, 350, 210);
      Run_cmd = GUI_MODE_SCREEN;
      myGLCD.clrScr();
      initialized = false;  // Re-initialize next screen
    }
    /*
    // If we press the Shut down Button is pressed
    if ((x>=150) && (x<=350) && (y>=260) && (y<=360)) {
      drawFrame(150, 260, 350, 360);
      Run_cmd = GUI_SHUTDOWN_SCREEN;
      myGLCD.clrScr();
    } */
    
    // Add conditions for buttons here
  }
}
/*
void handleShutDownScreen(){
  static bool initialized = false;
  if (!initialized) {
    drawShutDownScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
   
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = GUI_MAIN_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  }
}*/
void handleModeScreen() {
  static bool initialized = false;
  if (!initialized) {
    drawModeScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();

    // if AUTO SCHEDULE is pressed
    if ((x >= 30) && (x <= 250) && (y >= 90) && (y <= 190)) {
      drawFrame(30, 90, 250, 190);
      Run_cmd = GUI_AUTO_SCHEDULE_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }

    // if AUTO PROCESS is pressed
    if ((x >= 290) && (x <= 510) && (y >= 90) && (y <= 190)) {
      drawFrame(290, 90, 510, 190);
      Run_cmd = GUI_AUTO_PROCESS_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }

    // if MANUAL is pressed
    if ((x >= 550) && (x <= 770) && (y >= 90) && (y <= 190)) {
      drawFrame(550, 90, 770, 190);
      Run_cmd = GUI_MANUAL_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }

    // if PROGRAMMING is pressed
    if ((x >= 30) && (x <= 250) && (y >= 230) && (y <= 330)) {
      drawFrame(30, 230, 250, 330);
      Run_cmd = GUI_PROGRAMMING_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }

    // if SERVICE INTERFACE is pressed
    if ((x >= 290) && (x <= 510) && (y >= 230) && (y <= 330)) {
      drawFrame(290, 230, 510, 330);
      Run_cmd = GUI_SERVICE_INTERFACE_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }

    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = GUI_MAIN_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
  }
}
//=========== Mode's Screens ============ 
void handleAutoScheduleScreen() {
  
  drawLiveDate(0, 460, 205, 480);
  drawLiveTime(205, 460, 410, 480);
  
  if (checkSchedule(&pasteurizationSchedule)) {
  programData = pasteurizationProgram;
  exe_Pasteurization(EXE_AUTO_SCHEDULE, nullptr, &programData);
  } else if (checkSchedule(&cheeseMakingSchedule)) {
  programData = cheeseMakingProgram;
  exe_CheeseMaking(EXE_AUTO_SCHEDULE, nullptr, &programData);
  } else if (checkSchedule(&coolingSchedule)) {
  programData = coolingProgram;
  //exe_Cooling(EXE_AUTO_SCHEDULE, nullptr, &programData);
  } 
  else if (checkSchedule(&milkRefillSchedule)) {
  //programData = milkRefillProgram;
  exe_MilkRefill(EXE_AUTO_SCHEDULE, nullptr, &programData);
  } else if (checkSchedule(&waterDrainageSchedule)) {
  exe_WaterDrainage();
  } else if (checkSchedule(&waterRefillSchedule)) {
  exe_WaterRefill();
  } // cheese drainage left


  static ScreenMode modeScheduling = MODE_SCHEDULING;
  currentScreenMode = &modeScheduling;
  // Reset all active pointers — ONLY one should be active at a time
  activeProgram = nullptr;
  activeManual = nullptr;
  activeSchedule = nullptr;
  static bool initialized = false;
  if (!initialized) {
    drawAutoScheduleScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();

  //if PATEURIZATION button is pressed
    if ((x >= 30) && (x <= 260) && (y >= 90) && (y <= 190)) {
      drawFrame(30, 90, 260, 190);
      Run_cmd = GUI_AS_PASTEURIZATION_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
  //if CHEESE_MAKING button is pressed
    if ((x >= 280) && (x <= 430) && (y >= 90) && (y <= 190)) {
      drawFrame(280, 90, 430, 190);
      Run_cmd = GUI_AS_CHEESE_MAKING_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
  //if COOLING button is pressed
    if ((x >= 450) && (x <= 600) && (y >= 90) && (y <= 190)) {
      drawFrame(450, 90, 600, 190);
      Run_cmd = GUI_AS_COOLING_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
  //if CHEESE_DRAINAGE button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 90) && (y <= 190)) {
      drawFrame( 620, 90, 770, 190);
      Run_cmd = GUI_AS_CHEESE_DRAINAGE_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
  //if WATER_DRAINAGE button is pressed
    if ((x >= 30) && (x <= 260) && (y >= 230) && (y <= 330)) {
      drawFrame(30, 230, 260, 330);
      Run_cmd = GUI_AS_WATER_DRAINAGE_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
  //if WATER_REFILL button is pressed
    if ((x >= 280) && (x <= 430) && (y >= 230) && (y <= 330)) {
      drawFrame(280, 230, 430, 330);
      Run_cmd = GUI_AS_WATER_REFILL_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
  //if MILK_REFILL button is pressed
    if ((x >= 450) && (x <= 600) && (y >= 230) && (y <= 330)) {
      drawFrame(450, 230, 600, 330);
      Run_cmd = GUI_AS_MILK_REFILL_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    //if live data button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 230) && (y <= 330)) {
      drawFrame( 620, 230, 770, 330);
      previousRunCmd = Run_cmd;   // Save where we came from
      Run_cmd = GUI_LIVE_DATA_SCREEN; // Go to live data
      myGLCD.clrScr();
      initialized = false;
    }
      
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = GUI_MODE_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  }
}
void handleAutoProcessScreen() {
  static bool initialized = false;
  if (!initialized) {
    drawAutoProcessScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
  //if PATEURIZATION button is pressed
    if ((x >= 30) && (x <= 260) && (y >= 90) && (y <= 190)) {
      drawFrame(30, 90, 260, 190);
      Run_cmd = GUI_AP_PASTEURIZATION_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
  //if CHEESE_MAKING button is pressed
    if ((x >= 280) && (x <= 430) && (y >= 90) && (y <= 190)) {
      drawFrame(280, 90, 430, 190);
      Run_cmd = GUI_AP_CHEESE_MAKING_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
  //if COOLING button is pressed
    if ((x >= 450) && (x <= 600) && (y >= 90) && (y <= 190)) {
      drawFrame(450, 90, 600, 190);
      Run_cmd = GUI_AP_COOLING_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
  //if CHEESE_DRAINAGE button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 90) && (y <= 190)) {
      drawFrame(620, 90, 770, 190);
      Run_cmd = GUI_AP_CHEESE_DRAINAGE_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
  //if WATER_DRAINAGE button is pressed
    if ((x >= 30) && (x <= 260) && (y >= 230) && (y <= 330)) {
      drawFrame(30, 230, 260, 330);
      Run_cmd = GUI_AP_WATER_DRAINAGE_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
  //if WATER_REFILL button is pressed
    if ((x >= 280) && (x <= 430) && (y >= 230) && (y <= 330)) {
      drawFrame(280, 230, 430, 330);
      Run_cmd = GUI_AP_WATER_REFILL_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
  //if MILK_REFILL button is pressed
    if ((x >= 450) && (x <= 600) && (y >= 230) && (y <= 330)) {
      drawFrame(450, 230, 600, 330);
      Run_cmd = GUI_AP_MILK_REFILL_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }

    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = GUI_MODE_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  
  }
}
void handleManualScreen() {
  static ScreenMode modeManual= MODE_MANUAL;
  currentScreenMode = &modeManual;
 
  static bool initialized = false;
  if (!initialized) {
    drawManualScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();

    //if PATEURIZATION button is pressed
    if ((x >= 30) && (x <= 260) && (y >= 90) && (y <= 190)) {
      drawFrame(30, 90, 260, 190);
      Run_cmd = GUI_MAN_PASTEURIZATION_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
  //if CHEESE_MAKING button is pressed
    if ((x >= 280) && (x <= 430) && (y >= 90) && (y <= 190)) {
      drawFrame(280, 90, 430, 190);
      Run_cmd = GUI_MAN_CHEESE_MAKING_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
  //if COOLING button is pressed
    if ((x >= 450) && (x <= 600) && (y >= 90) && (y <= 190)) {
      drawFrame(450, 90, 600, 190);
      Run_cmd = GUI_MAN_COOLING_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
  //if CHEESE_DRAINAGE button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 90) && (y <= 190)) {
      drawFrame(620, 90, 770, 190);
      Run_cmd = GUI_MAN_CHEESE_DRAINAGE_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
  //if WATER_DRAINAGE button is pressed
    if ((x >= 30) && (x <= 260) && (y >= 230) && (y <= 330)) {
      drawFrame(30, 230, 260, 330);
      Run_cmd = GUI_MAN_WATER_DRAINAGE_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
  //if WATER_REFILL button is pressed
    if ((x >= 280) && (x <= 430) && (y >= 230) && (y <= 330)) {
      drawFrame(280, 230, 430, 330);
      Run_cmd = GUI_MAN_WATER_REFILL_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
  //if MILK_REFILL button is pressed
    if ((x >= 450) && (x <= 600) && (y >= 230) && (y <= 330)) {
      drawFrame(450, 230, 600, 330);
      Run_cmd = GUI_MAN_MILK_REFILL_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    //if live data button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 230) && (y <= 330)) {
      drawFrame(450, 230, 600, 330);
      previousRunCmd = Run_cmd;   // Save where we came from
      Run_cmd = GUI_LIVE_DATA_SCREEN; // Go to live data
      myGLCD.clrScr();
      initialized = false;
    }
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = GUI_MODE_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  }
}
void handleProgrammingScreen() {
  static ScreenMode modeProgramming = MODE_PROGRAMMING;
  currentScreenMode = &modeProgramming;
  // Reset all active pointers — ONLY one should be active at a time
  activeProgram = nullptr;
  activeManual = nullptr;
  activeSchedule = nullptr;
  static bool initialized = false;
  if (!initialized) {
    drawProgrammingScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();

  //if PATEURIZATION button is pressed
    if ((x >= 30) && (x <= 260) && (y >= 175) && (y <= 275)) {
      drawFrame(30, 175, 260, 275);
      Run_cmd = GUI_PRG_PASTEURIZATION_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
  //if CHEESE_MAKING button is pressed
    if ((x >= 285) && (x <= 515) && (y >= 175) && (y <= 275)) {
      drawFrame(285, 175, 515, 275);
      Run_cmd = GUI_PRG_CHEESE_MAKING_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
  //if COOLING button is pressed
    if ((x >= 540) && (x <= 770) && (y >= 175) && (y <= 275)) {
      drawFrame(540, 175, 770, 275);
      Run_cmd = GUI_PRG_COOLING_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }    
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = GUI_MODE_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  }
}
void handleServiceInterfaceScreen() {
  static bool initialized = false;
  if (!initialized) {
    drawServiceInterfaceScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
    
    
    // if Run test button pressed
    if ((x >= 150) && (x <= 330) && (y >= 180) && (y <= 280)) {
      drawFrame(150, 180, 330, 280);
      Run_cmd = GUI_SI_RUN_TEST_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    // if View Logs button pressed
    if ((x >= 470) && (x <= 650) && (y >= 180) && (y <= 280)) {
      drawFrame(470, 180, 650, 280);
      Run_cmd = GUI_SI_VIEW_LOGS_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }  
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = GUI_MODE_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  }
}

//=========== Auto-Schedule Processes Screens ============ 
void handleAS_PasteurizationScreen(){
 
  activeSchedule = &pasteurizationSchedule;
  static bool initialized = false;
  if (!initialized) {
    drawAS_PasteurizationScreen();
    initialized = true;
  }
  

  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
   //Save button area
  if (x > 155 && x < 295 && y > 340 && y < 380) {
   drawFrame(155, 340, 295, 380);
    

    }
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = GUI_AUTO_SCHEDULE_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  }
}
void handleAS_CheeseMakingScreen(){
  activeSchedule = &cheeseMakingSchedule;
  static bool initialized = false;
  if (!initialized) {
    drawAS_CheeseMakingScreen();
    //activeSchedule = &pasteurizationSchedule;
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();

   // Save button area
  // if (x > 155 && x < 295 && y > 340 && y < 380) {
   // drawFrame(155, 340, 295, 380);
   // saveCurrentInputToSchedule();
  //}
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = GUI_AUTO_SCHEDULE_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  }  
}
void handleAS_CoolingScreen(){
  activeSchedule = &coolingSchedule;
  static bool initialized = false;
  if (!initialized) {
    drawAS_CoolingScreen();
    //activeSchedule = &pasteurizationSchedule;
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();

   // Save button area
  // if (x > 155 && x < 295 && y > 340 && y < 380) {
   // drawFrame(155, 340, 295, 380);
   // saveCurrentInputToSchedule();
  //}
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = GUI_AUTO_SCHEDULE_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  }
}
void handleAS_CheeseDrainageScreen(){
  activeSchedule = &cheeseDrainageSchedule;
  static bool initialized = false;
  if (!initialized) {
    drawAS_CheeseDrainageScreen();
    //activeSchedule = &pasteurizationSchedule;
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();

   // Save button area
  // if (x > 155 && x < 295 && y > 340 && y < 380) {
   // drawFrame(155, 340, 295, 380);
   // saveCurrentInputToSchedule();
    //}
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = GUI_AUTO_SCHEDULE_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  }
}
void handleAS_WaterDrainageScreen(){
  activeSchedule = &waterDrainageSchedule;
  static bool initialized = false;
  if (!initialized) {
    drawAS_WaterDrainageScreen();
    //activeSchedule = &pasteurizationSchedule;
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();

   // Save button area
  // if (x > 155 && x < 295 && y > 340 && y < 380) {
   // drawFrame(155, 340, 295, 380);
   // saveCurrentInputToSchedule();
  //}
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = GUI_AUTO_SCHEDULE_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  }
}
void handleAS_WaterRefillScreen(){
  activeSchedule = &waterRefillSchedule;
  static bool initialized = false;
  if (!initialized) {
    drawAS_WaterRefillScreen();
    //activeSchedule = &pasteurizationSchedule;
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();


   
   // Save button area
  // if (x > 155 && x < 295 && y > 340 && y < 380) {
   // drawFrame(155, 340, 295, 380);
   // saveCurrentInputToSchedule();
 //}
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = GUI_AUTO_SCHEDULE_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  }
}
void handleAS_MilkRefillScreen(){
  activeSchedule = &milkRefillSchedule;
  static bool initialized = false;
  if (!initialized) {
    drawAS_MilkRefillScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();

   // Save button area
  // if (x > 155 && x < 295 && y > 340 && y < 380) {
   // drawFrame(155, 340, 295, 380);
   // saveCurrentInputToSchedule();
 //}
    
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = GUI_AUTO_SCHEDULE_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  }
}

//=========== Auto-Processes Screens ============ 
void handleAP_PasteurizationScreen(){
  static bool initialized = false;
  if (!initialized) {
    drawAP_PasteurizationScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();

    //if Milk refill is pressed
    if ((x >= 180) && (x <= 280) && (y >= 410) && (y <= 460)) {
      drawFrame(180, 410, 280, 460);
      Run_cmd = GUI_AP_MILK_REFILL_SCREEN; 
      myGLCD.clrScr();
      initialized = false;
    }

    //if RUN button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 110) && (y <= 210)) {
      drawFrame(620, 110, 770, 210);
      previousRunCmd = Run_cmd;   // Save where we came from
      Run_cmd = GUI_RUN_PASTEURIZATION_SCREEN;
      myGLCD.clrScr();
      // Set mode and trigger execution code
      programData = pasteurizationProgram;
      exe_Pasteurization(EXE_AUTO_PROCESS, nullptr, &programData);
      initialized = false;
    }
    //if live data button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 230) && (y <= 330)) {
      drawFrame(620, 230, 770, 330);
      previousRunCmd = Run_cmd;   // Save where we came from
      Run_cmd = GUI_LIVE_DATA_SCREEN; // Go to live data
      myGLCD.clrScr();
      initialized = false;
    }
    //if stop button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 350) && (y <= 450)) {
      drawFrame(620, 350, 770, 450);
      StopButtonPressed = true;
      myGLCD.clrScr();
      initialized = false;
    }
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = GUI_AUTO_PROCESS_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  }  
}
void handleAP_CheeseMakingScreen(){
  static bool initialized = false;
  if (!initialized) {
    drawAP_CheeseMakingScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();

    //if Milk refill is pressed
    if ((x >= 180) && (x <= 280) && (y >= 410) && (y <= 460)) {
      drawFrame(180, 410, 280, 460);
      Run_cmd = GUI_AP_MILK_REFILL_SCREEN; 
      myGLCD.clrScr();
      initialized = false;
    }
    //if RUN button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 110) && (y <= 210)) {
      drawFrame(620, 110, 770, 210);
      previousRunCmd = Run_cmd;   // Save where we came from
      Run_cmd = GUI_RUN_CHEESE_MAKING_SCREEN; 
      myGLCD.clrScr();
      // Set mode and trigger execution code
      programData = cheeseMakingProgram;
      exe_CheeseMaking(EXE_AUTO_PROCESS, nullptr, &programData);
      initialized = false;
    }
    //if live data button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 230) && (y <= 330)) {
      drawFrame(620, 230, 770, 330);
      previousRunCmd = Run_cmd;   // Save where we came from
      Run_cmd = GUI_LIVE_DATA_SCREEN; // Go to live data
      myGLCD.clrScr();
      initialized = false;
    }
    //if stop button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 350) && (y <= 450)) {
      drawFrame(620, 350, 770, 450);
      StopButtonPressed = true;
      myGLCD.clrScr();
      initialized = false;
    }
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = GUI_AUTO_PROCESS_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  }
}
void handleAP_CoolingScreen(){
  static bool initialized = false;
  if (!initialized) {
    drawAP_CoolingScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
    //if RUN button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 110) && (y <= 210)) {
      drawFrame(620, 110, 770, 210);
      previousRunCmd = Run_cmd;   // Save where we came from
      Run_cmd = GUI_RUN_COOLING_SCREEN;
      myGLCD.clrScr(); 
      // Set mode and trigger execution code
      programData = coolingProgram;
      //exe_Cooling(EXE_AUTO_PROCESS, nullptr, &programData);
      initialized = false;
    }
    //if live data button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 230) && (y <= 330)) {
      drawFrame(620, 230, 770, 330);
      previousRunCmd = Run_cmd;   // Save where we came from
      Run_cmd = GUI_LIVE_DATA_SCREEN; // Go to live data
      myGLCD.clrScr();
      initialized = false;
    }
    //if stop button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 350) && (y <= 450)) {
      drawFrame(620, 350, 770, 450);
      StopButtonPressed = true;
      myGLCD.clrScr();
      initialized = false;
    }
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = GUI_AUTO_PROCESS_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  }
}
void handleAP_CheeseDrainageScreen(){
  static bool initialized = false;
  if (!initialized) {
    drawAP_CheeseDrainageScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
    //if RUN button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 110) && (y <= 210)) {
      drawFrame(620, 110, 770, 210);
      previousRunCmd = Run_cmd;   // Save where we came from
      Run_cmd = GUI_RUN_CHEESE_DRAINAGE_SCREEN; 
      ///cd
      myGLCD.clrScr();
      initialized = false;
    }
    //if live data button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 230) && (y <= 330)) {
      drawFrame(620, 230, 770, 330);
      previousRunCmd = Run_cmd;   // Save where we came from
      Run_cmd = GUI_LIVE_DATA_SCREEN; // Go to live data
      myGLCD.clrScr();
      initialized = false;
    }
    //if stop button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 350) && (y <= 450)) {
      drawFrame(620, 350, 770, 450);
      StopButtonPressed = true;
      myGLCD.clrScr();
      initialized = false;
    }
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = GUI_AUTO_PROCESS_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  }
}
void handleAP_WaterDrainageScreen(){
  static bool initialized = false;
  if (!initialized) {
    drawAP_WaterDrainageScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
    //if RUN button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 110) && (y <= 210)) {
      drawFrame(620, 110, 770, 210);
      previousRunCmd = Run_cmd;   // Save where we came from
      Run_cmd = GUI_RUN_WATER_DRAINAGE_SCREEN;
      myGLCD.clrScr();
      // Set mode and trigger execution code
      //programData = waterDrainageProgram;
      exe_WaterDrainage(); 
      initialized = false;
    }
    //if live data button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 230) && (y <= 330)) {
      drawFrame(450, 230, 600, 330);
      previousRunCmd = Run_cmd;   // Save where we came from
      Run_cmd = GUI_LIVE_DATA_SCREEN; // Go to live data
      myGLCD.clrScr();
      initialized = false;
    }
    //if stop button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 350) && (y <= 450)) {
      drawFrame(620, 350, 770, 450);
      StopButtonPressed = true;
      myGLCD.clrScr();
      initialized = false;
    }
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = GUI_AUTO_PROCESS_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  }
}
void handleAP_WaterRefillScreen(){
  static bool initialized = false;
  if (!initialized) {
    drawAP_WaterRefillScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
    //if RUN button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 110) && (y <= 210)) {
      drawFrame(620, 110, 770, 210);
      previousRunCmd = Run_cmd;   // Save where we came from
      Run_cmd = GUI_RUN_WATER_REFILL_SCREEN; 
      myGLCD.clrScr();
      // Set mode and trigger execution code
      //programData = waterRefillProgram;
      exe_WaterRefill(); 
      initialized = false;
    }
    //if live data button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 230) && (y <= 330)) {
      drawFrame(620, 230, 770, 330);
      previousRunCmd = Run_cmd;   // Save where we came from
      Run_cmd = GUI_LIVE_DATA_SCREEN; // Go to live data
      myGLCD.clrScr();
      initialized = false;
    }
    //if stop button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 350) && (y <= 450)) {
      drawFrame(620, 350, 770, 450);
      StopButtonPressed = true;
      myGLCD.clrScr();
      initialized = false;
    }
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = GUI_AUTO_PROCESS_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  }
}
void handleAP_MilkRefillScreen(){
  static bool initialized = false;
  if (!initialized) {
    drawAP_MilkRefillScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
    //if RUN button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 110) && (y <= 210)) {
      drawFrame(620, 110, 770, 210);
      previousRunCmd = Run_cmd;   // Save where we came from
      Run_cmd = GUI_RUN_MILK_REFILL_SCREEN;
      myGLCD.clrScr();
      // Set mode and trigger execution code
      //programData = milkRefillProgram;
      exe_MilkRefill(EXE_AUTO_PROCESS, nullptr, &programData);  
      initialized = false;
    }
    //if live data button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 230) && (y <= 330)) {
      drawFrame(620, 230, 770, 330);
      previousRunCmd = Run_cmd;   // Save where we came from
      Run_cmd = GUI_LIVE_DATA_SCREEN; // Go to live data
      myGLCD.clrScr();
      initialized = false;
    }
    //if stop button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 350) && (y <= 450)) {
      drawFrame(620, 350, 770, 450);
      StopButtonPressed = true;
      myGLCD.clrScr();
      initialized = false;
    }
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = GUI_AUTO_PROCESS_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  }
}

//=========== Programming Screens ============
void handlePRG_PasteurizationScreen(){
  activeProgram = &pasteurizationProgram;
  static bool initialized = false;
  if (!initialized) {
    drawPRG_PasteurizationScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();

  // Rest of the code is in the prg_data.cpp that handles data inputs from user 

    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = GUI_PROGRAMMING_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  }
}
void handlePRG_CheeseMakingScreen(){
  activeProgram = &cheeseMakingProgram;
  static bool initialized = false;
  if (!initialized) {
    drawPRG_CheeseMakingScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();

  // Rest of the code is in the prg_data.cpp that handles data inputs from user 


    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = GUI_PROGRAMMING_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  }
}
void handlePRG_CoolingScreen(){
  activeProgram = &coolingProgram;
  static bool initialized = false;
  if (!initialized) {
    drawPRG_CoolingScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();

  // Rest of the code is in the prg_data.cpp that handles data inputs from user 

    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = GUI_PROGRAMMING_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  }
}
//=========== Manual Processes Screens ============
void handleMan_PasteurizationScreen(){
  activeManual = &pasteurizationManual;
  static bool initialized = false;
  if (!initialized) {
    drawMan_PasteurizationScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();

  // Rest of the code is in the manual_data.cpp that handles data inputs from user 
    //if RUN button is pressed
    if ((x >= 470) && (x <= 560) && (y >= 360) && (y <= 410)) {
      drawFrame(470, 360, 560, 410);
      previousRunCmd = Run_cmd;   // Save where we came from
      Run_cmd = GUI_RUN_PASTEURIZATION_SCREEN; 
      myGLCD.clrScr();
      // Set mode and trigger execution code
      manualData = pasteurizationManual;
      isPasteurizing = true;
      // Set execution parameters
      exe_Pasteurization(EXE_MANUAL, &manualData, nullptr );
      pastPhase = PAST_PHASE_LEVEL_CHECK; 
      initialized = false;
    }
    //if stop button is pressed
    if ((x >= 670) && (x <= 770) && (y >= 360) && (y <= 410)) {
      drawFrame(460, 360, 560, 410); 
      StopButtonPressed = true;
      myGLCD.clrScr();
      initialized = false;
    }
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = GUI_MANUAL_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  }
}
void handleMan_CheeseMakingScreen(){
  activeManual = &cheeseMakingManual;
  static bool initialized = false;
  if (!initialized) {
    drawMan_CheeseMakingScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();

  // Rest of the code is in the manual_data.cpp that handles data inputs from user 
    //if RUN button is pressed
    if ((x >= 470) && (x <= 560) && (y >= 360) && (y <= 410)) {
      drawFrame(470, 360, 560, 410);
      previousRunCmd = Run_cmd;   // Save where we came from
      Run_cmd = GUI_RUN_CHEESE_MAKING_SCREEN; 
      myGLCD.clrScr();
      //execution
      manualData = cheeseMakingManual;
      isCheeseMaking = true;
      cmPhase = CM_PHASE_LEVEL_CHECK;
      exe_CheeseMaking(EXE_MANUAL, &manualData, nullptr );
      initialized = false;
    }
    //if stop button is pressed
    if ((x >= 670) && (x <= 770) && (y >= 360) && (y <= 410)) {
      drawFrame(460, 360, 560, 410);
      StopButtonPressed = true;
      myGLCD.clrScr();
      initialized = false;
    }
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = GUI_MANUAL_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  }
}
void handleMan_CoolingScreen(){
  activeManual = &coolingManual;
  static bool initialized = false;
  if (!initialized) {
    drawMan_CoolingScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();

  // Rest of the code is in the manual_data.cpp that handles data inputs from user 

    //if RUN button is pressed
    if ((x >= 470) && (x <= 560) && (y >= 360) && (y <= 410)) {
      drawFrame(470, 360, 560, 410);
      previousRunCmd = Run_cmd;   // Save where we came from
      Run_cmd = GUI_RUN_COOLING_SCREEN; 
      myGLCD.clrScr();
      //execution
      //manualData = coolingManual;
      //exe_Cooling(EXE_MANUAL, &manualData, nullptr );
      initialized = false;
    }
    //if stop button is pressed
    if ((x >= 670) && (x <= 770) && (y >= 360) && (y <= 410)) {
      drawFrame(460, 360, 560, 410);
      StopButtonPressed = true; 
      myGLCD.clrScr();
      initialized = false;
    }
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = GUI_MANUAL_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  }
}
void handleMan_CheeseDrainageScreen(){
  static bool initialized = false;
  if (!initialized) {
    drawMan_CheeseDrainageScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();

  // Rest of the code is in the manual_data.cpp that handles data inputs from user 
    //if RUN button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 110) && (y <= 210)) {
      drawFrame(620, 110, 770, 210);
      previousRunCmd = Run_cmd;   // Save where we came from
      Run_cmd = GUI_RUN_CHEESE_DRAINAGE_SCREEN; 
      myGLCD.clrScr();
      initialized = false;
    }
    //if stop button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 230) && (y <= 330)) {
      drawFrame(620, 230, 770, 330);
      StopButtonPressed = true; 
      myGLCD.clrScr();
      initialized = false;
    }
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = GUI_MANUAL_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  }
}
void handleMan_WaterDrainageScreen(){
  static bool initialized = false;
  if (!initialized) {
    drawMan_WaterDrainageScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();

  // Rest of the code is in the manual_data.cpp that handles data inputs from user 
    if ((x >= 620) && (x <= 770) && (y >= 110) && (y <= 210)) {
      drawFrame(620, 110, 770, 210);
      previousRunCmd = Run_cmd;   // Save where we came from
      Run_cmd = GUI_RUN_WATER_DRAINAGE_SCREEN;
      myGLCD.clrScr(); 
      //execution
      //manualData = waterDrainageManual;
      isWaterDrainage = true;
      WDStage = WD_STAGE_START;
      println("Running Water Drainage ...");
      initialized = false;
    }
    //if stop button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 230) && (y <= 330)) {
      drawFrame(620, 230, 770, 330);
      StopButtonPressed = true; 
      myGLCD.clrScr();
      initialized = false;
    }
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = GUI_MANUAL_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  }
}
void handleMan_WaterRefillScreen(){
  static bool initialized = false;
  if (!initialized) {
    drawMan_WaterRefillScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();

  // Rest of the code is in the manual_data.cpp that handles data inputs from user 
    // Rest of the code is in the manual_data.cpp that handles data inputs from user 
    if ((x >= 620) && (x <= 770) && (y >= 110) && (y <= 210)) {
      drawFrame(620, 110, 770, 210);
      previousRunCmd = Run_cmd;   // Save where we came from
      Run_cmd = GUI_RUN_WATER_REFILL_SCREEN;
      myGLCD.clrScr();
      //execution
      //manualData = waterRefillManual;
      isWaterRefilling = true;
      WRStage = WR_STAGE_START;
      println("Running Water refill ...");
      initialized = false;
    }
    //if stop button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 230) && (y <= 330)) {
      drawFrame(620, 230, 770, 330);
      StopButtonPressed = true;
      myGLCD.clrScr();
      initialized = false;
    }
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = GUI_MANUAL_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  }
}
void handleMan_MilkRefillScreen(){
  activeManual = &milkRefillManual;
  static bool initialized = false;
  if (!initialized) {
    drawMan_MilkRefillScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();

  // Rest of the code is in the manual_data.cpp that handles data inputs from user 

    //if RUN button is pressed
    if ((x >= 470) && (x <= 560) && (y >= 360) && (y <= 410)) {
      drawFrame(470, 360, 560, 410);
      previousRunCmd = Run_cmd;   // Save where we came from
      Run_cmd = GUI_RUN_MILK_REFILL_SCREEN;
      myGLCD.clrScr();
      //execution
      manualData = milkRefillManual;
      isMilkRefilling = true;
      milkStage = MILK_STAGE_START;
      println("Running Milk Refill ...");
      //exe_MilkRefill(EXE_MANUAL, &manualData, nullptr ); 
      initialized = false;
    }
    //if stop button is pressed
    if ((x >= 670) && (x <= 770) && (y >= 360) && (y <= 410)) {
      drawFrame(460, 360, 560, 410);
      StopButtonPressed = true;
      myGLCD.clrScr();
      initialized = false;
    }
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = GUI_MANUAL_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  }
}
//========= Service Interface Screens ============
void handleSI_RunTestScreen() {
  static bool initialized = false;
  if (!initialized) {
    drawSI_RunTestScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
    // if Diagnostics button pressed
    if ((x >= 150) && (x <= 330) && (y >= 180) && (y <= 280)) {
      drawFrame(150, 180, 330, 280);
      Run_cmd = GUI_SI_RUN_DIAGNOSTICS_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    // if Monitor sensors button pressed
    if ((x >= 470) && (x <= 650) && (y >= 180) && (y <= 280)) {
      drawFrame(470, 180, 650, 280);
      //Run_cmd = GUI_SI_RUN_MONITOR_SENSORS_SCREEN;
      previousRunCmd = Run_cmd;   // Save where we came from
      Run_cmd = GUI_LIVE_DATA_SCREEN; // Go to live data
      myGLCD.clrScr();
      initialized = false;
    }  
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = GUI_SERVICE_INTERFACE_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  }
}
void handleSI_ViewLogsScreen() {
  static ScreenMode modeLogs = MODE_LOGS;
  currentScreenMode = &modeLogs;
  activeLogs = &filter;
  checkDropdownTouch();
  static bool initialized = false;
  if (!initialized) {
    drawSI_ViewLogsScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
    
    // if Back button pressed
    if ((x >= 180) && (x <= 280) && (y >= 360) && (y <= 410)) {
      drawFrame(180, 360, 280, 410);
      Run_cmd = GUI_BTN_VIEW_PRESSED;
      myGLCD.clrScr();
      initialized = false;
    }
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = GUI_SERVICE_INTERFACE_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  }

}

//---SI --> Run test
void handleSI_RunDiagnosticsScreen(){
  static bool initialized = false;
  if (!initialized) {
    drawSI_RunDiagnosticsScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
    // if Temperature Sensor button pressed
    if ((x >= 20) && (x <= 260) && (y >= 90) && (y <= 190)) {
      drawFrame(20, 90, 260, 190);
      Run_cmd = GUI_DIAGNOSTICS_TEMP_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    // if RTC button pressed
    if ((x >= 280) && (x <= 520) && (y >= 90) && (y <= 190)) {
      drawFrame(280, 90, 520, 190);
      Run_cmd = GUI_DIAGNOSTICS_RTC_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    // if Relays button pressed
    if ((x >= 540) && (x <= 780) && (y >= 90) && (y <= 190)) {
      drawFrame(540, 90, 780, 190);
      Run_cmd = GUI_DIAGNOSTICS_RELAYS_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    // if Agitator Motor button pressed
    if ((x >= 20) && (x <= 230) && (y >= 260) && (y <= 330)) {
      drawFrame(20, 230, 260, 330);
      Run_cmd = GUI_DIAGNOSTICS_AGITATOR_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    // if Valves button pressed
    if ((x >= 280) && (x <= 520) && (y >= 260) && (y <= 330)) {
      drawFrame(280, 230, 520, 330);
      Run_cmd = GUI_DIAGNOSTICS_VALVES_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    // if Valves button pressed
    if ((x >= 540) && (x <= 780) && (y >= 260) && (y <= 330)) {
      drawFrame(540, 230, 780, 330);
      Run_cmd = GUI_DIAGNOSTICS_SDCARD_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = GUI_SI_RUN_TEST_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  }
}
void handleSI_RunMonitorSensorsScreen(){
  static bool initialized = false;
  if (!initialized) {
    drawSI_RunMonitorSensorsScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
    
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      //Run_cmd = GUI_SI_RUN_TEST_SCREEN;
      Run_cmd = previousRunCmd;
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  }
}
//---SI --> Run test --> Diagnostic
void handleDiagnostics_TempScreen(){
  
  drawFloatLive(TEMP_1.readCelsius(), 400, 150, 470, 170);
  drawFloatLive(TEMP_2.readCelsius(), 400, 200, 470, 220);
  drawFloatLive(TEMP_3.readCelsius(), 400, 250, 470, 270);

  static bool initialized = false;
  if (!initialized) {
    drawDiagnostics_TempScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
    
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = GUI_SI_RUN_DIAGNOSTICS_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  }
}
void handleDiagnostics_RTCScreen(){
  checkRTC();
  drawToggleButton(410, 145, isRTCConnected() , "", ""); //is RTC connected?

  drawLiveDate(410, 195, 600, 225);
  drawLiveTime(410, 245, 600, 275);

  static bool initialized = false;
  if (!initialized) {
    drawDiagnostics_RTCScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
    
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = GUI_SI_RUN_DIAGNOSTICS_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  }
}
void handleDiagnostics_RelaysScreen(){
  static bool initialized = false;
  if (!initialized) {
    drawDiagnostics_RelaysScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
    //WILL DEFINE with hardware integration
    if (x > 600 && x < 700 && y > 75 && y < 110) {
      drawToggleButton(600, 75, true, "ON", "OFF");
    }
    if (x > 600 && x < 700 && y > 195 && y < 225) {
      drawToggleButton(600, 115, true, "ON", "OFF");
    }
    if (x > 600 && x < 700 && y > 275 && y < 305) {
      drawToggleButton(600, 155, false, "ON", "OFF");
    }

    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = GUI_SI_RUN_DIAGNOSTICS_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  }
}
void handleDiagnostics_AgitatorScreen(){
  static bool initialized = false;
  if (!initialized) {
    drawDiagnostics_AgitatorScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
    
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = GUI_SI_RUN_DIAGNOSTICS_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  }
}
void handleDiagnostics_ValvesScreen(){
  static bool initialized = false;
  if (!initialized) {
    drawDiagnostics_ValvesScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
    
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = GUI_SI_RUN_DIAGNOSTICS_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  }
}
void handleDiagnostics_SDcardScreen(){
  checkSDcard();
  drawToggleButton(420, 145, isSDInitialized() , "", ""); //is sd card inserted?
  //delay(200);
  static bool initialized = false;
  if (!initialized) {
    drawDiagnostics_SDcardScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
    

    // if Read button pressed
    if ((x >= 200) && (x <= 300) && (y >= 330) && (y <= 380)) {
      drawFrame(200, 330, 300, 380);
      Run_cmd = GUI_SD_READ_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    // if Write button pressed
    if ((x >= 500) && (x <= 600) && (y >= 330) && (y <= 380)) {
      drawFrame(500, 330, 600, 380);     
      myGLCD.clrScr();
      /*
      File file = SD.open("test.txt", FILE_WRITE);
      if (file) {
        file.println("SD Card Diagnostic Test OK");
        file.close();
        myGLCD.setColor(0, 255, 0);
        myGLCD.print("Write OK!", 20, 250);
      } else {
        myGLCD.setColor(255, 0, 0);
        myGLCD.print("Write Failed!", 20, 250);
      }*/
      initialized = false;
    }
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = GUI_SI_RUN_DIAGNOSTICS_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  }
}

void handleSD_ReadScreen(){
  drawCSV();
  static bool initialized = false;
  if (!initialized) {
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
    
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = GUI_DIAGNOSTICS_SDCARD_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
  }
}


//---SI --> View Logs screen --> Log Viewing screen
void handleViewLogs_ViewScreen(){
  static bool initialized = false;
  if (!initialized) {
    drawViewLogs_ViewScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
    
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = GUI_SI_VIEW_LOGS_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
  }
}

// ----handle Run Screens----
void handleRun_PasteurizationScreen(){
  
  drawFloatLive(TEMP_1.readCelsius(), 350, 75, 420, 100);
  //drawFloatLive(TEMP_2.readCelsius(), 300, 115, 370, 140);
  //drawFloatLive(TEMP_3.readCelsius(), 300, 155, 370, 180);

  static bool initialized = false;
  if (!initialized) {
    drawRun_PasteurizationScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
  
    //if Pause button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 110) && (y <= 210)) {
      drawFrame(620, 110, 770, 210);
      // will implement the process pausing logic 
      myGLCD.clrScr();
      initialized = false;
    }
    //if Resume button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 230) && (y <= 330)) {
      drawFrame(620, 230, 770, 330);
      
      myGLCD.clrScr();
      initialized = false;
    }
    //if Stop button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 350) && (y <= 450)) {
      drawFrame(620, 350, 770, 450);
      StopButtonPressed = true;
      myGLCD.clrScr();
      initialized = false;
    }  
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = previousRunCmd;     // Go back to the screen we came from
      //Run_cmd = GUI_AP_PASTEURIZATION_SCREEN;
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  }  
}
void handleRun_CheeseMakingScreen(){
  
  drawFloatLive(TEMP_1.readCelsius(), 350, 75, 420, 100);
  //drawFloatLive(TEMP_2.readCelsius(), 300, 115, 370, 140);
  //drawFloatLive(TEMP_3.readCelsius(), 300, 155, 370, 180);
  static bool initialized = false;
  if (!initialized) {
    drawRun_CheeseMakingScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
    //if Pause button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 110) && (y <= 210)) {
      drawFrame(620, 110, 770, 210);
      // will implement the process pausing logic 
      myGLCD.clrScr();
      initialized = false;
    }
    //if Resume button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 230) && (y <= 330)) {
      drawFrame(620, 230, 770, 330);
      // will implement the process resuming logic
      myGLCD.clrScr();
      initialized = false;
    }
    //if Stop button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 350) && (y <= 450)) {
      drawFrame(620, 350, 770, 450);
      StopButtonPressed = true;
      myGLCD.clrScr();
      initialized = false;
    } 
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = previousRunCmd;     // Go back to the screen we came from
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  }
}
void handleRun_CoolingScreen(){
  
  drawFloatLive(TEMP_1.readCelsius(), 350, 75, 420, 100);
  //drawFloatLive(TEMP_2.readCelsius(), 300, 115, 370, 140);
  //drawFloatLive(TEMP_3.readCelsius(), 300, 155, 370, 180);
  static bool initialized = false;
  if (!initialized) {
    drawRun_CoolingScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
  
    //if Pause button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 110) && (y <= 210)) {
      drawFrame(620, 110, 770, 210);
      // will implement the process pausing logic 
      myGLCD.clrScr();
      initialized = false;
    }
    //if Resume button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 230) && (y <= 330)) {
      drawFrame(620, 230, 770, 330);
      // will implement the process resuming logic
      myGLCD.clrScr();
      initialized = false;
    }
    //if Stop button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 350) && (y <= 450)) {
      drawFrame(620, 350, 770, 450);
      StopButtonPressed = true;
      myGLCD.clrScr();
      initialized = false;
    }  
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = previousRunCmd;     // Go back to the screen we came from
      myGLCD.clrScr();
      initialized = false;
    }
    // Add conditions for buttons here
  }
}
void handleRun_CheeseDrainageScreen(){
  static bool initialized = false;
  if (!initialized) {
    drawRun_CheeseDrainageScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
  
    //if Pause button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 110) && (y <= 210)) {
      drawFrame(620, 110, 770, 210);
      // will implement the process pausing logic 
      myGLCD.clrScr();
      initialized = false;
    }
    //if Resume button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 230) && (y <= 330)) {
      drawFrame(620, 230, 770, 330);
      // will implement the process resuming logic
      myGLCD.clrScr();
      initialized = false;
    }
    //if Stop button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 350) && (y <= 450)) {
      drawFrame(620, 350, 770, 450);
      StopButtonPressed = true;
      myGLCD.clrScr();
      initialized = false;
    }  
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = previousRunCmd;     // Go back to the screen we came from
      myGLCD.clrScr();
      initialized = false;
    }
    // Add conditions for buttons here
  }
}
void handleRun_WaterDrainageScreen(){
  static bool initialized = false;
  if (!initialized) {
    drawRun_WaterDrainageScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
  
    //if Pause button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 110) && (y <= 210)) {
      drawFrame(620, 110, 770, 210);
      // will implement the process pausing logic 
      myGLCD.clrScr();
      initialized = false;
    }
    //if Resume button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 230) && (y <= 330)) {
      drawFrame(620, 230, 770, 330);
      // will implement the process resuming logic
      myGLCD.clrScr();
      initialized = false;
    }
    //if Stop button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 350) && (y <= 450)) {
      drawFrame(620, 350, 770, 450);
      StopButtonPressed = true;
      myGLCD.clrScr();
      initialized = false;
    }  
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = previousRunCmd;     // Go back to the screen we came from
      myGLCD.clrScr();
      initialized = false;
    }
    // Add conditions for buttons here
  }
}
void handleRun_WaterRefillScreen(){
  static bool initialized = false;
  if (!initialized) {
    drawRun_WaterRefillScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
  
    //if Pause button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 110) && (y <= 210)) {
      drawFrame(620, 110, 770, 210);
      // will implement the process pausing logic 
      myGLCD.clrScr();
      initialized = false;
    }
    //if Resume button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 230) && (y <= 330)) {
      drawFrame(620, 230, 770, 330);
      // will implement the process resuming logic
      myGLCD.clrScr();
      initialized = false;
    }
    //if Stop button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 350) && (y <= 450)) {
      drawFrame(620, 350, 770, 450);
      StopButtonPressed = true;
      myGLCD.clrScr();
      initialized = false;
    }  
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = previousRunCmd;     // Go back to the screen we came from
      myGLCD.clrScr();
      initialized = false;
    }
    // Add conditions for buttons here
  }
}
void handleRun_MilkRefillScreen(){
  static bool initialized = false;
  if (!initialized) {
    drawRun_MilkRefillScreen();
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
  
    //if Pause button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 110) && (y <= 210)) {
      drawFrame(620, 110, 770, 210);
      // will implement the process pausing logic 
      myGLCD.clrScr();
      initialized = false;
    }
    //if Resume button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 230) && (y <= 330)) {
      drawFrame(620, 230, 770, 330);
      // will implement the process resuming logic
      myGLCD.clrScr();
      initialized = false;
    }
    //if Stop button is pressed
    if ((x >= 620) && (x <= 770) && (y >= 350) && (y <= 450)) {
      drawFrame(620, 350, 770, 450);
      StopButtonPressed = true;
      myGLCD.clrScr();
      initialized = false;
    }  
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = previousRunCmd;     // Go back to the screen we came from
      myGLCD.clrScr();
      initialized = false;
    }
    // Add conditions for buttons here
  }
}

ScreenMode modeNone = MODE_NONE;
ScreenMode modeScheduling = MODE_SCHEDULING;
ScreenMode modeProgramming = MODE_PROGRAMMING;
ScreenMode modeManual = MODE_MANUAL;
ScreenMode modeLogs = MODE_LOGS;
ScreenMode* currentScreenMode = &modeNone;

//routing function controlling the system which input a user can put. 
void handleKeypadData(const char* key) {
  if (*currentScreenMode == MODE_SCHEDULING) {
    //Serial.println("Keypad: Scheduling Mode");
    handleKeypadDataSchedule(key);
  } else if (*currentScreenMode == MODE_PROGRAMMING) {
    //Serial.println("Keypad: Programming Mode");
    handleKeypadDataPRG(key);
  } else if (*currentScreenMode == MODE_MANUAL) {
    //Serial.println("Keypad: Manual Mode");
    handleKeypadDataMan(key);
  } else if (*currentScreenMode == MODE_LOGS) {
    //Serial.println("Keypad: Manual Mode");
    handleKeypadDataLogs(key);
  }
}

//it handles keypad button presses
void handleKeypadPress(int x, int y){
  // Keypad logic
    const char* keys[12] = {"1","2","3","4","5","6","7","8","9","DEL","0","OK"};
    int kx[3] = {470, 570, 670};
    int ky[4] = {120, 170, 220, 270};

    for (int row = 0; row < 4; row++) {
      for (int col = 0; col < 3; col++) {
        int x1 = kx[col];
        int y1 = ky[row];
        int x2 = x1 + 90;
        int y2 = y1 + 40;
        if (x > x1 && x < x2 && y > y1 && y < y2) {
          // Draw frame around the pressed key
          drawFrame(x1, y1, x2, y2);
          handleKeypadData(keys[row * 3 + col]);
          return;
        }
      }
    }
}
//Live data screen handling
void handleLiveDataScreen(){
 
  drawFloatLive(TEMP_1.readCelsius(), 300, 75, 370, 100);
  drawFloatLive(TEMP_2.readCelsius(), 300, 115, 370, 140);
  drawFloatLive(TEMP_3.readCelsius(), 300, 155, 370, 180);
  drawLiveDate(0, 460, 205, 480);
  drawLiveTime(205, 460, 410, 480);
  static bool initialized = false;
  if (!initialized) {
    drawLiveDataScreen();  
    initialized = true;
  }
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
      
    // if Back button pressed
    if ((x >= 0) && (x <= 140) && (y >= 0) && (y <= 50)) {
      drawFrame(10, 10, 60, 36);
      Run_cmd = previousRunCmd;     // Go back to the screen we came from
      myGLCD.clrScr();
      initialized = false;
    }
    
    // Add conditions for buttons here
  }

} 