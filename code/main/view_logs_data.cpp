#include "view_logs_data.h"
#include "UARTCom.h"
#include "constant.h"
#include "command.h"
#include "sys_init.h"
#include "hardwareAPI.h"
#include <max6675.h>
#include "log.h"
#include <UTFT.h>
#include <UTouch.h>
#include "gui.h"
#include "global_var.h"
#include "gui_fsm.h"

Logs filter= {"00/00/0000", -1, ""};    //date, index of item selected , 
// Global touch and display objects
extern UTFT myGLCD;
extern UTouch myTouch;

char currentInputLogs[12] = "";
uint8_t inputPosLogs= 0;
InputFieldLogs activeFieldLogs = NONE_LOGS;

Logs* activeLogs = nullptr;

void resetInputBufferLogs() {
  memset(currentInputLogs, 0, sizeof(currentInputLogs));
  inputPosLogs = 0;
}

void updateDisplayFieldsLogs() {
  myGLCD.setFont(BigFont);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  drawBoxCenteredText(activeLogs->date, 200, 75, 440, 105);
  
}

void handleKeypadDataLogs(const char* key) {
  if (strcmp(key, "DEL") == 0) {
    if (inputPosLogs > 0) {
      // Don't delete format characters like '/' or ':'
      if (activeFieldLogs == DATE && (inputPosLogs == 3 || inputPosLogs == 6)) {
        inputPosLogs--;  // skip formatting character
      }
      currentInput[--inputPosLogs] = '\0';
    }
  } 
 
  else if (strcmp(key, "OK") == 0) { 
  
  if (activeLogs == nullptr) return;

  if (activeFieldLogs == DATE_LOGS) {
    strncpy(activeLogs->date, currentInputLogs, sizeof(activeLogs->date));
  } 
  
  print("Logs Date: ");
  println(activeLogs->date);
  //resetInputBufferLogs();
  }

  else {
    if (activeFieldLogs == DATE_LOGS && inputPosLogs < 10) {
      if (inputPosLogs == 2 || inputPosLogs == 5) {
        currentInputLogs[inputPosLogs++] = '/';
        currentInputLogs[inputPosLogs] = '\0';
      } // add logic for new inputs here
    } 

    // Add digit only if within limit
    if (activeFieldLogs == DATE_LOGS && inputPosLogs < 10 ) {
      strcat(currentInputLogs, key);
      inputPosLogs++;
    }
  }

  // Live update preview
  if (activeFieldLogs == DATE_LOGS) {
    myGLCD.setFont(BigFont);
    myGLCD.setColor(60, 60, 60);  // Background color
    myGLCD.fillRect(250, 82, 420, 100);  // Adjust width as needed to cover full text area
    myGLCD.setColor(255, 255, 255);  // Text color
    myGLCD.print(currentInputLogs, 250, 82);
    
  } 
}

void checkTouchInputLogs() {
  if (*currentScreenMode == MODE_LOGS && activeLogs != nullptr) {
  if (myTouch.dataAvailable()) {
    myTouch.read();
    int x = myTouch.getX();
    int y = myTouch.getY();
    //while (myTouch.dataAvailable());
    if (x > 200 && x < 440 && y > 75 && y < 105) {
      activeFieldLogs = DATE_LOGS;
      resetInputBufferLogs();
    } 
    else {
      handleKeypadPress( x, y);
      }
    }
  }
}

const char* dropdownItems[] = {"Temperature", "Errors", "Processes"};
const int numItems = sizeof(dropdownItems) / sizeof(dropdownItems[0]);
//Drop down logics
int dropdownX = 200;
int dropdownY = 115;
int dropdownWidth = 230;
int dropdownHeight = 35;
int selectedItemIndex = 0;

bool dropdownExpanded = false;

