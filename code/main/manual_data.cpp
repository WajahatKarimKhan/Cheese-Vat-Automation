#include "manual_data.h"
#include "UARTCom.h"
#include "constant.h"
#include <UTFT.h>
#include <UTouch.h>
#include "gui.h"
#include "global_var.h"
#include "gui_fsm.h"

  // { process name, target tempertaure, temp regulation, hold timer, speed heat, heat CW/CCW, speed hold, hold CW/CCW }
Manual pasteurizationManual = { "Pasteurization","00", true, "00", "0000", true, "0000", false , "000", "00"}; 
Manual cheeseMakingManual   = { "Cheese Making","00", true, "00", "0000", true, "0000", false, "000", "00"};
Manual coolingManual        = { "Cooling","00", false, "00", "0000", true, "0000", false, "000", "00"}; // for cooling user can only define the target temperature // no use - will delete later
Manual milkRefillManual     = { "Mill Refill","00", false, "00", "0000", false, "0000", false, "000", "00"}; // for Milk refilling user can only define the milk refilling in liters
// Global touch and display objects
extern UTFT myGLCD;
extern UTouch myTouch;

// Generic pointer for active Manualming screen
Manual* activeManual = nullptr;

// Keep track of current input
char currentInputMan[12] = "";
uint8_t inputPosMan = 0;
InputFieldMan activeFieldMan = NONE_MAN;

void resetInputBufferMan() {
  memset(currentInputMan, 0, sizeof(currentInputMan));
  inputPosMan = 0;
}

void updateDisplayFieldsMan() {
  myGLCD.setFont(BigFont);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);

  if (activeManual != nullptr) {
  drawBoxCenteredText(activeManual->man_targetTemp, 300, 75, 390, 100);
  drawBoxCenteredText(activeManual->man_holdTimeMinutes, 300, 155, 380, 180);
  drawBoxCenteredText(activeManual->man_stirSpeedHeating, 300, 235, 380, 260);
  drawBoxCenteredText(activeManual->man_stirSpeedHolding, 310, 315, 380, 340);
  drawBoxCenteredText(activeManual->man_milkinltrs, 10, 160, 210, 280);
  drawBoxCenteredText(activeManual->man_targetCoolTemp, 300, 355, 390, 380);
  }
}

void handleKeypadDataMan(const char* key) {
  if (strcmp(key, "DEL") == 0) {
    if (inputPosMan > 0) {
      currentInputMan[--inputPosMan] = '\0';
    }
  }

  else if (strcmp(key, "OK") == 0) {
    if (activeManual == nullptr) return;

    if (activeFieldMan == TARGET_TEMP_MAN) {
      strncpy(activeManual->man_targetTemp, currentInputMan, sizeof(activeManual->man_targetTemp));
    } else if (activeFieldMan == HOLD_TIME_MAN) {
      strncpy(activeManual->man_holdTimeMinutes, currentInputMan, sizeof(activeManual->man_holdTimeMinutes));
    } else if (activeFieldMan == STIR_SPEED_HEATING_MAN) {
      strncpy(activeManual->man_stirSpeedHeating, currentInputMan, sizeof(activeManual->man_stirSpeedHeating));
    } else if (activeFieldMan == STIR_SPEED_HOLDING_MAN) {
      strncpy(activeManual->man_stirSpeedHolding, currentInputMan, sizeof(activeManual->man_stirSpeedHolding));
    } else if (activeFieldMan == MILK_REFILL_MAN) {
      strncpy(activeManual->man_milkinltrs, currentInputMan, sizeof(activeManual->man_milkinltrs));
    } else if (activeFieldMan == TARGET_COOL_TEMP_MAN) {
      strncpy(activeManual->man_targetCoolTemp, currentInputMan, sizeof(activeManual->man_targetCoolTemp));
    }
    
    //resetInputBufferMan();
  }

  else{
    if ((activeFieldMan == TARGET_TEMP_MAN && inputPosMan < 2) || (activeFieldMan == HOLD_TIME_MAN && inputPosMan < 2) ||
      (activeFieldMan == STIR_SPEED_HEATING_MAN && inputPosMan < 4) || (activeFieldMan == STIR_SPEED_HOLDING_MAN && inputPosMan < 4) || 
      (activeFieldMan == MILK_REFILL_MAN && inputPosMan < 3) || (activeFieldMan == TARGET_COOL_TEMP_MAN && inputPosMan < 2)) {
      strcat(currentInputMan, key);
      inputPosMan++;
    }
  }
  // Live update rendering
  myGLCD.setColor(60, 60, 60);  // Background
  myGLCD.setFont(BigFont);
  if (activeFieldMan == TARGET_TEMP_MAN) {
    myGLCD.fillRect(305, 80, 385, 96);
    myGLCD.setColor(255, 255, 255);
    myGLCD.print(currentInputMan, 305, 80);
  } else if (activeFieldMan == HOLD_TIME_MAN) {
    myGLCD.fillRect(324, 160, 356, 176);
    myGLCD.setColor(255, 255, 255);
    myGLCD.print(currentInputMan, 324, 160);
  } else if (activeFieldMan == STIR_SPEED_HEATING_MAN) {
    myGLCD.fillRect(308, 240, 372, 256);
    myGLCD.setColor(255, 255, 255);
    myGLCD.print(currentInputMan, 308, 240);
  } else if (activeFieldMan == STIR_SPEED_HOLDING_MAN) {
    myGLCD.fillRect(318, 320, 372, 336);
    myGLCD.setColor(255, 255, 255);
    myGLCD.print(currentInputMan, 318, 320);
  } else if (activeFieldMan == MILK_REFILL_MAN) {
    //myGLCD.fillRect(10, 160, 210, 280);
    myGLCD.setColor(255, 255, 255);
    myGLCD.print(currentInputMan, 86, 252);
  } else if (activeFieldMan == TARGET_COOL_TEMP_MAN) {
    myGLCD.fillRect(305, 360, 385, 376);
    myGLCD.setColor(255, 255, 255);
    myGLCD.print(currentInputMan, 305, 360);
  }
}

