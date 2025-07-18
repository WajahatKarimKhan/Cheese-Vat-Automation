#include "gui.h"

#include "sys_init.h"
#include "UARTCom.h"
#include "log.h"
#include "hardwareAPI.h"
#include "uRTCLib.h"
#include "rtc.h"
#include "sd_card.h"
#include "sd_card.h"
#include <SD.h>
//#include <SdFat.h>
#include <SPI.h>


#include <UTFT.h>
#include <UTouch.h>
#include "global_var.h"
#include "command.h"
#include "schedule_data.h"
#include "prg_data.h"
#include "manual_data.h"
#include "view_logs_data.h"
#include "logo.cpp"
#include "cvat.cpp"

  UTFT myGLCD(SSD1963_800ALT, 38, 39, 40, 41);
  UTouch myTouch(6, 5, 4, 3, 2);
  extern const unsigned short logo[];
  extern const unsigned short cvat[];

void initGUI(){
  // Initialize display
  myGLCD.InitLCD();
  myGLCD.clrScr();
  myTouch.InitTouch();
  myTouch.setPrecision(PREC_MEDIUM);
  
  drawMainScreen();  // Draws the Main Screen
  Run_cmd = GUI_MAIN_SCREEN ; // Indicates that we are at Main Screen 

}

void drawMainScreen() {
  // Set full background to grey
  myGLCD.fillScr( 140, 140, 140);
  myGLCD.setBackColor( 140, 140, 140);
  // Title
  myGLCD.setFont(BigFont);
  myGLCD.setColor(255, 255, 255);  // White text
  myGLCD.print(" Cheese Vat System", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);

  //myGLCD.print("Developed by AE Design", CENTER, 440); ///will link RTC 
  drawLogo(); // AED logo - commented for fast code uploading  
  //myGLCD.drawBitmap(500, 90, 300, 300, (unsigned short*)cvat);  // cheese vat image
  // --- Startup Button ---
  myGLCD.setColor(0, 102, 204);  // Blue
  myGLCD.fillRoundRect(150, 110, 350, 210);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(150, 110, 350, 210);
  drawBoxCenteredText("Start Up",150, 110, 350, 210);
  // --- Shut Down Button ---
  myGLCD.setColor(0, 102, 204);  // Blue
  myGLCD.fillRoundRect(150, 260, 350, 360);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(150, 260, 350, 360);
  drawBoxCenteredText("Shut Down",150, 260, 350, 360);
}
/*
void drawShutDownScreen(){
  myGLCD.drawBitmap(34, 159, 732, 162, (unsigned short*)logo);  // (x, y, width, height, data)
  //------ back button ------
  drawBackButton();
  myGLCD.print("Back", 70, 18);
}
*/
void drawModeScreen(){
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  // Title
  myGLCD.setFont(BigFont);
  myGLCD.setColor(255, 255, 255);  // White text
  myGLCD.print(" Mode Selection Screen", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);

  // ===== Mode buttons ====

  // --- Auto Schedule Button ---
  myGLCD.setColor(0, 102, 204);  // Blue
  myGLCD.fillRoundRect(30, 90, 250, 190);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(30, 90, 250, 190);
  drawBoxCenteredText("Auto", 30, 110, 250, 140);
  drawBoxCenteredText("Schedule", 30, 140, 250, 170);

  // --- Auto Process Button ---
  myGLCD.setColor(0, 102, 204);  // Blue
  myGLCD.fillRoundRect(290, 90, 510, 190);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(290, 90, 510, 190);
  drawBoxCenteredText("Auto", 290, 110, 510, 140);
  drawBoxCenteredText("Process", 290, 140, 510, 170);

  // --- Manual Button ---
  myGLCD.setColor(0, 102, 204);  // Blue
  myGLCD.fillRoundRect(550, 90, 770, 190);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(550, 90, 770, 190);
  drawBoxCenteredText("Manual", 550, 90, 770, 190);

  // --- Programming Button ---
  myGLCD.setColor(0, 102, 204);  // Blue
  myGLCD.fillRoundRect(30, 230, 250, 330);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(30, 230, 250, 330);
  drawBoxCenteredText("Programming", 30, 230, 250, 330);

  // --- Service Interface Button ---
  myGLCD.setColor(0, 102, 204);  // Blue
  myGLCD.fillRoundRect(290, 230, 510, 330);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(290, 230, 510, 330);
  drawBoxCenteredText("Service", 290, 250, 510, 280);
  drawBoxCenteredText("Interface", 290, 280, 510, 310);

  // --- Back button ---
  drawBackButton();
  myGLCD.print("Back to Main Menu", 70, 18);
  
}

//======= Modes Screens ========
void drawAutoScheduleScreen(){
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  // Title
  myGLCD.setFont(BigFont);
  myGLCD.setColor(255, 255, 255);  // White text
  myGLCD.print(" Auto-Schedule Mode", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);

  // ===== Auto Schedule mode buttons ====

  // --- Auto Schedule - Pasteurization Button ---
  myGLCD.setColor(0, 102, 204);  // Blue
  myGLCD.fillRoundRect(30, 90, 260, 190);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(30, 90, 260, 190);
  drawBoxCenteredText("Pasteurization", 30, 90, 260, 190);

  // --- Auto Schedule - Cheese Making Button ---
  myGLCD.setColor(0, 102, 204);  // Blue
  myGLCD.fillRoundRect(280, 90, 430, 190);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(280, 90, 430, 190);
  drawBoxCenteredText("Cheese", 280, 110, 430, 140);
  drawBoxCenteredText("Making", 280, 140, 430, 170);

  // --- Auto Schedule - Cooling Button ---
  myGLCD.setColor(0, 102, 204);  // Blue
  myGLCD.fillRoundRect(450, 90, 600, 190);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(450, 90, 600, 190);
  drawBoxCenteredText("Cooling", 450, 90, 600, 190);

  // --- Auto Schedule - Cheese Drainage Button ---
  myGLCD.setColor(0, 102, 204);  // Blue
  myGLCD.fillRoundRect(620, 90, 770, 190);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(620, 90, 770, 190);
  drawBoxCenteredText("Cheese", 620, 110, 770, 140);
  drawBoxCenteredText("Drainage", 620, 140, 770, 170);

  // --- Auto Schedule - Water Drain Button ---
  myGLCD.setColor(0, 102, 204);  // Blue
  myGLCD.fillRoundRect(30, 230, 260, 330);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(30, 230, 260, 330);
  drawBoxCenteredText("Water Drainage", 30, 230, 260, 330);

  // --- Auto Schedule - Water Refill Button ---
  myGLCD.setColor(0, 102, 204);  // Blue
  myGLCD.fillRoundRect(280, 230, 430, 330);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(280, 230, 430, 330);
  drawBoxCenteredText("Water", 280, 250, 430, 280);
  drawBoxCenteredText("Refill", 280, 280, 430, 310);

  // --- Auto Schedule - Milk Filling Button ---   
  myGLCD.setColor(0, 102, 204);  // blue
  myGLCD.fillRoundRect(450, 230, 600, 330);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(450, 230, 600, 330);
  drawBoxCenteredText("Milk", 450, 250, 600, 280);
  drawBoxCenteredText("Filling", 450, 280, 600, 310);

  // --- Auto Schedule - Live Data Button --- 
  myGLCD.setColor(255, 102, 102);  // red
  myGLCD.fillRoundRect(620, 230, 770, 330);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 102, 102); // Text background also red
  myGLCD.drawRoundRect(620, 230, 770, 330);
  drawBoxCenteredText("Live", 620, 250, 770, 280);
  drawBoxCenteredText("Data", 620, 280, 770, 310);

  // --- Back button ---
  drawBackButton();
  myGLCD.print("Back to Mode Menu", 70, 18);
  
}
void drawAutoProcessScreen(){
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  // Title
  myGLCD.setFont(BigFont);
  myGLCD.setColor(255, 255, 255);  // White text
  myGLCD.print(" Auto-Process Mode", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);

  // ===== Auto Process mode buttons ====

  // --- Auto Process - Pasteurization Button ---
  myGLCD.setColor(0, 102, 204);  // Blue
  myGLCD.fillRoundRect(30, 90, 260, 190);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(30, 90, 260, 190);
  drawBoxCenteredText("Pasteurization", 30, 90, 260, 190);

  // --- Auto Process - Cheese Making Button ---
  myGLCD.setColor(0, 102, 204);  // Blue
  myGLCD.fillRoundRect(280, 90, 430, 190);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(280, 90, 430, 190);
  drawBoxCenteredText("Cheese", 280, 110, 430, 140);
  drawBoxCenteredText("Making", 280, 140, 430, 170);

  // --- Auto Process - Cooling Button ---
  myGLCD.setColor(0, 102, 204);  // Blue
  myGLCD.fillRoundRect(450, 90, 600, 190);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(450, 90, 600, 190);
  drawBoxCenteredText("Cooling", 450, 90, 600, 190);

  // --- Auto Process - Cheese Drainage Button ---
  myGLCD.setColor(0, 102, 204);  // Blue
  myGLCD.fillRoundRect(620, 90, 770, 190);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(620, 90, 770, 190);
  drawBoxCenteredText("Cheese", 620, 110, 770, 140);
  drawBoxCenteredText("Drainage", 620, 140, 770, 170);

  // --- Auto Process - Water Drain Button ---
  myGLCD.setColor(0, 102, 204);  // Blue
  myGLCD.fillRoundRect(30, 230, 260, 330);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(30, 230, 260, 330);
  drawBoxCenteredText("Water Drainage", 30, 230, 260, 330);

  // --- Auto Process - Water Refill Button ---
  myGLCD.setColor(0, 102, 204);  // Blue
  myGLCD.fillRoundRect(280, 230, 430, 330);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(280, 230, 430, 330);
  drawBoxCenteredText("Water", 280, 250, 430, 280);
  drawBoxCenteredText("Refill", 280, 280, 430, 310);

  // --- Auto Process - milk filling Button ---   
  myGLCD.setColor(0, 102, 204);  // blue
  myGLCD.fillRoundRect(450, 230, 600, 330);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(450, 230, 600, 330);
  drawBoxCenteredText("Milk", 450, 250, 600, 280);
  drawBoxCenteredText("Filling", 450, 280, 600, 310);

  // --- Back button ---
  drawBackButton();
  myGLCD.print("Back to Mode Menu", 70, 18);
}
void drawManualScreen(){
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  // Title
  myGLCD.setFont(BigFont);
  myGLCD.setColor(255, 255, 255);  // White text
  myGLCD.print(" Manual Mode", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);

  // ===== Manual mode buttons ====

  // --- Manual - Pasteurization Button ---
  myGLCD.setColor(0, 102, 204);  // Blue
  myGLCD.fillRoundRect(30, 90, 260, 190);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(30, 90, 260, 190);
  drawBoxCenteredText("Pasteurization", 30, 90, 260, 190);

  // --- Manual - Cheese Making Button ---
  myGLCD.setColor(0, 102, 204);  // Blue
  myGLCD.fillRoundRect(280, 90, 430, 190);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(280, 90, 430, 190);
  drawBoxCenteredText("Cheese", 280, 110, 430, 140);
  drawBoxCenteredText("Making", 280, 140, 430, 170);

  // --- Manual - Cooling Button ---
  myGLCD.setColor(0, 102, 204);  // Blue
  myGLCD.fillRoundRect(450, 90, 600, 190);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(450, 90, 600, 190);
  drawBoxCenteredText("Cooling", 450, 90, 600, 190);

  // --- Manual - Cheese Drainage Button ---
  myGLCD.setColor(0, 102, 204);  // Blue
  myGLCD.fillRoundRect(620, 90, 770, 190);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(620, 90, 770, 190);
  drawBoxCenteredText("Cheese", 620, 110, 770, 140);
  drawBoxCenteredText("Drainage", 620, 140, 770, 170);

  // --- Manual - Water Drain Button ---
  myGLCD.setColor(0, 102, 204);  // Blue
  myGLCD.fillRoundRect(30, 230, 260, 330);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(30, 230, 260, 330);
  drawBoxCenteredText("Water Drainage", 30, 230, 260, 330);

  // --- Manual - Water Refill Button ---
  myGLCD.setColor(0, 102, 204);  // Blue
  myGLCD.fillRoundRect(280, 230, 430, 330);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(280, 230, 430, 330);
  drawBoxCenteredText("Water", 280, 250, 430, 280);
  drawBoxCenteredText("Refill", 280, 280, 430, 310);

  // --- Manual - Milk Filling Button ---  
  myGLCD.setColor(0, 102, 204);  // red
  myGLCD.fillRoundRect(450, 230, 600, 330);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also red
  myGLCD.drawRoundRect(450, 230, 600, 330);
  drawBoxCenteredText("Milk", 450, 250, 600, 280);
  drawBoxCenteredText("Filling", 450, 280, 600, 310);

    // --- Manual - Live Data Button ---  
  myGLCD.setColor(255, 102, 102);  // red
  myGLCD.fillRoundRect(620, 230, 770, 330);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 102, 102); // Text background also red
  myGLCD.drawRoundRect(620, 230, 770, 330);
  drawBoxCenteredText("Live", 620, 250, 770, 280);
  drawBoxCenteredText("Data", 620, 280, 770, 310);

    // --- Back button ---
  drawBackButton();
  myGLCD.print("Back to Mode Menu", 70, 18);
}
void drawProgrammingScreen(){
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  // Title
  myGLCD.setFont(BigFont);
  myGLCD.setColor(255, 255, 255);  // White text
  myGLCD.print(" Programming Mode", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);

  // --- Programming - Pasteurization Setting Button ---
  myGLCD.setColor(0, 102, 204);  // Blue
  myGLCD.fillRoundRect(30, 175, 260, 275);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(30, 175, 260, 275);
  drawBoxCenteredText("Pasteurization", 30, 175, 260, 275);

  // --- Programming - Cheese Setting Button ---
  myGLCD.setColor(0, 102, 204);  // Blue
  myGLCD.fillRoundRect(285, 175, 515, 275);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(285, 175, 515, 275);
  drawBoxCenteredText("Cheese Making", 285, 175, 515, 275);
  // drawBoxCenteredText("Making", 280, 140, 430, 190);

  // --- Programming - Cooling Setting Button ---
  myGLCD.setColor(0, 102, 204);  // Blue
  myGLCD.fillRoundRect(540, 175, 770, 275);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(540, 175, 770, 275);
  drawBoxCenteredText("Cooling", 540, 175, 770, 275);

  // --- Back button ---
  drawBackButton();
  myGLCD.print("Back to Mode Menu", 70, 18);
}
void drawServiceInterfaceScreen(){
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  // Title
  myGLCD.setFont(BigFont);
  myGLCD.setColor(255, 255, 255);  // White text
  myGLCD.print(" Service Interface Mode", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);

  // --- Run Test Button ---
  myGLCD.setColor(0, 102, 204);  // Blue
  myGLCD.fillRoundRect(150, 180, 330, 280);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(150, 180, 330, 280);
  drawBoxCenteredText("Run test", 150, 180, 330, 280);

  // --- View Logs Button ---
  myGLCD.setColor(0, 102, 204);  // Blue
  myGLCD.fillRoundRect(470, 180, 650, 280);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(470, 180, 650, 280);
  drawBoxCenteredText("View Logs", 470, 180, 650, 280);

    // --- Back button ---
  drawBackButton();
  myGLCD.print("Back to Mode Menu", 70, 18);
}

