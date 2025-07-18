#ifndef GUI_FSM_H
#define GUI_FSM_H

void runGUI(); /// Checks the updated Run_cmd value
void handleMainScreen();
void handleShutDownScreen();

void handleModeScreen(); 
//Modes main screens touch handling 
void handleAutoScheduleScreen();
void handleAutoProcessScreen();
void handleManualScreen();
void handleProgrammingScreen();
void handleServiceInterfaceScreen();

//Auto-Schedule Screens touch functions
void handleAS_PasteurizationScreen();
void handleAS_CheeseMakingScreen();
void handleAS_CoolingScreen();
void handleAS_CheeseDrainageScreen();
void handleAS_WaterDrainageScreen();
void handleAS_WaterRefillScreen();
void handleAS_MilkRefillScreen();

//Auto-Process Screens touch functions
void handleAP_PasteurizationScreen();
void handleAP_CheeseMakingScreen();
void handleAP_CoolingScreen();
void handleAP_CheeseDrainageScreen();
void handleAP_WaterDrainageScreen();
void handleAP_WaterRefillScreen();
void handleAP_MilkRefillScreen();

//Manual Screens touch functions
void handleMan_PasteurizationScreen();
void handleMan_CheeseMakingScreen();
void handleMan_CoolingScreen();
void handleMan_CheeseDrainageScreen();
void handleMan_WaterDrainageScreen();
void handleMan_WaterRefillScreen();
void handleMan_MilkRefillScreen();

//Programming Screens touch functions
void handlePRG_PasteurizationScreen();
void handlePRG_CheeseMakingScreen();
void handlePRG_CoolingScreen();

//Service interface Screens touch functions
void handleSI_RunTestScreen();
void handleSI_ViewLogsScreen();

//---SI --> Run test
void handleSI_RunDiagnosticsScreen();
void handleSI_RunMonitorSensorsScreen();
//---SI --> Run test --> Diagnostic
void handleDiagnostics_TempScreen();
void handleDiagnostics_RTCScreen();
void handleDiagnostics_RelaysScreen();
void handleDiagnostics_AgitatorScreen();
void handleDiagnostics_ValvesScreen();
void handleDiagnostics_SDcardScreen();

void handleSD_ReadScreen();

//---SI --> View Logs screen --> Log Viewing screen
void handleViewLogs_ViewScreen();

// Run Screens
void handleRun_PasteurizationScreen();
void handleRun_CheeseMakingScreen();
void handleRun_CoolingScreen();
void handleRun_CheeseDrainageScreen();
void handleRun_WaterDrainageScreen();
void handleRun_WaterRefillScreen();
void handleRun_MilkRefillScreen();

void handleKeypadData(const char* key);
void handleKeypadPress(int x, int y);
void handleLiveDataScreen();

#endif