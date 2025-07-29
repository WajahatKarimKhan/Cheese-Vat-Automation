#ifndef SCHEDULE_DATA_H
#define SCHEDULE_DATA_H

#include <Arduino.h>

struct Schedule {
  const char* name; 
  char date[9]; // Format: DD/MM/YYYY + null
  char time[6];  // Format: HH:MM + null
  uint8_t repeat; // 0 = Once, 1 = Daily, 2 = Weekly
  bool triggered; // Default is false (not triggered yet)
};

void checkTouchInputSchedule();
void updateDisplayFieldsSchedule();
void handleKeypadDataSchedule(const char* key);

bool checkSchedule(Schedule* sch);
extern bool hasRunToday;

extern Schedule pasteurizationSchedule;
extern Schedule cheeseMakingSchedule;
extern Schedule coolingSchedule ;
extern Schedule waterDrainageSchedule;
extern Schedule cheeseDrainageSchedule;
extern Schedule waterRefillSchedule;
extern Schedule milkRefillSchedule;

extern Schedule* activeSchedule;

#endif