void checkDropdownTouch() {
  if (myTouch.dataAvailable()) {
    myTouch.read();
    int tx = myTouch.getX();
    int ty = myTouch.getY();

    // Touch inside the type selection box
    if (tx > dropdownX && tx < dropdownX + dropdownWidth &&
        ty > dropdownY && ty < dropdownY + dropdownHeight) {
      //dropdownExpanded = false;  // dropdown go up
      dropdownExpanded = !dropdownExpanded;
      drawDropdown();
    }

    // If expanded, check item selection
    else if (dropdownExpanded) {
      for (int i = 0; i < numItems; i++) {
        int y1 = dropdownY + dropdownHeight * (i + 1);
        int y2 = y1 + dropdownHeight;

        if (tx > dropdownX && tx < dropdownX + dropdownWidth &&
            ty > y1 && ty < y2) {
          selectedItemIndex = i;
          dropdownExpanded = false;
          drawDropdown();
          return;
        }
      }
    }
  }
}
void drawDropdown() {
  // text along the box
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Select Type:", 10, 120);
  // --- save Data Button --- 
  myGLCD.setFont(BigFont);
  myGLCD.setColor(255, 102, 102);  // red
  myGLCD.fillRoundRect(180, 360, 280, 410);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 102, 102); // Text background also red
  myGLCD.drawRoundRect(180, 360, 280, 410);
  drawBoxCenteredText("View", 180, 360, 280, 410);

  // Draw the main box
  myGLCD.setColor(VGA_BLUE);
  myGLCD.fillRect(dropdownX, dropdownY, dropdownX + dropdownWidth, dropdownY + dropdownHeight);
  myGLCD.setColor(VGA_WHITE);
  myGLCD.setBackColor(VGA_BLUE);
  myGLCD.print(dropdownItems[selectedItemIndex], dropdownX + 5, dropdownY + 7);
  myGLCD.drawRect(dropdownX, dropdownY, dropdownX + dropdownWidth, dropdownY + dropdownHeight);

  if (dropdownExpanded) {
    for (int i = 0; i < numItems; i++) {
      int y1 = dropdownY + dropdownHeight * (i + 1);
      int y2 = y1 + dropdownHeight;

      myGLCD.setColor(i == selectedItemIndex ? VGA_GRAY : VGA_NAVY);
      myGLCD.fillRect(dropdownX, y1, dropdownX + dropdownWidth, y2);
      myGLCD.setColor(VGA_WHITE);
      myGLCD.setBackColor(i == selectedItemIndex ? VGA_GRAY : VGA_NAVY);
      myGLCD.print(dropdownItems[i], dropdownX + 5, y1 + 7);
      myGLCD.drawRect(dropdownX, y1, dropdownX + dropdownWidth, y2);
    }
  }
}

//Log size
Logs logsArray[10];  // saved logs size
int logsCount = 0;
void filteredLogsByDateAndType() {
  myGLCD.setFont(BigFont);
  int y = 70;
  bool found = false;
  /// the data acquisition and filtering from the SD card will be done later
  for (int i = 0; i < logsCount; i++) {
    if (strcmp(logsArray[i].date, filter.date) == 0 &&
        logsArray[i].typeIndex == selectedItemIndex) {

      // Show the log message
      myGLCD.setColor(VGA_WHITE);
      myGLCD.print(logsArray[i].message, 10, y);
      y += 15;
      found = true;
    }
  }

  if (!found) {
    myGLCD.setColor(VGA_RED);
    myGLCD.print("No logs found", 10, y);
  }
}

//=========== Temperature 1(Milk) Live graph ===========
unsigned long lastUpdate = 0;

float tempData[MAX_POINTS];  // Stores last 50 readings

int dataIndex = 0;

int mapTimeToX(int i) {
  // i ranges from 0 to 49
  return 30 + (i * 730 / (MAX_POINTS - 1));  // 20px margin on left
}

int mapTempToY(float temp) {
  temp = constrain(temp, 0, 110);
  return 450 - ((temp * 400) / 110);  // Bottom margin = 20px
}

float currentTemp = 0.0;
void update_Temp1_Graph() {
  static unsigned long lastUpdate = 0;
  const unsigned long updateInterval = 1000;  // 1 second

  if (millis() - lastUpdate < updateInterval) {return;}
  lastUpdate = millis();
  // Read and store new temperature in  buffer
  currentTemp = readTEMP(TEMP_SENSOR_1);
  tempData[dataIndex] = currentTemp;
  dataIndex = (dataIndex + 1) % MAX_POINTS;
  //println("updated time and temp");
}  

void live_Temp1_Graph() {
  // Clear only the graph plot area (not full screen)
  myGLCD.setColor(60, 60, 60);  // Match background color
  myGLCD.fillRect(31, 51, 759, 449);
  // Plot updated temperature line
  myGLCD.setColor(VGA_YELLOW);
  for (int i = 1; i < MAX_POINTS; i++) {
    int x1 = mapTimeToX(i - 1);
    int y1 = mapTempToY(tempData[(dataIndex + i - 1) % MAX_POINTS]);
    int x2 = mapTimeToX(i);
    int y2 = mapTempToY(tempData[(dataIndex + i) % MAX_POINTS]);
    myGLCD.drawLine(x1, y1, x2, y2);
    //drawFloatLive(TEMP_1.readCelsius(), x1-90, y1-20, x1-10, y1-4);
  }
  Serial.print("Temp = ");
  Serial.println(currentTemp);
  //Serial.print(" at index ");
  //Serial.println(dataIndex);
}




