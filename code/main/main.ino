/**
*@file main.ino
*@brief sketch for control of the Cheese Vat Automation
*This program reads the data of multiple sensors and analyze different 
*parameters to control the cheese making process
*
*
*@author
*@date
*@version 1.0
*
*/

#include <Arduino.h>
#include "global_var.h"
#include "temp_sensors.h"
#include "log.h"
#include "constant.h"
#include "UARTCom.h"
#include "hardwareAPI.h"
#include "sys_init.h"
#include "uRTCLib.h"
#include "UART_service_interface.h"
#include "sd_card.h"
#include <UTFT.h>
#include <UTouch.h>
#include "utils.h"
#include "command.h"
#include "gui.h"
#include "gui_fsm.h"
#include "schedule_data.h"
#include "prg_data.h"
#include "manual_data.h"
#include "view_logs_data.h"
#include "process_execution.h"

extern int x, y ;
uint8_t Run_cmd = 0x00;



/**
*@brief Initialization of system at boot time
*
*/
void setup() {
  // Initial setup
  initSerial();
  initRTC();
  initGUI();
  checkSDcard();
  initValvesandRelays();

}
/**
*@brief Main program loop
*It runs in a loop continuously and the operation defined in it runs continuously
*
*@note delay1s function gives the system a 1 second pause after printing
*/
void loop() {
  
  SerialServiceInterface();
  
  logToSD(readTEMP(TEMP_SENSOR_1), readTEMP(TEMP_SENSOR_2), readTEMP(TEMP_SENSOR_3), getCurrentProcessStatus() );
  
  //delay1s();
  runGUI();
  checkTouchInputSchedule();
  checkTouchInputPRG();
  checkTouchInputMan();
  checkTouchInputLogs();
  StopProcess();
  //update_Temp1_Graph();
  if (isPasteurizing)  { fsm_Pasteurization(); }
  if (isCheeseMaking)  { fsm_CheeseMaking();   }
  if (isMilkRefilling) { exe_MilkRefill(mode, &m, &p); }
  if (isWaterRefilling){ exe_WaterRefill(); }
  if (isWaterDrainage) { exe_WaterDrainage(); }
}