//======= Auto Schedule Screens ========
void drawAS_PasteurizationScreen(){
  // Set full background to grey
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(BigFont);

  // Header
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("SCHEDULE PASTEURIZATION", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);


  drawAS_GenericScreen();
  drawKeypadSec();
}
void drawAS_CheeseMakingScreen(){
  // Set full background to grey
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(BigFont);

  // Header
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("SCHEDULE CHEESE MAKING", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);
  drawAS_GenericScreen();
  drawKeypadSec();
}
void drawAS_CoolingScreen(){
  // Set full background to grey
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(BigFont);

  // Header
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("SCHEDULE COOLING", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);

  drawAS_GenericScreen();
  drawKeypadSec();
}
void drawAS_CheeseDrainageScreen(){
  // Set full background to grey
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(BigFont);

  // Header
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("SCHEDULE CHEESE DRAINAGE", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);
  drawAS_GenericScreen();
  drawKeypadSec();
}
void drawAS_WaterDrainageScreen(){
  // Set full background to grey
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(BigFont);

  // Header
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("SCHEDULE WATER DRAINAGE", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);

  drawAS_GenericScreen();
  drawKeypadSec();
}
void drawAS_WaterRefillScreen(){
  // Set full background to grey
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(BigFont);

  // Header
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("SCHEDULE WATER REFILL", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);

  drawAS_GenericScreen();
  drawKeypadSec();
}
void drawAS_MilkRefillScreen(){
  // Set full background to grey
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(BigFont);

  // Header
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("SCHEDULE MILK REFILL", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);

  drawAS_GenericScreen();
  drawKeypadSec();
}

