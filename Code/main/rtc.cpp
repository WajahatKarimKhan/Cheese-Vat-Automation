/**
 * @file rtc.cpp
 * @brief Real-Time Clock (RTC) initialization and weekday utility function.
 *
 * This file initializes the RTC module (uRTCLib) with default date/time values
 * and provides a function to get the current weekday as a string.
 */

#include "Arduino.h"
#include "uRTCLib.h"
#include "constant.h"
#include "UARTCom.h"
#include "rtc.h"

uRTCLib rtc(0x68);

//Array of weekday names for indexing by RTC day-of-week value.
char weekDay[7][12] = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" };

/**
 * @brief Initializes the RTC module and sets the default date/time.
 *
 * - Starts the I2C interface (`URTCLIB_WIRE.begin()`).
 * - Sets RTC values defined in `constant.h`.
 * - Initializes serial communication.
 */
void initRTC() {
  initSerial();
  URTCLIB_WIRE.begin();
  //rtc.set(_SET_SEC,_SET_MIN,_SET_HOUR, _SET_WEEK_DAY, _SET_DAY, _SET_MONTH, _SET_YEAR);
}

/**
 * @brief Returns the current day of the week as a string.
*/
const char* currentDOW(){ return weekDay[rtc.dayOfWeek() - 1]; }

bool rtc_working = false;
// check for getting bool 
bool checkRTC(){
  rtc.refresh();
  if (rtc.hour()> 24 || rtc.minute()> 59 ){
      rtc_working = false;
    } else {
      rtc_working = true; 
    }
  return rtc_working;
}

bool isRTCConnected(){ return rtc_working ; }



