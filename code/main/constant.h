/**
 * @file constant.h
 * @brief Contains global constants and enumerations used across the system.
 *
 * Defines baud rate, standard delays, real-time clock (RTC) initial settings,
 * and temperature sensor identifiers.
 */

#ifndef CONSTANT_H
#define CONSTANT_H


#define _BAUD_RATE 9600   //Baud rate for serial communication.
#define _DELAY_1S 1000    //Delay of 1 second (in milliseconds)

#define _SET_DAY 8     //Default day of the month to set in RTC. 
#define _SET_MONTH 7    //Default month to set in RTC.
#define _SET_YEAR 25    //Default year (2-digit format) to set in RTC.
#define _SET_HOUR 15    //Default hour to set in RTC (24-hour format).
#define _SET_MIN 16     //Default minute to set in RTC.
#define _SET_SEC 35     //Default second to set in RTC.
#define _SET_WEEK_DAY 2  // Default day of week (1=Monday, 7=Sunday)

// -------------- Temperature Sensor Enum ----------------

/**
 * @enum Temp_Sensors
 * @brief Identifiers for each connected temperature sensor.
 */
enum Temp_Sensors{ TEMP_SENSOR_1, TEMP_SENSOR_2, TEMP_SENSOR_3 };

// ------- Process Execution Configuration enum ---------
enum Execute_Mode { EXE_NONE, EXE_AUTO_SCHEDULE, EXE_AUTO_PROCESS, EXE_MANUAL };
// ------- Process's Phase Execution Configuration enum ---------
enum PasteurizationPhase { PAST_PHASE_IDLE, PAST_PHASE_LEVEL_CHECK, PAST_PHASE_HEATING,
                           PAST_PHASE_HOLDING, PAST_PHASE_COOLING, PAST_PHASE_DONE   };
enum CheeseMakingPhase { CM_PHASE_IDLE, CM_PHASE_LEVEL_CHECK, CM_PHASE_HEATING,
                         CM_PHASE_HOLDING, CM_PHASE_COOLING, CM_PHASE_DONE  };
enum CoolingStage { COOL_STAGE_IDLE, COOL_STAGE_START, COOL_STAGE_DONE };
enum MilkRefillingStage  { MILK_STAGE_IDLE, MILK_STAGE_START, MILK_STAGE_DONE };
enum WaterRefillingStage { WR_STAGE_IDLE, WR_STAGE_START, WR_STAGE_DONE };
enum WaterDrainageStage  { WD_STAGE_IDLE, WD_STAGE_START, WD_STAGE_DONE };

// ----------- Milk/Water level limits ---------------
#define MILK_TANK_HEIGHT_CM     100.0
#define MILK_MIN_THRESHOLD      50     // minimum % required
#define WATER_TANK_HEIGHT_CM    50.0
#define WATER_MIN_THRESHOLD     80    // minimum % required

#define MILK_EMPTY_THRESHOLD    10    // minimum % required
#define WATER_EMPTY_THRESHOLD   10    // minimum % required

//// =================  GUI Section ===================

enum InputFieldSchedule { NONE, DATE, TIME };
enum InputFieldPRG { NONE_PRG, TARGET_TEMP, STIR_SPEED_HEATING, STIR_DIR_HEATING, STIR_SPEED_HOLDING, STIR_DIR_HOLDING, HOLD_TIME, TARGET_COOL_TEMP} ;
enum InputFieldMan { NONE_MAN, TARGET_TEMP_MAN, STIR_SPEED_HEATING_MAN, STIR_DIR_HEATING_MAN, STIR_SPEED_HOLDING_MAN, STIR_DIR_HOLDING_MAN, HOLD_TIME_MAN, MILK_REFILL_MAN, TARGET_COOL_TEMP_MAN} ;
enum ScreenMode { MODE_NONE, MODE_SCHEDULING, MODE_PROGRAMMING, MODE_MANUAL, MODE_LOGS };
enum InputFieldLogs { NONE_LOGS, DATE_LOGS };

enum RepeatType { REPEAT_ONCE, REPEAT_DAILY, REPEAT_WEEKLY };

#endif // CONSTANT_H