//======= Auto Process Screens ========
void drawAP_PasteurizationScreen(){
  // Set full background to grey
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(BigFont);

  // Header
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Auto-Process PASTEURIZATION", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);

  myGLCD.setFont(BigFont);
  // --- Target temperature of milk ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Target Heat Temp.:", 10, 80);
  myGLCD.drawRoundRect(300, 75, 370, 100);
    //myGLCD.setColor(255, 255, 255);
  myGLCD.print(pasteurizationProgram.targetTemp, 310, 80);
  myGLCD.print("C", 380, 80);

  //--- Temperature Regulation ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Temp. Regulation:", 10, 120);  
  drawToggleButton(300, 115, pasteurizationProgram.tempRegulation, "ON", "OFF");

  //--- Temp Holding time --- 
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Temp. Hold Time:", 10, 160);  
  myGLCD.drawRoundRect(300, 155, 370, 180);
  myGLCD.print(pasteurizationProgram.holdTimeMinutes, 310, 160);
  myGLCD.print("min", 380, 160);

  //--- Stir Direction Heating/cooling ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Stir Dir. Heat:", 10, 200);  
  drawToggleButton( 300, 195, pasteurizationProgram.stirDirHeating, "CW", "CCW");

  //--- Stir Speed Heating/cooling ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Stir Speed Heat:", 10, 240);  
  myGLCD.drawRoundRect(300, 235, 380, 260);
  myGLCD.print(pasteurizationProgram.stirSpeedHeating, 310, 240);
  myGLCD.print("rpm", 390, 240);

  //--- Stir Direction Holding ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Stir Dir. Holding:", 10, 280);  
  drawToggleButton( 300, 275, pasteurizationProgram.stirDirHolding, "CW", "CCW");

  //--- Stir Speed Holding ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Stir Speed Holding:", 10, 320);  
  myGLCD.drawRoundRect(310, 315, 390, 340);
  myGLCD.print(pasteurizationProgram.stirSpeedHolding, 320, 320);
  myGLCD.print("rpm", 400, 320);

  // --- Target cooling temperature of milk ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Target Cool Temp.:", 10, 360);
  myGLCD.drawRoundRect(300, 355, 370, 380);
  myGLCD.print(pasteurizationProgram.targetCoolTemp, 310, 360);
  myGLCD.print("C", 400, 360);

  // --- Milk Refill Button --- 
  myGLCD.setColor(0, 102, 204);  // blue
  myGLCD.fillRoundRect(180, 410, 280, 460);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(180, 410, 280, 460);
  drawBoxCenteredText("Refill", 180, 410, 280, 435);
  drawBoxCenteredText("Milk", 180, 435, 280, 460);
  
  // --- Live Data Button --- 
  myGLCD.setFont(BigFont);
  myGLCD.setColor(255, 102, 102);  // pink
  myGLCD.fillRoundRect(620, 230, 770, 330);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 102, 102); // Text background also pink
  myGLCD.drawRoundRect(620, 230, 770, 330);
  drawBoxCenteredText("Live", 620, 250, 770, 280);
  drawBoxCenteredText("Data", 620, 280, 770, 310);
  
  //------ Run button -------
  drawRunButton(620, 110, 770, 210);
  //------ Stop button -------
  drawStopButton(620, 350, 770, 450);
  //------ back button ------
  drawBackButton();
  myGLCD.print("Back", 70, 18);

}
void drawAP_CheeseMakingScreen(){
  // Set full background to grey
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(BigFont);

  // Header
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Auto-Process CHEESE MAKING", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);

  myGLCD.setFont(BigFont);
  // --- Target temperature of milk ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Target Temp.:", 10, 80);
  myGLCD.drawRoundRect(300, 75, 390, 100);
  myGLCD.print(cheeseMakingProgram.targetTemp, 310, 80);
  myGLCD.print("C", 400, 80);

  //--- Temperature Regulation ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Temp. Regulation:", 10, 120);  
  drawToggleButton(300, 115, cheeseMakingProgram.tempRegulation, "ON", "OFF");
  //drawToggleButton(300, 115, true, "ON", "OFF");
  
  //--- Temp Holding time --- 
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Temp. Hold Time:", 10, 160);  
  myGLCD.drawRoundRect(300, 155, 380, 180);
  myGLCD.print(cheeseMakingProgram.holdTimeMinutes, 310, 160);
  myGLCD.print("min", 390, 160);

  //--- Stir Direction Heating/cooling ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Stir Dir. Heat:", 10, 200);  
  drawToggleButton( 300, 195, cheeseMakingProgram.stirDirHeating, "CW", "CCW");
  
  //--- Stir Speed Heating/cooling ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Stir Speed Heat:", 10, 240);  
  myGLCD.drawRoundRect(300, 235, 390, 260);
  myGLCD.print(cheeseMakingProgram.stirSpeedHeating, 310, 240);
  myGLCD.print("rpm", 400, 240);

  //--- Stir Direction Holding ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Stir Dir. Holding:", 10, 280);  
  drawToggleButton( 300, 275, cheeseMakingProgram.stirDirHolding, "CW", "CCW");
  
  //--- Stir Speed Holding ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Stir Speed Holding:", 10, 320);  
  myGLCD.drawRoundRect(310, 315, 390, 340);
  myGLCD.print(cheeseMakingProgram.stirSpeedHolding, 320, 320);
  myGLCD.print("rpm", 410, 320);

  // --- Target cooling temperature of milk ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Target Cool Temp.:", 10, 360);
  myGLCD.drawRoundRect(300, 355, 370, 380);
  myGLCD.print(cheeseMakingProgram.targetCoolTemp, 310, 360);
  myGLCD.print("C", 400, 360);

  // --- Milk Refill Button --- 
  myGLCD.setColor(0, 102, 204);  // blue
  myGLCD.fillRoundRect(180, 410, 280, 460);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(180, 410, 280, 460);
  drawBoxCenteredText("Refill", 180, 410, 280, 435);
  drawBoxCenteredText("Milk", 180, 435, 280, 460);

  // --- Live Data Button --- 
  myGLCD.setFont(BigFont);
  myGLCD.setColor(255, 102, 102);  // red
  myGLCD.fillRoundRect(620, 230, 770, 330);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 102, 102); // Text background also red
  myGLCD.drawRoundRect(620, 230, 770, 330);
  drawBoxCenteredText("Live", 620, 250, 770, 280);
  drawBoxCenteredText("Data", 620, 280, 770, 310);
  //------ Run button -------
  drawRunButton(620, 110, 770, 210);
  //------ Stop button -------
  drawStopButton(620, 350, 770, 450);
  //------ back button ------
  drawBackButton();
  myGLCD.print("Back", 70, 18);
}
void drawAP_CoolingScreen(){
  // Set full background to grey
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(BigFont);

  // Header
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Auto-Process COOLING", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);

  myGLCD.setFont(BigFont);
  // --- Target temperature of milk ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Target Cool Temp.:", 10, 80);
  myGLCD.drawRoundRect(300, 75, 390, 100);
  myGLCD.print(coolingProgram.targetTemp, 310, 80);
  myGLCD.print("C", 400, 80);

  // --- Live Data Button --- 
  myGLCD.setFont(BigFont);
  myGLCD.setColor(255, 102, 102);  // red
  myGLCD.fillRoundRect(620, 230, 770, 330);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 102, 102); // Text background also red
  myGLCD.drawRoundRect(620, 230, 770, 330);
  drawBoxCenteredText("Live", 620, 250, 770, 280);
  drawBoxCenteredText("Data", 620, 280, 770, 310);
  //------ Run button -------
  drawRunButton(620, 110, 770, 210);
  //------ Stop button -------
  drawStopButton(620, 350, 770, 450);
  //------ back button ------
  drawBackButton();
  myGLCD.print("Back", 70, 18);
}
void drawAP_CheeseDrainageScreen(){
  // Set full background to grey
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(BigFont);

  // Header
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Auto-Process CHEESE DRAINAGE", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);
  // --- Live Data Button --- 
  myGLCD.setFont(BigFont);
  myGLCD.setColor(255, 102, 102);  // red
  myGLCD.fillRoundRect(620, 230, 770, 330);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 102, 102); // Text background also red
  myGLCD.drawRoundRect(620, 230, 770, 330);
  drawBoxCenteredText("Live", 620, 250, 770, 280);
  drawBoxCenteredText("Data", 620, 280, 770, 310);
  //------ Run button -------
  drawRunButton(620, 110, 770, 210);
  //------ Stop button -------
  drawStopButton(620, 350, 770, 450);
  //------ back button ------
  drawBackButton();
  myGLCD.print("Back", 70, 18);

}
void drawAP_WaterDrainageScreen(){
  // Set full background to grey
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(BigFont);

  // Header
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Auto-Process WATER DRAINAGE", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);
  // --- Live Data Button --- 
  myGLCD.setFont(BigFont);
  myGLCD.setColor(255, 102, 102);  // red
  myGLCD.fillRoundRect(620, 230, 770, 330);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 102, 102); // Text background also red
  myGLCD.drawRoundRect(620, 230, 770, 330);
  drawBoxCenteredText("Live", 620, 250, 770, 280);
  drawBoxCenteredText("Data", 620, 280, 770, 310);
  //------ Run button -------
  drawRunButton(620, 110, 770, 210);
  //------ Stop button -------
  drawStopButton(620, 350, 770, 450);
  //------ back button ------
  drawBackButton();
  myGLCD.print("Back", 70, 18);
}
void drawAP_WaterRefillScreen(){
  // Set full background to grey
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(BigFont);

  // Header
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Auto-Process WATER REFILL", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);
  // --- Live Data Button --- 
  myGLCD.setFont(BigFont);
  myGLCD.setColor(255, 102, 102);  // red
  myGLCD.fillRoundRect(620, 230, 770, 330);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 102, 102); // Text background also red
  myGLCD.drawRoundRect(620, 230, 770, 330);
  drawBoxCenteredText("Live", 620, 250, 770, 280);
  drawBoxCenteredText("Data", 620, 280, 770, 310);
  //------ Run button -------
  drawRunButton(620, 110, 770, 210);
  //------ Stop button -------
  drawStopButton(620, 350, 770, 450);
  //------ back button ------
  drawBackButton();
  myGLCD.print("Back", 70, 18);
}
void drawAP_MilkRefillScreen(){
  // Set full background to grey
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(BigFont);

  // Header
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Auto-Process MILK REFILL", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);
  // --- Live Data Button ---
  myGLCD.setFont(BigFont); 
  myGLCD.setColor(255, 102, 102);  // red
  myGLCD.fillRoundRect(620, 230, 770, 330);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 102, 102); // Text background also red
  myGLCD.drawRoundRect(620, 230, 770, 330);
  drawBoxCenteredText("Live", 620, 250, 770, 280);
  drawBoxCenteredText("Data", 620, 280, 770, 310);
  //------ Run button -------
  drawRunButton(620, 110, 770, 210);
  //------ Stop button -------
  drawStopButton(620, 350, 770, 450);
  //------ back button ------
  drawBackButton();
  myGLCD.print("Back", 70, 18);
}

//======= Programming Screens ========
void drawPRG_PasteurizationScreen(){
  // Set full background to grey
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(BigFont);

  // Header
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Program PASTEURIZATION", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);

  myGLCD.setFont(BigFont);
  // --- Target heating temperature of milk ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Target Heat Temp.:", 10, 80);
  myGLCD.drawRoundRect(300, 75, 390, 100);
  myGLCD.print("C", 400, 80);

  //--- Temperature Regulation ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Temp. Regulation:", 10, 120);  
  drawToggleButton(300, 115, true, "ON", "OFF");
  
  //--- Temp Holding time --- 
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Temp. Hold Time:", 10, 160);  
  myGLCD.drawRoundRect(300, 155, 380, 180);
  myGLCD.print("min", 390, 160);

  //--- Stir Direction Heating/cooling ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Stir Dir. Heat:", 10, 200);  
  drawToggleButton(300, 195, true, "CW", "CCW");
  
  //--- Stir Speed Heating/cooling ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Stir Speed Heat:", 10, 240);  
  myGLCD.drawRoundRect(300, 235, 380, 260);
  myGLCD.print("rpm", 390, 240);

  //--- Stir Direction Holding ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Stir Dir. Holding:", 10, 280);  
  drawToggleButton(300, 275, true, "CW", "CCW");

  //--- Stir Speed Holding ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Stir Speed Holding:", 10, 320);  
  myGLCD.drawRoundRect(310, 315, 390, 340);
  myGLCD.print("rpm", 400, 320);

  // --- Target cooling temperature of milk ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Target Cool Temp.:", 10, 360);
  myGLCD.drawRoundRect(300, 355, 390, 380);
  myGLCD.print("C", 400, 360);

  // --- save Data Button --- 
  myGLCD.setFont(BigFont);
  myGLCD.setColor(255, 102, 102);  // red
  myGLCD.fillRoundRect(180, 410, 280, 460);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 102, 102); // Text background also red
  myGLCD.drawRoundRect(180, 410, 280, 460);
  drawBoxCenteredText("Save", 180, 410, 280, 460);
  
  myGLCD.drawLine(450, 60, 450, 440); //vertical
  drawKeypadSec();
  //------ back button ------
  drawBackButton();
  myGLCD.print("Back", 70, 18);

}
void drawPRG_CheeseMakingScreen(){
  // Set full background to grey
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(BigFont);

  // Header
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Program CHEESE MAKING", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);

  myGLCD.setFont(BigFont);
  // --- Target temperature of milk ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Target Temp.:", 10, 80);
  myGLCD.drawRoundRect(300, 75, 390, 100);
  myGLCD.print("C", 400, 80);

  //--- Temperature Regulation ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Temp. Regulation:", 10, 120);  
  drawToggleButton(300, 115, true, "ON", "OFF");
  
  //--- Temp Holding time --- 
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Temp. Hold Time:", 10, 160);  
  myGLCD.drawRoundRect(300, 155, 380, 180);
  myGLCD.print("min", 390, 160);

  //--- Stir Direction Heating/cooling ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Stir Dir. Heat:", 10, 200);  
  drawToggleButton(300, 195, true, "CW", "CCW");
  

  //--- Stir Speed Heating/cooling ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Stir Speed Heat:", 10, 240);  
  myGLCD.drawRoundRect(300, 235, 390, 260);
  myGLCD.print("rpm", 400, 240);

  //--- Stir Direction Holding ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Stir Dir. Holding:", 10, 280);  
  drawToggleButton(300, 275, true, "CW", "CCW");
  
  //--- Stir Speed Holding ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Stir Speed Holding:", 10, 320);  
  myGLCD.drawRoundRect(310, 315, 390, 340);
  myGLCD.print("rpm", 400, 320);

  // --- Target cooling temperature of milk ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Target Cool Temp.:", 10, 360);
  myGLCD.drawRoundRect(300, 355, 390, 380);
  myGLCD.print("C", 400, 360);

  // --- save Data Button --- 
  myGLCD.setFont(BigFont);
  myGLCD.setColor(255, 102, 102);  // red
  myGLCD.fillRoundRect(180, 410, 280, 460);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 102, 102); // Text background also red
  myGLCD.drawRoundRect(180, 410, 280, 460);
  drawBoxCenteredText("Save", 180, 410, 280, 460);
  
  myGLCD.drawLine(450, 60, 450, 440); //vertical
  drawKeypadSec();
  //------ back button ------
  drawBackButton();
  myGLCD.print("Back", 70, 18);
}
void drawPRG_CoolingScreen(){
  // Set full background to grey
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(BigFont);

  // Header
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Program COOLING", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);

  // --- Target temperature of milk ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Target Cool Temp.:", 10, 80);
  myGLCD.drawRoundRect(300, 75, 390, 100);
  myGLCD.print("C", 400, 80);
  /*
  myGLCD.setColor(0, 102, 204);  // Blue
  myGLCD.fillRoundRect(40, 200, 240, 280);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(40, 200, 240, 280);
  drawBoxCenteredText("Target", 40, 210, 240, 229);
  drawBoxCenteredText("Cooling", 40, 232, 240, 251);
  drawBoxCenteredText("Temperature", 40, 254, 240, 270);

  //input box for target cooling tremperature
  myGLCD.drawRoundRect(295, 200, 395, 280);
  */
  // --- save Data Button --- 
  myGLCD.setFont(BigFont);
  myGLCD.setColor(255, 102, 102);  // red
  myGLCD.fillRoundRect(180, 360, 280, 410);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 102, 102); // Text background also red
  myGLCD.drawRoundRect(180, 360, 280, 410);
  drawBoxCenteredText("Save", 180, 360, 280, 410);

  myGLCD.drawLine(450, 60, 450, 440); //vertical
  drawKeypadSec();

  drawBackButton();
  myGLCD.print("Back", 70, 18);
}

