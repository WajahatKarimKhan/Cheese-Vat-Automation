#ifndef VIEW_LOGS_H
#define VIEW_LOGS_H

#include "constant.h"

struct Logs {
  //const char* name; 
  char date[11]; // Format: DD/MM/YYYY + null
  int typeIndex;  // Index of dropdownItems[] (e.g., 0 = Temperature)
  char message[100];
};

void checkTouchInputLogs();
void updateDisplayFieldsLogs();
void handleKeypadDataLogs(const char* key);

extern Logs filter;

extern Logs* activeLogs;

//const char* dropdownItems[] = {"Temperature", "Errors", "Processes"};
//const int numItems = sizeof(dropdownItems) / sizeof(dropdownItems[0]);
extern bool dropdownExpanded;
extern const char* dropdownItems[];
extern const int numItems;
//later will be added to constants
extern int dropdownX;
extern int dropdownY;
extern int dropdownWidth;
extern int dropdownHeight;
extern int selectedItemIndex;

extern bool dropdownExpanded;
void checkDropdownTouch();
void drawDropdown();

void filteredLogsByDateAndType();

void update_Temp1_Graph();
void live_Temp1_Graph();
extern int mapTimeToX(int i);
extern int mapTempToY(float temp);
extern float tempData[MAX_POINTS];  // Stores last 50 readings

extern int dataIndex;


#endif