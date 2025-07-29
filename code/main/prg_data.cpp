#include "prg_data.h"
#include "UARTCom.h"
#include "constant.h"
#include <UTFT.h>
#include <UTouch.h>
#include "gui.h"
#include "global_var.h"
#include "gui_fsm.h"

  // { process name, target tempertaure, temp regulation, hold timer, speed heat, heat CW/CCW, speed hold, hold CW/CCW, target cooling }
Program pasteurizationProgram = {  "Pasteurization","00", true, "00", "0000", true, "0000", false, "00"}; 
Program cheeseMakingProgram   = { "Cheese Making","00", true, "00", "0000", true, "0000", false, "00"};
Program coolingProgram        = { "Cooling","00", false, "00", "0000", true, "0000", false, "00"}; // for cooling user can only define the target temperature

// Global touch and display objects
extern UTFT myGLCD;
extern UTouch myTouch;

// Generic pointer for active programming screen
Program* activeProgram = nullptr;

// Keep track of current input
char currentInputPRG[12] = "";
uint8_t inputPosPRG = 0;
InputFieldPRG activeFieldPRG = NONE_PRG;

void resetInputBufferPRG() {
  memset(currentInputPRG, 0, sizeof(currentInputPRG));
  inputPosPRG = 0;
}

void updateDisplayFieldsPRG() {
  myGLCD.setFont(BigFont);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);

  if (activeProgram != nullptr) {
  drawBoxCenteredText(activeProgram->targetTemp, 300, 75, 390, 100);
  drawBoxCenteredText(activeProgram->holdTimeMinutes, 300, 155, 380, 180);
  drawBoxCenteredText(activeProgram->stirSpeedHeating, 300, 235, 380, 260);
  drawBoxCenteredText(activeProgram->stirSpeedHolding, 310, 315, 380, 340);
  drawBoxCenteredText(activeProgram->targetCoolTemp, 300, 355, 390, 380);
  }
}

void handleKeypadDataPRG(const char* key) {
  if (strcmp(key, "DEL") == 0) {
    if (inputPosPRG > 0) {
      currentInputPRG[--inputPosPRG] = '\0';
    }
  }

  else if (strcmp(key, "OK") == 0) {
    //drawFrame(670, 270, 760, 310);
    if (activeProgram == nullptr) return;

    if (activeFieldPRG == TARGET_TEMP) {
      //activeProgram->targetTemp = atof(currentInputPRG);  // float
      strncpy(activeProgram->targetTemp, currentInputPRG, sizeof(activeProgram->targetTemp));
    } else if (activeFieldPRG == HOLD_TIME) {
      strncpy(activeProgram->holdTimeMinutes, currentInputPRG, sizeof(activeProgram->holdTimeMinutes));
    } else if (activeFieldPRG == STIR_SPEED_HEATING) {
      strncpy(activeProgram->stirSpeedHeating, currentInputPRG, sizeof(activeProgram->stirSpeedHeating));
    } else if (activeFieldPRG == STIR_SPEED_HOLDING) {
      strncpy(activeProgram->stirSpeedHolding, currentInputPRG, sizeof(activeProgram->stirSpeedHolding));
    } else if (activeFieldPRG == TARGET_COOL_TEMP) {
      strncpy(activeProgram->targetCoolTemp, currentInputPRG, sizeof(activeProgram->targetCoolTemp));
    }
    
    //resetInputBufferPRG();
  }

  else{
    if ((activeFieldPRG == TARGET_TEMP && inputPosPRG < 2) || (activeFieldPRG == HOLD_TIME && inputPosPRG < 2) ||
      (activeFieldPRG == STIR_SPEED_HEATING && inputPosPRG < 4) || (activeFieldPRG == STIR_SPEED_HOLDING && inputPosPRG < 4) || (activeFieldPRG == TARGET_COOL_TEMP && inputPosPRG < 2) ) {
      strcat(currentInputPRG, key);
      inputPosPRG++;
    }
  }
  // Live update rendering
  myGLCD.setColor(60, 60, 60);  // Background
  myGLCD.setFont(BigFont);
  if (activeFieldPRG == TARGET_TEMP) {
    myGLCD.fillRect(305, 80, 385, 96);
    myGLCD.setColor(255, 255, 255);
    myGLCD.print(currentInputPRG, 305, 80);
  } else if (activeFieldPRG == HOLD_TIME) {
    myGLCD.fillRect(324, 160, 356, 176);
    myGLCD.setColor(255, 255, 255);
    myGLCD.print(currentInputPRG, 324, 160);
  } else if (activeFieldPRG == STIR_SPEED_HEATING) {
    myGLCD.fillRect(308, 240, 372, 256);
    myGLCD.setColor(255, 255, 255);
    myGLCD.print(currentInputPRG, 308, 240);
  } else if (activeFieldPRG == STIR_SPEED_HOLDING) {
    myGLCD.fillRect(318, 320, 372, 336);
    myGLCD.setColor(255, 255, 255);
    myGLCD.print(currentInputPRG, 318, 320);
  } else if (activeFieldPRG == TARGET_COOL_TEMP) {
    myGLCD.fillRect(305, 360, 385, 376);
    myGLCD.setColor(255, 255, 255);
    myGLCD.print(currentInputPRG, 305, 360);
  }
}