//======= Manual Screens ========
void drawMan_PasteurizationScreen(){
  // Set full background to grey
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(BigFont);

  // Header
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Manual - PASTEURIZATION", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);

  myGLCD.setFont(BigFont);
  // --- Target temperature of milk ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Target Temp.:", 10, 80);
  myGLCD.drawRoundRect(300, 75, 390, 100);
  myGLCD.print("C", 400, 80);

  //--- Temperature Regulation ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Temp. Regulation:", 10, 120);  
  drawToggleButton(300, 115, true, "ON", "OFF");
  
  //--- Temp Holding time --- 
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Temp. Hold Time:", 10, 160);  
  myGLCD.drawRoundRect(300, 155, 380, 180);
  myGLCD.print("min", 390, 160);

  //--- Stir Direction Heating/cooling ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Stir Dir. Heat:", 10, 200);  
  drawToggleButton(300, 195, true, "CW", "CCW");
  
  //--- Stir Speed Heating/cooling ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Stir Speed Heat:", 10, 240);  
  myGLCD.drawRoundRect(300, 235, 390, 260);
  myGLCD.print("rpm", 400, 240);

  //--- Stir Direction Holding ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Stir Dir. Holding:", 10, 280);  
  drawToggleButton(300, 275, false, "CW", "CCW");

  //--- Stir Speed Holding ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Stir Speed Holding:", 10, 320);  
  myGLCD.drawRoundRect(310, 315, 390, 340);
  myGLCD.print("rpm", 400, 320);

  // --- Target cooling temperature of milk ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Target Cool Temp.:", 10, 360);
  myGLCD.drawRoundRect(300, 355, 390, 380);
  myGLCD.print("C", 400, 360);

  // --- save Data Button --- 
  myGLCD.setFont(BigFont);
  myGLCD.setColor(255, 102, 102);  // red
  myGLCD.fillRoundRect(180, 410, 280, 460);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 102, 102); // Text background also red
  myGLCD.drawRoundRect(180, 410, 280, 460);
  drawBoxCenteredText("Save", 180, 410, 280, 460);
  
  myGLCD.drawLine(450, 60, 450, 440); //vertical
  //-------Keypad------------
  drawKeypadSec();
  //-------Run button------------
  drawRunButton(470, 360, 560, 410);
  //-------Run button------------
  drawStopButton(670, 360, 770, 410);
  //------ back button ------
  drawBackButton();
  myGLCD.print("Back", 70, 18);
}
void drawMan_CheeseMakingScreen(){
  // Set full background to grey
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(BigFont);

  // Header
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Manual - CHEESE MAKING", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);

  myGLCD.setFont(BigFont);
  // --- Target temperature of milk ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Target Temp.:", 10, 80);
  myGLCD.drawRoundRect(300, 75, 390, 100);
  myGLCD.print("C", 400, 80);

  //--- Temperature Regulation ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Temp. Regulation:", 10, 120);  
  drawToggleButton(300, 115, true, "ON", "OFF");
  
  //--- Temp Holding time --- 
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Temp. Hold Time:", 10, 160);  
  myGLCD.drawRoundRect(300, 155, 380, 180);
  myGLCD.print("min", 390, 160);

  //--- Stir Direction Heating/cooling ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Stir Dir. Heat:", 10, 200);  
  drawToggleButton(300, 195, true, "CW", "CCW");

  //--- Stir Speed Heating/cooling ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Stir Speed Heat:", 10, 240);  
  myGLCD.drawRoundRect(300, 235, 390, 260);
  myGLCD.print("rpm", 400, 240);

  //--- Stir Direction Holding ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Stir Dir. Holding:", 10, 280);  
  drawToggleButton(300, 275, true, "CW", "CCW");
  
  //--- Stir Speed Holding ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Stir Speed Holding:", 10, 320);  
  myGLCD.drawRoundRect(310, 315, 390, 340);
  myGLCD.print("rpm", 400, 320);

  // --- Target cooling temperature of milk ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Target Cool Temp.:", 10, 360);
  myGLCD.drawRoundRect(300, 355, 390, 380);
  myGLCD.print("C", 400, 360);

  // --- save Data Button --- 
  myGLCD.setFont(BigFont);
  myGLCD.setColor(255, 102, 102);  // red
  myGLCD.fillRoundRect(180, 410, 280, 460);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 102, 102); // Text background also red
  myGLCD.drawRoundRect(180, 410, 280, 460);
  drawBoxCenteredText("Save", 180, 410, 280, 460);
  
  myGLCD.drawLine(450, 60, 450, 440); //vertical
  //-------Keypad------------
  drawKeypadSec();
  //-------Run button------------
  drawRunButton(470, 360, 560, 410);
  //-------Run button------------
  drawStopButton(670, 360, 770, 410);
  //------ back button ------
  drawBackButton();
  myGLCD.print("Back", 70, 18);
}
void drawMan_CoolingScreen(){
  // Set full background to grey
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(BigFont);

  // Header
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Manual - COOLING", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);

  // --- Target temperature of milk ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Target Cool Temp.:", 10, 80);
  myGLCD.drawRoundRect(300, 75, 390, 100);
  myGLCD.print("C", 400, 80);
  // --- save Data Button --- 
  myGLCD.setFont(BigFont);
  myGLCD.setColor(255, 102, 102);  // red
  myGLCD.fillRoundRect(180, 360, 280, 410);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 102, 102); // Text background also red
  myGLCD.drawRoundRect(180, 360, 280, 410);
  drawBoxCenteredText("Save", 180, 360, 280, 410);
  myGLCD.drawLine(450, 60, 450, 440); //vertical
  //-------Keypad------------
  drawKeypadSec();
  //-------Run button------------
  drawRunButton(470, 360, 560, 410);
  //-------Run button------------
  drawStopButton(670, 360, 770, 410);
  //------ back button ------
  drawBackButton();
  myGLCD.print("Back", 70, 18);
}
void drawMan_CheeseDrainageScreen(){
  // Set full background to grey
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(BigFont);

  // Header
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Manual - CHEESE DRAINAGE", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);

  //--- Cheese Drain Valve status ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Cheese Drain Valve:", 10, 80);  
  //bool valveCheeseDrain = readValveCheeseDrain(); // true if cheese drain valve open, false if closed
  bool valveCheeseDrain = false;
  drawToggleButton(350, 75, valveCheeseDrain, "ON", "OFF");
  // ----- Product Level in vat ---------
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Product Level:", 10, 120);  
  drawLevelBar(350, 120, 100, 150, 89, VGA_CHEESE); // x, y , width ,height , current percentage(from the level sensor), color
  //-------Run button------------
  drawRunButton(620, 110, 770, 210);
  //------- Stop button------------
  drawStopButton(620, 230, 770, 330);
  //------ back button ------
  drawBackButton();
  myGLCD.print("Back", 70, 18);
}
void drawMan_WaterDrainageScreen(){
  // Set full background to grey
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(BigFont);

  // Header
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Manual - WATER DRAINAGE", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);

  //--- Water Drain Valve status ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Water Drain Valve:", 10, 80);  
  //bool valveWaterDrain = readValveWaterDrain(); // true if cheese drain valve open, false if closed
  bool valveWaterDrain = false;
  drawToggleButton(350, 75, valveWaterDrain, "ON", "OFF");
  // ----- Water Level ---------
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Water Level:", 10, 120);  
  drawLevelBar(350, 120, 100, 150, 69, VGA_AQUA); // x, y , width ,height , current percentage(from the level sensor)
  //-------Run button------------
  drawRunButton(620, 110, 770, 210);
  //------- Stop button------------
  drawStopButton(620, 230, 770, 330);
  //------ back button ------
  drawBackButton();
  myGLCD.print("Back", 70, 18);
}
void drawMan_WaterRefillScreen(){
  // Set full background to grey
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(BigFont);

  // Header
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Manual - WATER REFILL", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);

  //--- Water Inlet Valve status ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Water Inlet Valve:", 10, 80);  
  //bool valveWaterInlet = readValveWaterInlet(); // true if cheese drain valve open, false if closed
  bool valveWaterInlet = false;
  drawToggleButton(350, 75, valveWaterInlet, "ON", "OFF");
  // ----- Water Level ---------
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Water Level:", 10, 120);  
  drawLevelBar(350, 120, 100, 150, 69, VGA_AQUA); // x, y , width ,height , current percentage(from the level sensor
  //-------Run button------------
  drawRunButton(620, 110, 770, 210);
  //------- Stop button------------
  drawStopButton(620, 230, 770, 330);
  //------ back button ------
  drawBackButton();
  myGLCD.print("Back", 70, 18);
}
void drawMan_MilkRefillScreen(){
  // Set full background to grey
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(BigFont);

  // Header
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Manual - MILK REFILL", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);

  //--- Milk inlet Valve status ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Milk Inlet Valve:", 10, 80);  
  //bool valveMilkInlet = readvalveMilkInlet(); // true if cheese drain valve open, false if closed
  bool valveMilkInlet = false;
  drawToggleButton(300, 75, valveMilkInlet, "ON", "OFF");
  // ----- Product Level in vat ---------
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Milk Level:", 10, 120);  
  drawLevelBar(300, 120, 100, 160, 11, VGA_CHEESE); // x, y , width ,height , current percentage(from the level sensor), color
  // --- Input the amount of milk ---
  myGLCD.setFont(BigFont);
  myGLCD.setColor(0, 102, 204);  // Blue
  myGLCD.fillRoundRect(10, 160, 210, 240);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(10, 160, 210, 280);
  drawBoxCenteredText("Target", 10, 163, 210, 185);
  drawBoxCenteredText("Milk", 10, 188, 210, 210);
  drawBoxCenteredText("Refill (ltr)", 10, 213, 210, 235);

  // --- save Data Button --- 
  myGLCD.setFont(BigFont);
  myGLCD.setColor(255, 102, 102);  // red
  myGLCD.fillRoundRect(180, 360, 280, 410);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 102, 102); // Text background also red
  myGLCD.drawRoundRect(180, 360, 280, 410);
  drawBoxCenteredText("Save", 180, 360, 280, 410);

  myGLCD.drawLine(450, 60, 450, 440); //vertical
  //-------Keypad------------
  drawKeypadSec();
  //-------Run button------------
  drawRunButton(470, 360, 570, 410);
  //-------Run button------------
  drawStopButton(670, 360, 770, 410);
  //------ back button ------
  drawBackButton();
  myGLCD.print("Back", 70, 18);
}

