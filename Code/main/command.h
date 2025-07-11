/**
 * @file command.h
 * @brief Contains command definitions for UART-based service interface.
 * @details
 * Each command is associated with a unique hexadecimal code used to trigger
 * specific functions in the FSMs.
 */

#ifndef COMMAND_H
#define COMMAND_H

//// =============== GUI Portion ==============

//==== Main screen ====
#define GUI_MAIN_SCREEN               0x00 

#define BTN_STARTUP_PRESSED           0x01  
#define GUI_SHUTDOWN_SCREEN           0x02

//==== Mode screen ====
#define GUI_MODE_SCREEN               0x03

#define BTN_AUTO_SCHEDULE_PRESSED     0x10
#define BTN_AUTO_PROCESS_PRESSED      0x20
#define BTN_MANUAL_PRESSED            0x30
#define BTN_PROGRAMMING_PRESSED       0x40
#define BTN_SERVICE_INTERFACE_PRESSED 0x50

//==== Auto Schedule screen ====
#define GUI_AUTO_SCHEDULE_SCREEN      0x11
#define GUI_AS_PASTEURIZATION_SCREEN  0X12
#define GUI_AS_CHEESE_MAKING_SCREEN   0X13
#define GUI_AS_COOLING_SCREEN         0X14
#define GUI_AS_CHEESE_DRAINAGE_SCREEN 0X15
#define GUI_AS_WATER_DRAINAGE_SCREEN  0X16
#define GUI_AS_WATER_REFILL_SCREEN    0X17
#define GUI_AS_MILK_REFILL_SCREEN     0X18

#define GUI_LIVE_DATA_SCREEN          0x19

//==== Auto Process screen ====
#define GUI_AUTO_PROCESS_SCREEN       0x21
#define GUI_AP_PASTEURIZATION_SCREEN  0X22
#define GUI_AP_CHEESE_MAKING_SCREEN   0X23
#define GUI_AP_COOLING_SCREEN         0X24
#define GUI_AP_CHEESE_DRAINAGE_SCREEN 0X25
#define GUI_AP_WATER_DRAINAGE_SCREEN  0X26
#define GUI_AP_WATER_REFILL_SCREEN    0X27
#define GUI_AP_MILK_REFILL_SCREEN     0X28

//==== Programming screen ====
#define GUI_PROGRAMMING_SCREEN         0x41
#define GUI_PRG_PASTEURIZATION_SCREEN  0X42
#define GUI_PRG_CHEESE_MAKING_SCREEN   0X43
#define GUI_PRG_COOLING_SCREEN         0X44

//==== Manual screen ====
#define GUI_MANUAL_SCREEN              0x31
#define GUI_MAN_PASTEURIZATION_SCREEN  0X32
#define GUI_MAN_CHEESE_MAKING_SCREEN   0X33
#define GUI_MAN_COOLING_SCREEN         0X34
#define GUI_MAN_CHEESE_DRAINAGE_SCREEN 0X35
#define GUI_MAN_WATER_DRAINAGE_SCREEN  0X36
#define GUI_MAN_WATER_REFILL_SCREEN    0X37
#define GUI_MAN_MILK_REFILL_SCREEN     0X38

//==== Service Interface screen ====
#define GUI_SERVICE_INTERFACE_SCREEN      0x51
#define GUI_SI_RUN_TEST_SCREEN            0x52
#define GUI_SI_VIEW_LOGS_SCREEN           0x53
#define GUI_SI_RUN_DIAGNOSTICS_SCREEN     0x54
#define GUI_SI_RUN_MONITOR_SENSORS_SCREEN 0x55
///--
#define GUI_DIAGNOSTICS_TEMP_SCREEN       0x5A
#define GUI_DIAGNOSTICS_RTC_SCREEN        0x5B
#define GUI_DIAGNOSTICS_RELAYS_SCREEN     0x5C
#define GUI_DIAGNOSTICS_AGITATOR_SCREEN   0x5D
#define GUI_DIAGNOSTICS_VALVES_SCREEN     0x5E
#define GUI_DIAGNOSTICS_SDCARD_SCREEN     0x5F

#define GUI_SD_READ_SCREEN                0x56

//-logs screen -> view button pressed
#define GUI_BTN_VIEW_PRESSED              0x59
//===== Run Screens ======
#define GUI_RUN_PASTEURIZATION_SCREEN   0x60
#define GUI_RUN_CHEESE_MAKING_SCREEN    0x61
#define GUI_RUN_COOLING_SCREEN          0x62
#define GUI_RUN_CHEESE_DRAINAGE_SCREEN  0x63
#define GUI_RUN_WATER_DRAINAGE_SCREEN   0x64
#define GUI_RUN_WATER_REFILL_SCREEN     0x65
#define GUI_RUN_MILK_REFILL_SCREEN      0x66

//// =============== Main control Logic portion ==============
#define CMD_SI_MAIN         0x70  /// Command code for returning to main UART service interface menu

#define CMD_LOW_LEVEL       0x71  /// Command code for returning to Low level menu
#define CMD_MID_LEVEL       0x72  /// Command code for returning to Mid level menu
#define CMD_HIGH_LEVEL      0x73  /// Command code for returning to High level menu
#define CMD_USER_LEVEL      0x74  /// Command code for returning to User level menu

#define CMD_TEMP_MENU       0x77  /// Command code for entering Temperature service menu.
#define CMD_TEMP_SENSOR_1   0x76  /// Command code for selecting Temperature Sensor 1.
#define CMD_TEMP_SENSOR_2   0x77  /// Command code for selecting Temperature Sensor 2.
#define CMD_TEMP_SENSOR_3   0x78  /// Command code for selecting Temperature Sensor 3.

#define CMD_TEST_RTC        0x79 
#define CMD_SET_RTC         0x7A 

#define CMD_TEST_SD         0x7B 

#endif