void checkTouchInputPRG() {
  if (*currentScreenMode == MODE_PROGRAMMING && activeProgram != nullptr) {
  if (myTouch.dataAvailable()) {
    myTouch.read();
    int x = myTouch.getX();
    int y = myTouch.getY();
    
    //while (myTouch.dataAvailable());
    if (x > 300 && x < 390 && y > 75 && y < 105) {
      activeFieldPRG = TARGET_TEMP;
      resetInputBufferPRG();
      myGLCD.setColor(0, 255, 0);
      myGLCD.drawRoundRect(300, 75, 390, 100);
    } else if (x > 300 && x < 380 && y > 155 && y < 185) {
      activeFieldPRG = HOLD_TIME;
      resetInputBufferPRG();
      myGLCD.setColor(0, 255, 0);
      myGLCD.drawRoundRect(300, 155, 380, 180);
    } else if (x > 300 && x < 380 && y > 235 && y < 265) {
      activeFieldPRG = STIR_SPEED_HEATING;
      resetInputBufferPRG();
      myGLCD.setColor(0, 255, 0);
      myGLCD.drawRoundRect(300, 235, 380, 260);
    } else if (x > 310 && x < 380 && y > 315 && y < 345) {
      activeFieldPRG = STIR_SPEED_HOLDING;
      resetInputBufferPRG();
      myGLCD.setColor(0, 255, 0);
      myGLCD.drawRoundRect(310, 315, 390, 340);
    } // Toggle buttons
    else if (x > 300 && x < 400 && y > 115 && y < 145) {
      activeProgram->tempRegulation = !activeProgram->tempRegulation;
      drawToggleButton(300, 115, activeProgram->tempRegulation, "ON", "OFF");
    }
    else if (x > 300 && x < 400 && y > 195 && y < 225) {
      activeProgram->stirDirHeating = !activeProgram->stirDirHeating;
      drawToggleButton(300, 195, activeProgram->stirDirHeating, "CW", "CCW");
    }
    else if (x > 300 && x < 400 && y > 275 && y < 305) {
      activeProgram->stirDirHolding = !activeProgram->stirDirHolding;
      drawToggleButton(300, 275, activeProgram->stirDirHolding, "CW", "CCW");
    } 
    else if (x > 300 && x < 390 && y > 355 && y < 385) {
      activeFieldPRG = TARGET_COOL_TEMP;
      resetInputBufferPRG();
      myGLCD.setColor(0, 255, 0);
      myGLCD.drawRoundRect(300, 355, 390, 380);
    }
    // Check for Save button press
    else if (x > 180 && x < 280 && y > 410 && y < 460) {
  
  activeFieldPRG = NONE_PRG;
  drawFrame(180, 410, 280, 460);
  print("Programming Process: ");
  println(activeProgram->prg_name);
  print("Target Heating temperature: ");
  println(activeProgram->targetTemp);
  print("Holding Time: (minutes)");
  println(activeProgram->holdTimeMinutes);
  print("Heating stir speed: ");
  println(activeProgram->stirSpeedHeating);
  print("Holding stir speed: ");
  println(activeProgram->stirSpeedHolding);
  print("Temp Regulation: ");
  println(activeProgram->tempRegulation ? "ON" : "OFF");
  print("Stir Direction (Heating): ");
  println(activeProgram->stirDirHeating ? "CW" : "CCW");
  print("Stir Direction (Holding): ");
  println(activeProgram->stirDirHolding ? "CW" : "CCW");
  print("Target Cooling temperature: ");
  println(activeProgram->targetCoolTemp);
  }

  else{
    handleKeypadPress( x, y);
      }
    }
  }
}