//====== Service Screens =====
void drawSI_RunTestScreen(){
  // Set full background to grey
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(BigFont);

  // Header
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("RUN TEST", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);

  // --- Hardware Diagnostic Button ---
  myGLCD.setColor(0, 102, 204);  // Blue
  myGLCD.fillRoundRect(150, 180, 330, 280);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(150, 180, 330, 280);
  drawBoxCenteredText("Diagnostics", 150, 180, 330, 280);

  // --- Monitor sensors Button ---
  myGLCD.setColor(0, 102, 204);  // Blue
  myGLCD.fillRoundRect(470, 180, 650, 280);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(470, 180, 650, 280);
  drawBoxCenteredText("Monitor", 470, 200, 650, 230);
  drawBoxCenteredText("Sensors", 470, 230, 650, 260);

  drawBackButton();
}
void drawSI_ViewLogsScreen(){
  // Set full background to grey
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(BigFont);

  // Header
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("VIEW LOGS", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);
  
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Select Date:", 10, 80);
  myGLCD.drawRect(200, 75, 440, 105); // Date input box
  //drawBoxCenteredText("", 200, 75, 440, 105);
  myGLCD.drawLine(450, 60, 450, 440); //vertical
  
  drawDropdown();
  drawKeypadSec();
  
  drawBackButton();
  
}
//---SI --> Run test
void drawSI_RunDiagnosticsScreen(){
  // Set full background to grey
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(BigFont);

  // Header
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("DIAGNOSTICS", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);

  // --- Temperature sensors Button ---
  myGLCD.setColor(0, 102, 204);  // Blue
  myGLCD.fillRoundRect(20, 90, 260, 190);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(20, 90, 260, 190);
  drawBoxCenteredText("Temperature", 20, 110, 260, 140);
  drawBoxCenteredText("Sensors", 20, 140, 260, 170);

  // --- RTC Button ---
  myGLCD.setColor(0, 102, 204);  // Blue
  myGLCD.fillRoundRect(280, 90, 520, 190);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(280, 90, 520, 190);
  drawBoxCenteredText("RTC", 280, 90, 520, 190);

  // --- Relays Button ---
  myGLCD.setColor(0, 102, 204);  // Blue
  myGLCD.fillRoundRect(540, 90, 780, 190);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(540, 90, 780, 190);
  drawBoxCenteredText("Relays", 540, 90, 780, 190);

  // --- Agitator Motor Button ---
  myGLCD.setColor(0, 102, 204);  // Blue
  myGLCD.fillRoundRect(20, 230, 260, 330);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(20, 230, 260, 330);
  drawBoxCenteredText("Agitator", 20, 250, 260, 280);
  drawBoxCenteredText("Motor", 20, 280, 260, 310);

  // --- Valves Button ---
  myGLCD.setColor(0, 102, 204);  // Blue
  myGLCD.fillRoundRect(280, 230, 520, 330);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(280, 230, 520, 330);
  drawBoxCenteredText("Valves", 280, 230, 520, 330);

  // --- SD Card Button ---
  myGLCD.setColor(0, 102, 204);  // Blue
  myGLCD.fillRoundRect(540, 230, 780, 330);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(540, 230, 780, 330);
  drawBoxCenteredText("SD Card", 540, 230, 780, 330);
  
  drawBackButton();
}
void drawSI_RunMonitorSensorsScreen(){

  // Set full background to grey
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(BigFont);

  // Header
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("SENSOR MONITORING", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);
  drawLiveDataScreen();
  //drawBackButton();
}
//---SI --> Run test --> Diagnostic
void drawDiagnostics_TempScreen(){
  // Set full background to grey
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(BigFont);

  // Header
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("TEMPERATURE SENSORS", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);

  // --- Live data of temperature sensors ---
  myGLCD.setColor(255, 255, 255);
  // Temp 1
  myGLCD.print("Temperature 1:", 150, 150);
  myGLCD.drawRoundRect(400, 145, 470, 170);
  myGLCD.print("C", 480, 150);
  drawToggleButton(540, 145, true, "", "");

  //Temp 2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Temperature 2:", 150, 200);  
  myGLCD.drawRoundRect(400, 195, 470, 220);
  myGLCD.print("C", 480, 200);
  drawToggleButton(540, 195, true, "", "");
  //Temp 3
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Temperature 3:", 150, 250);  
  myGLCD.drawRoundRect(400, 245, 470, 270);
  myGLCD.print("C", 480, 250);
  drawToggleButton(540, 245, false, "", "");

  drawBackButton();
}
void drawDiagnostics_RTCScreen(){
  // Set full background to grey
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(BigFont);

  // Header
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("RTC", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);

  myGLCD.setBackColor(60, 60, 60);

  myGLCD.print("RTC Status:", 150, 150);
  drawToggleButton(410, 145, true, "", ""); // working / not working

  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Current Date:", 150, 200);
  myGLCD.drawRect(410, 195, 600, 225); // Date display box
  drawBoxCenteredText("", 410, 195, 600, 225);

  myGLCD.print("Current Time:", 150, 250);
  myGLCD.drawRect(410, 245, 600, 275); // Time display box
  drawBoxCenteredText("", 410, 245, 600, 275);
  myGLCD.print("(24-hr)", 620, 250);

  drawBackButton();
}
void drawDiagnostics_RelaysScreen(){
  // Set full background to grey
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(BigFont);

  // Header
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("RELAYS", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);

  myGLCD.setColor(255, 255, 255);
  // Heating Rods Relay
  myGLCD.print("Heating Rods Relay:", 100, 150);
  //showing status 
  drawToggleButton(420, 145, true, "", "");
  //test input
  drawToggleButton(600, 145, true, "ON", "OFF");
  
  //Water Pump Relay
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Water Pump Relay:", 100, 200);  
  //showing status
  drawToggleButton(420, 195, false, "", "");
  //test input
  drawToggleButton(600, 195, false, "ON", "OFF");

  drawBackButton();
}
void drawDiagnostics_AgitatorScreen(){
  // Set full background to grey
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(BigFont);

  // Header
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("AGITATOR MOTOR", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);

   myGLCD.setColor(255, 255, 255);
  // Agitator status
  myGLCD.print("Motor status:", 100, 150);
      //showing status 
  drawToggleButton(350, 145, true, "ON", "OFF");

  //--- Stir Direction ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Stir Direction:", 100, 200);  
  drawToggleButton(350, 195, true, "CW", "CCW");//just to display
      //test input
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Test:", 520, 200);
  drawToggleButton(600, 195, true, "CW", "CCW");

  //--- Stir Speed Holding ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Speed (RPM):", 100, 250);  
  myGLCD.drawRoundRect(350, 245, 430, 275);
  myGLCD.print("rpm", 500, 250);

  drawBackButton();
}
void drawDiagnostics_ValvesScreen(){
  // Set full background to grey
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(BigFont);

  // Header 
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("VALVES", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);

  myGLCD.setColor(255, 255, 255);
  // Water Drain valve
  myGLCD.print("Water Drain Valve:", 100, 150);
  //showing status 
  drawToggleButton(420, 145, true, "", "");
  //test input
  drawToggleButton(600, 145, true, "ON", "OFF");
  
  //Water Refill valve
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Water Refill Valve:", 100, 200);  
  //showing status
  drawToggleButton(420, 195, false, "", "");
  //test input
  drawToggleButton(600, 195, false, "ON", "OFF");

  //Milk Refill valve
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Milk Refill Valve:", 100, 250);  
  //showing status
  drawToggleButton(420, 245, false, "", "");
  //test input
  drawToggleButton(600, 245, false, "ON", "OFF");

  drawBackButton();
}
void drawDiagnostics_SDcardScreen(){
  // Set full background to grey
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(BigFont);

  // Header
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("SD CARD", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);

  myGLCD.setColor(255, 255, 255);
  // SD card status
  myGLCD.print("SD card Status:", 150, 150);
      //showing status 
  drawToggleButton(420, 145, isSDInitialized() , "", ""); //is sd card inserted?

  //Used space
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Used Space:", 150, 200);
  //percentage bar 
  drawHorizontalBar(350, 200, 200, 20, 2 , VGA_BLUE);
  //filledSpace_SD()
  myGLCD.setFont(BigFont);
  //No. of files
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Number of Files:", 150, 250);
      // NO. OF FILES WILL be shown here
  myGLCD.print("15", 430, 250); // in future will connect here


  // --- Read Data Button --- 
  myGLCD.setColor(255, 102, 102);  // red
  myGLCD.fillRoundRect(200, 330, 300, 380);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 102, 102); // Text background also red
  myGLCD.drawRoundRect(200, 330, 300, 380);
  drawBoxCenteredText("Read", 200, 330, 300, 380);

  // --- Write Data Button --- 
  myGLCD.setColor(0, 102, 204);  // blue
  myGLCD.fillRoundRect(500, 330, 600, 380);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204); // Text background also blue
  myGLCD.drawRoundRect(500, 330, 600, 380);
  drawBoxCenteredText("Write", 500, 330, 600, 380);

  drawBackButton();
}
//---SI --> View Logs screen --> Log Viewing screen
void drawViewLogs_ViewScreen(){
  // Set full background to grey
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(BigFont);

  // Header
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("THE FILTERED LOGS", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);
  //FILTERED LOG
  filteredLogsByDateAndType();

  drawBackButton();
  
}
//====== Run Screens =========
void drawRun_PasteurizationScreen(){
  // Set full background to grey
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(BigFont);

  // Header
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("PASTEURIZATION - Run Screen", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);

  // --- Live data of Milk temperature sensor ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Current Milk Temp 1:", 10, 80);
  myGLCD.drawRoundRect(350, 75, 420, 100);
  myGLCD.print("C", 430, 80);

  //Temp Holding timer 
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Holding time Elapsed:", 10, 120);  
  myGLCD.drawRoundRect(350, 115, 420, 140);
  myGLCD.print("min", 430, 120);

  //--- Stir Motor Status ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Stir Motor:", 10, 160); 
  //bool motorStatus = getMotorStatus(); // in future reads from the vfd
  bool motorStatus = true; // for now just to show
  drawToggleButton(350, 155, motorStatus, "ON", "OFF");

  //--- Stir Direction ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Stirring Direction:", 10, 200);  
  //bool directionrRotation = readMotorDirection(); // true if CW, false if CCW
  bool directionRotation = false;
  drawToggleButton(350, 195, directionRotation, "CW", "CCW");

  // --- Live data of Water temperature sensor 1---
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Current Water Temp 2:", 10, 240);
  myGLCD.drawRoundRect(350, 235, 420, 260);
  myGLCD.print("C", 430, 240);

  // --- Live data of Water temperature sensor 2---
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Current Water Temp 3:", 10, 280);
  myGLCD.drawRoundRect(350, 275, 420, 300);
  myGLCD.print("C", 430, 280);

  // --- Target Milk temperature sensor ---
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Target Heat Temp.:", 10, 320);
  myGLCD.print(pasteurizationProgram.targetTemp, 360, 320);
  myGLCD.drawRoundRect(350, 315, 420, 340);
  myGLCD.print("C", 430, 320);

  // ----- Pause Button ----------
  drawPauseButton();
  //------ Resume button -------
  drawResumeButton();
  //------ Stop button -------
  drawStopButton(620, 350, 770, 450);
  //------ back button ------
  drawBackButton();
  myGLCD.print("Back", 70, 18);

}
void drawRun_CheeseMakingScreen(){
  // Set full background to grey
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(BigFont);

  // Header
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("CHEESE MAKING - Run Screen", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);
  // --- Live data of Milk temperature sensor ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Current Temperature:", 10, 80);
  myGLCD.drawRoundRect(350, 75, 420, 100);
  myGLCD.print("C", 430, 80);

  //Temp Holding timer 
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Holding time Elapsed:", 10, 120);  
  myGLCD.drawRoundRect(350, 115, 420, 140);
  myGLCD.print("min", 430, 120);

  //--- Stir Motor Status ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Stir Motor:", 10, 160); 
  //bool motorStatus = getMotorStatus(); // in future reads from the vfd
  bool motorStatus = true; // for now just to show
  drawToggleButton(350, 155, motorStatus, "ON", "OFF");

  //--- Stir Direction ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Stirring Direction:", 10, 200);  
  //bool directionrRotation = readMotorDirection(); // true if CW, false if CCW
  bool directionRotation = false;
  drawToggleButton(350, 195, directionRotation, "CW", "CCW");

  // ----- Pause Button ----------
  drawPauseButton();
  //------ Resume button -------
  drawResumeButton();
  //------ Stop button -------
  drawStopButton(620, 350, 770, 450);
  //------ back button ------
  drawBackButton();
  myGLCD.print("Back", 70, 18);
}
void drawRun_CoolingScreen(){
  // Set full background to grey
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(BigFont);

  // Header
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("COOLING - Run Screen", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);

  // --- Live data of Milk temperature sensor ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Current Temperature:", 10, 80);
  myGLCD.drawRoundRect(350, 75, 420, 100);
  myGLCD.print("C", 430, 80);

  // ----- Pause Button ----------
  drawPauseButton();
  //------ Resume button -------
  drawResumeButton();
  //------ Stop button -------
  drawStopButton(620, 350, 770, 450);
  //------ back button ------
  drawBackButton();
  myGLCD.print("Back", 70, 18);
}
void drawRun_CheeseDrainageScreen(){
  // Set full background to grey
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(BigFont);

  // Header
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("CHEESE DRAINAGE - Run Screen", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);

  //--- Cheese Drain Valve status ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Cheese Drain Valve:", 10, 80);  
  //bool valveCheeseDrain = readValveCheeseDrain(); // true if cheese drain valve open, false if closed
  bool valveCheeseDrain = false;
  drawToggleButton(350, 75, valveCheeseDrain, "ON", "OFF");
  // ----- Product Level in vat ---------
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Product Level:", 10, 120);  
  drawLevelBar(350, 120, 100, 150, 89, VGA_CHEESE); // x, y , width ,height , current percentage(from the level sensor)
  // ----- Pause Button ----------
  drawPauseButton();
  //------ Resume button -------
  drawResumeButton();
  //------ Stop button -------
  drawStopButton(620, 350, 770, 450);
  //------ back button ------
  drawBackButton();
  myGLCD.print("Back", 70, 18);
}
void drawRun_WaterDrainageScreen(){
  // Set full background to grey
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(BigFont);

  // Header
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("WATER DRAINAGE - Run Screen", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);
  
  //--- Water Drain Valve status ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Water Drain Valve:", 10, 80);  
  //bool valveWaterDrain = readValveWaterDrain(); // true if cheese drain valve open, false if closed
  bool valveWaterDrain = false;
  drawToggleButton(350, 75, valveWaterDrain, "ON", "OFF");
  // ----- Water Level ---------
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Water Level:", 10, 120);  
  drawLevelBar(350, 120, 100, 150, 69, VGA_AQUA); // x, y , width ,height , current percentage(from the level sensor)
  // ----- Pause Button ----------
  drawPauseButton();
  //------ Resume button -------
  drawResumeButton();
  //------ Stop button -------
  drawStopButton(620, 350, 770, 450);
  //------ back button ------
  drawBackButton();
  myGLCD.print("Back", 70, 18);
}
void drawRun_WaterRefillScreen(){
  // Set full background to grey
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(BigFont);

  // Header
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("WATER REFILL - Run Screen", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);

  //--- Water Inlet Valve status ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Water Inlet Valve:", 10, 80);  
  //bool valveWaterInlet = readValveWaterInlet(); // true if cheese drain valve open, false if closed
  bool valveWaterInlet = false;
  drawToggleButton(350, 75, valveWaterInlet, "ON", "OFF");
  // ----- Water Level ---------
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Water Level:", 10, 120);  
  drawLevelBar(350, 120, 100, 150, 69, VGA_AQUA); // x, y , width ,height , current percentage(from the level sensor)

  // ----- Pause Button ----------
  drawPauseButton();
  //------ Resume button -------
  drawResumeButton();
  //------ Stop button -------
  drawStopButton(620, 350, 770, 450);
  //------ back button ------
  drawBackButton();
  myGLCD.print("Back", 70, 18);
}
void drawRun_MilkRefillScreen(){
  // Set full background to grey
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(BigFont);

  // Header
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("MILK REFILL - Run Screen", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);

  //--- Milk inlet Valve status ---
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Milk Inlet Valve:", 10, 80);  
  //bool valveMilkInlet = readvalveMilkInlet(); // true if cheese drain valve open, false if closed
  bool valveMilkInlet = false;
  drawToggleButton(350, 75, valveMilkInlet, "ON", "OFF");
  // ----- Product Level in vat ---------
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Product Level:", 10, 120);  
  drawLevelBar(350, 120, 100, 150, 89, VGA_CHEESE); // x, y , width ,height , current percentage(from the level sensor)

  // ----- Pause Button ----------
  drawPauseButton();
  //------ Resume button -------
  drawResumeButton();
  //------ Stop button -------
  drawStopButton(620, 350, 770, 450);
  //------ back button ------
  drawBackButton();
  myGLCD.print("Back", 70, 18);
}
//======================================
void drawAS_GenericScreen(){

  // Labels and input boxes
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Start Date:", 10, 80);
  myGLCD.drawRect(200, 75, 440, 105); // Date input box
  drawBoxCenteredText("", 200, 75, 440, 105);

  myGLCD.print("Start Time:", 10, 130);
  myGLCD.drawRect(200, 125, 330, 155); // Time input box
  drawBoxCenteredText("", 200, 125, 330, 155);
  myGLCD.print("(24-hr)", 340, 130);

  // Repeat options
  myGLCD.print("Repeat:", 10, 190);
  
  // Once checkbox
  myGLCD.drawRect(130, 185, 150, 205);
  myGLCD.print("Once", 160, 190);

  // Daily checkbox
  myGLCD.drawRect(130, 225, 150, 245);
  myGLCD.print("Daily", 160, 225);

  // Weekly checkbox
  myGLCD.drawRect(130, 265, 150, 285);
  myGLCD.print("Weekly", 160, 270);

  // --- save Data Button --- 
  myGLCD.setFont(BigFont);
  myGLCD.setColor(255, 102, 102);  // red
  myGLCD.fillRoundRect(180, 360, 280, 410);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 102, 102); // Text background also red
  myGLCD.drawRoundRect(180, 360, 280, 410);
  drawBoxCenteredText("Save", 180, 360, 280, 410);

  // line
  myGLCD.setColor(100, 100, 100);
  myGLCD.drawLine(10, 320, 440, 320); //horizontal
  myGLCD.drawLine(450, 60, 450, 440); //vertical

  // --- Back button ---
  drawBackButton();
  myGLCD.print("Back", 70, 18);
}
void drawKeypadSec(){
  ///Key Pad Section
  myGLCD.setFont(BigFont);
  // Title / Label
  myGLCD.setColor(0, 120, 255);
  myGLCD.drawRoundRect(460, 75, 770, 110);
  myGLCD.fillRoundRect(460, 75, 770, 110);
  myGLCD.setBackColor(0, 120, 255);
  myGLCD.setColor(255, 255, 255);
  drawBoxCenteredText("Keypad",460, 75, 770, 110);

  //ROW 1
  // button 1
  myGLCD.setColor(160, 160, 160);
  myGLCD.drawRoundRect(470, 120, 560, 160);
  myGLCD.fillRoundRect(470, 120, 560, 160);
  myGLCD.setBackColor(160, 160, 160);
  myGLCD.setColor(255, 255, 255);
  drawBoxCenteredText("1",470, 120, 560, 160);
  // button 2
  myGLCD.setColor(160, 160, 160);
  myGLCD.drawRoundRect(570, 120, 660, 160);
  myGLCD.fillRoundRect(570, 120, 660, 160);
  myGLCD.setBackColor(160, 160, 160);
  myGLCD.setColor(255, 255, 255);
  drawBoxCenteredText("2",570, 120, 660, 160);
  // button 3
  myGLCD.setColor(160, 160, 160);
  myGLCD.drawRoundRect(670, 120, 760, 160);
  myGLCD.fillRoundRect(670, 120, 760, 160);
  myGLCD.setBackColor(160, 160, 160);
  myGLCD.setColor(255, 255, 255);
  drawBoxCenteredText("3",670, 120, 760, 160);
  //ROW 2
  // button 4
  myGLCD.setColor(160, 160, 160);
  myGLCD.drawRoundRect(470, 170, 560, 210);
  myGLCD.fillRoundRect(470, 170, 560, 210);
  myGLCD.setBackColor(160, 160, 160);
  myGLCD.setColor(255, 255, 255);
  drawBoxCenteredText("4", 470, 170, 560, 210);
  // button 5
  myGLCD.setColor(160, 160, 160);
  myGLCD.drawRoundRect(570, 170, 660, 210);
  myGLCD.fillRoundRect(570, 170, 660, 210);
  myGLCD.setBackColor(160, 160, 160);
  myGLCD.setColor(255, 255, 255);
  drawBoxCenteredText("5", 570, 170, 660, 210);
  // button 6
  myGLCD.setColor(160, 160, 160);
  myGLCD.drawRoundRect(670, 170, 760, 210);
  myGLCD.fillRoundRect(670, 170, 760, 210);
  myGLCD.setBackColor(160, 160, 160);
  myGLCD.setColor(255, 255, 255);
  drawBoxCenteredText("6", 670, 170, 760, 210);
  //ROW 3
  // button 7
  myGLCD.setColor(160, 160, 160);
  myGLCD.drawRoundRect(470, 220, 560, 260);
  myGLCD.fillRoundRect(470, 220, 560, 260);
  myGLCD.setBackColor(160, 160, 160);
  myGLCD.setColor(255, 255, 255);
  drawBoxCenteredText("7", 470, 220, 560, 260);
  // button 8
  myGLCD.setColor(160, 160, 160);
  myGLCD.drawRoundRect(570, 220, 660, 260);
  myGLCD.fillRoundRect(570, 220, 660, 260);
  myGLCD.setBackColor(160, 160, 160);
  myGLCD.setColor(255, 255, 255);
  drawBoxCenteredText("8", 570, 220, 660, 260);
  // button 9
  myGLCD.setColor(160, 160, 160);
  myGLCD.drawRoundRect(670, 220, 760, 260);
  myGLCD.fillRoundRect(670, 220, 760, 260);
  myGLCD.setBackColor(160, 160, 160);
  myGLCD.setColor(255, 255, 255);
  drawBoxCenteredText("9", 670, 220, 760, 260);
  //ROW 4
  // button DEL
  myGLCD.setColor(255, 51, 51);
  myGLCD.drawRoundRect(470, 270, 560, 310);
  myGLCD.fillRoundRect(470, 270, 560, 310);
  myGLCD.setBackColor(255, 51, 51);
  myGLCD.setColor(255, 255, 255);
  drawBoxCenteredText("DEL", 470, 270, 560, 310);
  // button 0
  myGLCD.setColor(160, 160, 160);
  myGLCD.drawRoundRect(570, 270, 660, 310);
  myGLCD.fillRoundRect(570, 270, 660, 310);
  myGLCD.setBackColor(160, 160, 160);
  myGLCD.setColor(255, 255, 255);
  drawBoxCenteredText("0", 570, 270, 660, 310);
  // button OK
  myGLCD.setColor(51, 153, 255);
  myGLCD.drawRoundRect(670, 270, 760, 310);
  myGLCD.fillRoundRect(670, 270, 760, 310);
  myGLCD.setBackColor(51, 153, 255);
  myGLCD.setColor(255, 255, 255);
  drawBoxCenteredText("OK", 670, 270, 760, 310);

  //BOX AROUND KEYPAD
  myGLCD.drawRoundRect(460, 75, 770, 320);

}
void drawBackButton(){
  // --- Back button ---
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setColor(0, 102, 204);
  myGLCD.fillRoundRect (10, 10, 60, 36);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (10, 10, 60, 36);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(0, 102, 204); //blue
  myGLCD.print("<-", 18, 15);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.setFont(SmallFont);
}
void drawBoxCenteredText(const char* label, int x1, int y1, int x2, int y2) {
  int boxWidth = x2 - x1;
  int boxHeight = y2 - y1;
  int textWidth = strlen(label) * 16;   // Assuming 16 pixels per character (SmallFont)  ///can make nanother function for small font 8x16
  int textHeight = 16;                 // Assuming 16 pixels font height

  int textX = x1 + (boxWidth - textWidth) / 2;
  int textY = y1 + (boxHeight - textHeight) / 2;

  myGLCD.print(label, textX, textY); 
  ///just use this function >>>>>  drawBoxCenteredText(const char* label, int x1, int y1, int x2, int y2)
  // for long texts divide it into two lines . divide the height of box in two and for first line add 20 in y1 and for second line add 20 in y2. 
}
void drawBoxCenteredText(float value, int x1, int y1, int x2, int y2) {
  char buf[16];
  sprintf(buf, "%.2f", value);  // No width padding

  // Measure width of actual content
  int textWidth = strlen(buf) * myGLCD.getFontXsize();
  int boxWidth = x2 - x1;
  int textX = x1 + (boxWidth - textWidth) / 2;
  int textY = y1 + ((y2 - y1) - myGLCD.getFontYsize()) / 2;

  // Draw background box (optional)
  myGLCD.setColor(60, 60, 60);
  myGLCD.fillRect(x1, y1, x2, y2);

  // Draw centered text
  myGLCD.setColor(255, 255, 255);
  myGLCD.print(buf, textX, textY);
}
void drawRunButton(int x1, int y1, int x2, int y2) {
  myGLCD.setFont(BigFont);
  myGLCD.setColor(2, 140, 32);  // dark green
  myGLCD.fillRoundRect(x1, y1, x2, y2);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(2, 140, 32);
  myGLCD.drawRoundRect(x1, y1, x2, y2);
  drawBoxCenteredText("RUN", x1, y1, x2, y2);
}
void drawPauseButton(){
  // --- PAUSE Button ---
  myGLCD.setFont(BigFont);
  myGLCD.setColor(100, 100, 255);  // dark green
  myGLCD.fillRoundRect(620, 110, 770, 210);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(100, 100, 255); // Text background also green
  myGLCD.drawRoundRect(620, 110, 770, 210);
  drawBoxCenteredText("PAUSE", 620, 110, 770, 210);
}
void drawResumeButton(){
  // --- resume Button ---
  myGLCD.setFont(BigFont); 
  myGLCD.setColor(2, 140, 32);  // green
  myGLCD.fillRoundRect(620, 230, 770, 330);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(2, 140, 32); // Text background also green
  myGLCD.drawRoundRect(620, 230, 770, 330);
  drawBoxCenteredText("Resume", 620, 230, 770, 330);
}