void checkTouchInputMan() {
  if (*currentScreenMode == MODE_MANUAL && activeManual != nullptr) {
  if (myTouch.dataAvailable()) {
    myTouch.read();
    int x = myTouch.getX();
    int y = myTouch.getY();
    
    //while (myTouch.dataAvailable());
    
    if (x > 300 && x < 390 && y > 75 && y < 105) {
      activeFieldMan = TARGET_TEMP_MAN;
      resetInputBufferMan();
      myGLCD.setColor(0, 255, 0);
      myGLCD.drawRoundRect(300, 75, 390, 100);
    } else if (x > 300 && x < 380 && y > 155 && y < 185) {
      activeFieldMan = HOLD_TIME_MAN;
      resetInputBufferMan();
      myGLCD.setColor(0, 255, 0);
      myGLCD.drawRoundRect(300, 155, 380, 180);
    } else if (x > 300 && x < 390 && y > 235 && y < 265) {
      activeFieldMan = STIR_SPEED_HEATING_MAN;
      resetInputBufferMan();
      myGLCD.setColor(0, 255, 0);
      myGLCD.drawRoundRect(300, 235, 390, 260);
    } else if (x > 310 && x < 390 && y > 315 && y < 345) {
      activeFieldMan = STIR_SPEED_HOLDING_MAN;
      resetInputBufferMan();
      myGLCD.setColor(0, 255, 0);
      myGLCD.drawRoundRect(310, 315, 390, 340);
    }  else if (x > 10 && x < 210 && y > 160 && y < 280) {
      activeFieldMan = MILK_REFILL_MAN;
      resetInputBufferMan();
      myGLCD.setColor(0, 255, 0);
      myGLCD.drawRoundRect(10, 160, 210, 280);
    } // Toggle buttons
    else if (x > 300 && x < 400 && y > 115 && y < 145) {
      activeManual->man_tempRegulation = !activeManual->man_tempRegulation;
      drawToggleButton(300, 115, activeManual->man_tempRegulation, "ON", "OFF");
    }
    else if (x > 300 && x < 400 && y > 195 && y < 225) {
      activeManual->man_stirDirHeating = !activeManual->man_stirDirHeating;
      drawToggleButton(300, 195, activeManual->man_stirDirHeating, "CW", "CCW");
    }
    else if (x > 300 && x < 400 && y > 275 && y < 305) {
      activeManual->man_stirDirHolding = !activeManual->man_stirDirHolding;
      drawToggleButton(300, 275, activeManual->man_stirDirHolding, "CW", "CCW");
    }
    else if (x > 300 && x < 390 && y > 355 && y < 385) {
      activeFieldMan = TARGET_COOL_TEMP_MAN;
      resetInputBufferMan();
      myGLCD.setColor(0, 255, 0);
      myGLCD.drawRoundRect(300, 355, 390, 380);
    }
    // Check for Save button press
    else if (x > 180 && x < 280 && y > 410 && y < 460) {
  //resetInputBufferMan();   // clear field after save
  activeFieldMan = NONE_MAN;
  drawFrame(180, 410, 280, 460);
  println("===================================== ");
  print("Manual Process: ");
  println(activeManual->man_name);
  print("Target temperature: ");
  println(activeManual->man_targetTemp);
  print("Holding Time(minutes): ");
  println(activeManual->man_holdTimeMinutes);
  print("Heating stir speed: ");
  println(activeManual->man_stirSpeedHeating);
  print("Holding stir speed: ");
  println(activeManual->man_stirSpeedHolding);
  print("Milk Refill(ltr): ");
  println(activeManual->man_milkinltrs);
  print("Temp Regulation: ");
  println(activeManual->man_tempRegulation ? "ON" : "OFF");
  print("Stir Direction (Heating): ");
  println(activeManual->man_stirDirHeating ? "CW" : "CCW");
  print("Stir Direction (Holding): ");
  println(activeManual->man_stirDirHolding ? "CW" : "CCW");
  print("Target Cooling temperature: ");
  println(activeManual->man_targetCoolTemp);
  }

  else{
    handleKeypadPress( x, y);
      }
    }
  }
}






