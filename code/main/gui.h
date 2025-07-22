#ifndef GUI_H
#define GUI_H

#include <UTFT.h>
#include <UTouch.h>

void drawMainScreen();
//void drawShutDownScreen();
void drawModeScreen();

void drawAutoScheduleScreen();
void drawAutoProcessScreen();
void drawManualScreen();
void drawProgrammingScreen();
void drawServiceInterfaceScreen();

void drawLiveDataScreen();
void drawFloatLive(float value, int x1, int y1, int x2, int y2);

void drawAS_GenericScreen();

void drawAS_PasteurizationScreen();
void drawAS_CheeseMakingScreen();
void drawAS_CoolingScreen();
void drawAS_CheeseDrainageScreen();
void drawAS_WaterDrainageScreen();
void drawAS_WaterRefillScreen();
void drawAS_MilkRefillScreen();

void drawAP_PasteurizationScreen();
void drawAP_CheeseMakingScreen();
void drawAP_CoolingScreen();
void drawAP_CheeseDrainageScreen();
void drawAP_WaterDrainageScreen();
void drawAP_WaterRefillScreen();
void drawAP_MilkRefillScreen();

void drawMan_PasteurizationScreen();
void drawMan_CheeseMakingScreen();
void drawMan_CoolingScreen();
void drawMan_CheeseDrainageScreen();
void drawMan_WaterDrainageScreen();
void drawMan_WaterRefillScreen();
void drawMan_MilkRefillScreen();

void drawPRG_PasteurizationScreen();
void drawPRG_CheeseMakingScreen();
void drawPRG_CoolingScreen();

void drawSI_RunTestScreen();
void drawSI_ViewLogsScreen();
//---SI --> Run test
void drawSI_RunDiagnosticsScreen();
void drawSI_RunMonitorSensorsScreen();
//---SI --> Run test --> Diagnostic
void drawDiagnostics_TempScreen();
void drawDiagnostics_RTCScreen();
void drawDiagnostics_RelaysScreen();
void drawDiagnostics_AgitatorScreen();
void drawDiagnostics_ValvesScreen();
void drawDiagnostics_SDcardScreen();
//---SI --> View Logs screen --> Log Viewing screen
void drawViewLogs_ViewScreen();

// Run Screens
void drawRun_PasteurizationScreen();
void drawRun_CheeseMakingScreen();
void drawRun_CoolingScreen();
void drawRun_CheeseDrainageScreen();
void drawRun_WaterDrainageScreen();
void drawRun_WaterRefillScreen();
void drawRun_MilkRefillScreen();

void drawKeypadSec();
void drawFrame(int x1, int y1, int x2, int y2);
void drawBackButton();
void drawRunButton(int x1, int y1, int x2, int y2);
void drawPauseButton();
void drawResumeButton();
void drawStopButton(int x1, int y1, int x2, int y2);
void drawToggleButton(int x, int y, bool state, const char* trueLabel, const char* falseLabel);
// Center the text w.r.t x-y coordinates of box
void drawBoxCenteredText(const char* label, int x1, int y1, int x2, int y2);
void drawBoxCenteredText(float value, int x1, int y1, int x2, int y);
void drawLiveDate(int x1, int y1, int x2, int y2);
void drawLiveTime(int x1, int y1, int x2, int y2);

void drawCSV();
void draw_Temp1_Graph();

// vertical level bar : x, y , width ,height , current percentage(from the level sensor)
void drawLevelBar(int x, int y, int width, int height, int percentage, unsigned int color);
void drawHorizontalBar(int x, int y, int width, int height, int percentage, unsigned int color);
void drawHoldElapsedTime(unsigned long holdStartTime, int x, int y);
//void drawDropdown();
void drawLogo();


//==== Defining Variables
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];

extern UTFT myGLCD;
extern UTouch myTouch;


#endif