// Function to draw a STOP button used in the manual and auto-procees mode
void drawStopButton(int x1, int y1, int x2, int y2) {
  myGLCD.setFont(BigFont);
  myGLCD.setColor(200, 0, 0);  // red
  myGLCD.fillRoundRect(x1, y1, x2, y2);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(200, 0, 0);
  myGLCD.drawRoundRect(x1, y1, x2, y2);
  drawBoxCenteredText("STOP", x1, y1, x2, y2);
}

// Function to draw a toggle button
void drawToggleButton(int x, int y, bool state, const char* trueLabel, const char* falseLabel) {
  int halfWidth = 100 / 2;

  // Colors
  word trueColor = state ? VGA_GREEN : VGA_GRAY;
  word falseColor = state ? VGA_GRAY : VGA_RED;

  // Draw left (TRUE side)
  myGLCD.setColor(trueColor);
  myGLCD.fillRect(x, y, x + halfWidth - 3, y + 30);
  myGLCD.setColor(VGA_WHITE);
  myGLCD.setBackColor(trueColor);
  myGLCD.setFont(BigFont);
  drawBoxCenteredText(trueLabel, x, y, x + halfWidth - 3, y + 30);

  // Draw right (FALSE side)
  myGLCD.setColor(falseColor);
  myGLCD.fillRect(x + halfWidth, y, x + 100, y + 30);
  myGLCD.setColor(VGA_WHITE);
  myGLCD.setBackColor(falseColor);
  myGLCD.setFont(BigFont);
  drawBoxCenteredText(falseLabel,x + halfWidth, y, x + 100, y + 30);

  // border around entire toggle
  myGLCD.setColor(VGA_WHITE);
  myGLCD.drawRect(x, y, x + 100, y + 30);
}

//common screen to view live data
void drawLiveDataScreen(){
  myGLCD.fillScr( 60, 60, 60);
  myGLCD.setBackColor(60, 60, 60);
  // Title
  myGLCD.setFont(BigFont);
  myGLCD.setColor(255, 255, 255);  // White text
  myGLCD.print(" Live Data ", CENTER, 30);
  myGLCD.drawLine(10, 50, 790, 50);

  // --- Live data of temperature sensors ---
  myGLCD.setColor(255, 255, 255);
  // Temp 1
  myGLCD.print("Temperature 1:", 10, 80);
  
  myGLCD.drawRoundRect(300, 75, 370, 100);
  myGLCD.print("C", 380, 80);
      // Graph Button
  myGLCD.setColor(255, 102, 102);  // red
  myGLCD.fillRoundRect(420, 75, 520, 100);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 102, 102); // Text background also red
  myGLCD.drawRoundRect(420, 75, 520, 100);
  drawBoxCenteredText("Graph", 420, 75, 520, 100);

  //Temp 2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Temperature 2:", 10, 120);  
  myGLCD.drawRoundRect(300, 115, 370, 140);
  myGLCD.print("C", 380, 120);
      // Graph Button
  myGLCD.setColor(255, 102, 102);  // red
  myGLCD.fillRoundRect(420, 115, 520, 140);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 102, 102); // Text background also red
  myGLCD.drawRoundRect(420, 115, 520, 140);
  drawBoxCenteredText("Graph", 420, 115, 520, 140);

  //Temp 3
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Temperature 3:", 10, 160);  
  myGLCD.drawRoundRect(300, 155, 370, 180);
  myGLCD.print("C", 380, 160);
      // Graph Button
  myGLCD.setColor(255, 102, 102);  // red
  myGLCD.fillRoundRect(420, 155, 520, 180);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 102, 102); // Text background also red
  myGLCD.drawRoundRect(420, 155, 520, 180);
  drawBoxCenteredText("Graph", 420, 155, 520, 180);

  //Temp Holding timer 
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Temp. Hold Timer:", 10, 200);  
  myGLCD.drawRoundRect(300, 195, 370, 220);
  myGLCD.print("min", 380, 200);

  //Milk pH Level 
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Milk pH Level:", 10, 240);  
  myGLCD.drawRoundRect(300, 235, 370, 260);
  myGLCD.print("pH", 380, 240);
      // Graph Button
  myGLCD.setColor(255, 102, 102);  // red
  myGLCD.fillRoundRect(420, 235, 520, 260);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 102, 102); // Text background also red
  myGLCD.drawRoundRect(420, 235, 520, 260);
  drawBoxCenteredText("Graph", 420, 235, 520, 260);

  //Flow Rate 
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Flow Rate (L/min):", 10, 280);  
  myGLCD.drawRoundRect(300, 275, 370, 300);
    // Graph Button
  myGLCD.setColor(255, 102, 102);  // red
  myGLCD.fillRoundRect(420, 275, 520, 300);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 102, 102); // Text background also red
  myGLCD.drawRoundRect(420, 275, 520, 300);
  drawBoxCenteredText("Graph", 420, 275, 520, 300);

  //Milk Filled 
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Milk Filled:", 10, 320);  
  myGLCD.drawRoundRect(300, 315, 370, 340);
  myGLCD.print("ltr", 380, 320);

  //Agitator Motor speed
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Motor Speed (rpm):", 10, 360);  
  myGLCD.drawRoundRect(300, 355, 370, 380);
    // Graph Button
  myGLCD.setColor(255, 102, 102);  // red
  myGLCD.fillRoundRect(420, 355, 520, 385);  // x1, y1, x2, y2
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 102, 102); // Text background also red
  myGLCD.drawRoundRect(420, 355, 520, 385);
  drawBoxCenteredText("Graph", 420, 355, 520, 385);

  //date and time in the gui_fsm 
  
  //vertical line
  myGLCD.drawLine(530, 50, 530, 450);

  //Water drain valve
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Drain Valve", 550, 80);  
    // Draw status circle
  bool drainValveOn = true;  // Will replace with your real logic
  if (drainValveOn) { myGLCD.setColor(0, 255, 0); }   // Green
  else { myGLCD.setColor(255, 0, 0);  }   // Red
  myGLCD.fillCircle(760, 85, 10);

  //Water Refill valve
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Refill Valve", 550, 120);  
    // Draw status circle
  bool refillValveOn = false;  // Will replace with your real logic
  if (refillValveOn) { myGLCD.setColor(0, 255, 0);  } // Green
  else { myGLCD.setColor(255, 0, 0);  } // Red
  myGLCD.fillCircle(760, 125, 10);

  //Pump Status
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Water Pump", 550, 160);  
    // Draw status circle
  bool pumpOn = false;  // Will replace with your real logic
  if (pumpOn) { myGLCD.setColor(0, 255, 0);  } // Green
  else { myGLCD.setColor(255, 0, 0);  } // Red
  myGLCD.fillCircle(760, 165, 10);

  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(60, 60, 60);
  myGLCD.print("Water", 560, 275);
  myGLCD.print("Product", 670, 275);
  drawLevelBar(560, 300, 100, 150, 88, VGA_AQUA); // x, y , width ,height , current percentage(from the level sensor), color
  drawLevelBar(670, 300, 100, 150, 11, VGA_CHEESE); // x, y , width ,height , current percentage(from the level sensor), color
  // --- Back button ---
  drawBackButton();
  myGLCD.print("Back", 70, 18);
}

//draws live float by first converting it into char string
void drawFloatLive(float value, int x1, int y1, int x2, int y2) {
  // Clear the area
  myGLCD.setColor(60, 60, 60); // background color
  myGLCD.fillRect(x1, y1, x2, y2);

  // Format the float
  char buffer[10];
  sprintf(buffer, "%.2f", value);

  // Draw text
  myGLCD.setColor(255, 255, 255); // text color
  myGLCD.setFont(BigFont);
  drawBoxCenteredText(buffer, x1, y1, x2, y2);
  delay(20);
}
void drawLiveDate(int x1, int y1, int x2, int y2){
  // date from RTC
   rtc.refresh();  // <<< MUST call this to update time and date
  
  int day = rtc.day();
  int month = rtc.month();
  int year = rtc.year() % 100;  //to print twi digits

  // Format date as a single string
  char buffer[25];
  sprintf(buffer, "%02d/%02d/%02d", day, month, year);

  // Clear the area
  myGLCD.setColor(60, 60, 60); // background color
  myGLCD.fillRect(x1, y1, x2, y2);

  // Draw the formatted string
  myGLCD.setColor(255, 255, 255); // text color
  myGLCD.setFont(BigFont);      // You can change font if needed
  drawBoxCenteredText(buffer, x1, y1, x2, y2);

  delay(200);  // Refresh interval
}
void drawLiveTime(int x1, int y1, int x2, int y2){
  //time from RTC
   rtc.refresh();  // <<< MUST call this to update time and date
  
  int hour = rtc.hour();
  int minute = rtc.minute();
  int second = rtc.second();

  // Format time as a single string
  char buffer[25];
  sprintf(buffer, "%02d:%02d:%02d", hour, minute, second);

  // Clear the area
  myGLCD.setColor(60, 60, 60); // background color
  myGLCD.fillRect(x1, y1, x2, y2);

  // Draw the formatted string
  myGLCD.setColor(255, 255, 255); // text color
  myGLCD.setFont(BigFont);      // You can change font if needed
  drawBoxCenteredText(buffer, x1, y1, x2, y2);

  delay(200);  // Refresh interval
}


// Vertiacal level bar
void drawLevelBar(int x, int y, int width, int height, int percentage, unsigned int color) {
  // Clamp percentage between 0 and 100
  if (percentage < 0) percentage = 0;
  if (percentage > 100) percentage = 100;

  // Draw outer frame
  myGLCD.setColor(VGA_WHITE);
  myGLCD.drawRect(x, y, x + width, y + height);

  // Background fill
  myGLCD.setColor(VGA_GRAY);
  myGLCD.fillRect(x + 1, y + 1, x + width - 1, y + height - 1);

  // Calculate fill height (bottom-up)
  int fillHeight = (height - 2) * percentage / 100;
  int fillY = y + height - 1 - fillHeight;

  // Fill bar
  myGLCD.setColor(color);
  myGLCD.fillRect(x + 1, fillY, x + width - 1, y + height - 2);

  // Display percentage value
  myGLCD.setBackColor(VGA_BLACK);
  myGLCD.setColor(VGA_WHITE);
  myGLCD.setFont(SmallFont);

  char label[10];
  sprintf(label, "%d%%", percentage);
  myGLCD.print(label, x + (width)/2 - 12 , y + height / 2 - 8);
}
//horizontal progress/level bar
void drawHorizontalBar(int x, int y, int width, int height, int percentage, unsigned int color) {
  // Clamp percentage between 0 and 100
  if (percentage < 0) percentage = 0;
  if (percentage > 100) percentage = 100;

  // Draw outer frame
  myGLCD.setColor(VGA_WHITE);
  myGLCD.drawRect(x, y, x + width, y + height);

  // Background fill
  myGLCD.setColor(VGA_GRAY);
  myGLCD.fillRect(x + 1, y + 1, x + width - 1, y + height - 1);

  // Calculate fill width (left to right)
  int fillWidth = (width - 2) * percentage / 100;

  // Fill bar horizontally
  if (fillWidth > 0) {
    myGLCD.setColor(color);
    myGLCD.fillRect(x + 1, y + 1, x + fillWidth, y + height - 1);
  }

  // Display percentage value
  myGLCD.setBackColor(VGA_BLACK);
  myGLCD.setColor(VGA_WHITE);
  myGLCD.setFont(SmallFont);

  char label[10];
  sprintf(label, "%d%%", percentage);
  myGLCD.print(label, x + width/2 - 12, y + height/2 - 8);
}

// Highlights the button when pressed
void drawFrame(int x1, int y1, int x2, int y2) {
  myGLCD.setColor(255, 0, 0);
  myGLCD.drawRoundRect (x1, y1, x2, y2);
  while (myTouch.dataAvailable())
    myTouch.read();
    myGLCD.setColor(255, 255, 255);
    myGLCD.drawRoundRect (x1, y1, x2, y2);
}

void drawCSV() {
  static unsigned long lastLogTime = 0;
  const unsigned long logInterval = 200; 

  unsigned long currentMillis = millis();
  if (currentMillis - lastLogTime < logInterval) return;
  lastLogTime = currentMillis;

  if (!SD.exists("log.csv")) {
    myGLCD.print("log.csv not found!", 10, 50);
    return;
  }

  File logFile = SD.open("log.csv");
  if (!logFile) {
    myGLCD.print("Error opening log.csv", 10, 50);
    return;
  }

  // === Read entire file and keep only last N lines ===
  const int MAX_VISIBLE_LINES = 20; // or however many lines fit on screen
  String lines[MAX_VISIBLE_LINES];
  int lineIndex = 0;

  while (logFile.available()) {
    String line = logFile.readStringUntil('\n');

    // Shift buffer if full
    if (lineIndex < MAX_VISIBLE_LINES) {
      lines[lineIndex++] = line;
    } else {
      for (int i = 0; i < MAX_VISIBLE_LINES - 1; i++) {
        lines[i] = lines[i + 1];
      }
      lines[MAX_VISIBLE_LINES - 1] = line;
    }
  }

  logFile.close();

  // === Display on screen ===
  myGLCD.clrScr();
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setFont(BigFont);

  int y = 40;
  for (int i = 0; i < lineIndex; i++) {
    myGLCD.print(lines[i].c_str(), 10, y);
    y += 20;
  }
}



void drawLogo() {
  myGLCD.drawBitmap(339, 440, 122, 27, (unsigned short*)logo);  // (x, y, width, height, data)
}