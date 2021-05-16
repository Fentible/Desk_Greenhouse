/**
  ******************************************************************************
  * @file    ui_elements.c
  * @author  T. Buckingham
  * @brief   User interface elements for creating the various user screens
  *
  *          The file contains ::
  *           + Two functions to draw different backgrounds
	*							- A checkered background
	*							- A flat solid colour background
	* 					+ A function to draw a top menu bar with options
	* 					+ A 'string box' function to draw pseudo buttons
  *
  ******************************************************************************
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f7xx_hal.h"              // Device header
#include "GLCD_Config.h"                // Keil.STM32756G-EVAL::Board Support:Graphic LCD
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "SystemClock.h" 								// System Clock Configuration & Event Handler
#include "Board_Touch.h"								// Touch Screen
#include "ui_elements.h"								// UI Elements
#include "info.h"												// Info Screen
#include "settings.h"										// Settings screen
#include "serial.h"											// Serial communication
#include "sensors.h"										// Sensors handler and user screen
#include "moisture.h"										// Moisture sensor definitions and variables


/*-- Global Definitions --*/
#define wait_delay HAL_Delay

#define HOME 0     /* home option for the user */
#define SETTINGS 1 /* settings option for the user */
#define HELP 2     /* help option for the user*/

/*-- External Variables --*/
extern GLCD_FONT GLCD_Font_6x8;
extern GLCD_FONT GLCD_Font_16x24;

/*-- Function Prototypes --*/

/** @brief Function to draw the solid colour flat background.
 *  @param  Void.
 *  @return Void.
 */
void draw_flat_background(void);

/** @brief Function to draw the checkered background.
 *  @param  Void.
 *  @return Void.
 */
void draw_background(void);


/** @brief Draws a string with checkboxes to the right of it. 
 *  @param  x The x position of the start of the string.
 *  @param  y The y position of the string. 
 *  @param  str The string to be drawn before the boxes.
 *  @param  boxes The number of boxes drawn after the string. 
 *  @return Void.
 */
void draw_check_option(int x, int y, char* str, int boxes);

/** @brief  Draws a pseudo button on the screen by drawing a box then drawing a string on top.
 * 					The padding uniform and adjusts to the strings length. 
 *  @param  x The x position of the start button.
 *  @param  y The y position of the start button. 
 *  @param  str The string that will be drawn in the button. 
 *  @return Void.
 */
void draw_string_box(int x, int y, char* str);

/** @brief Draws the top menu bar with the options 'Home', 'Settings' and 'Help'.
						These options sensors.c , settings.c and info.c respectively.
 *  @param  Void.
 *  @return Void.
 */
void draw_top_bar(void);


void draw_flat_background(void)
{
	
	int i = 0;
	GLCD_SetForegroundColor(BACKGROUND_BLUE);
	for(i = 0; i < GLCD_WIDTH; i++) {
		GLCD_DrawVLine(i, 0, GLCD_HEIGHT);
	}
	for(i = 0; i < GLCD_HEIGHT; i++) {
		GLCD_DrawHLine(0, i, GLCD_WIDTH);
	}

	return;
}


void draw_background(void)
{
	
	int x = 12, y = 0;
	GLCD_SetForegroundColor(GLCD_COLOR_CYAN); 
	for(x = 12; x < GLCD_WIDTH; x += 50) {
				GLCD_DrawVLine(x, 0, GLCD_HEIGHT);
	}
	for(y = 0; y < GLCD_HEIGHT; y += 50) {
				GLCD_DrawHLine(0, y, GLCD_WIDTH);
	}
	GLCD_SetForegroundColor(GLCD_COLOR_BLACK); 
	
	return;
}

void draw_check_option(int x, int y, char* str, int boxes)
{
	int i = 0;

	GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);   
	GLCD_SetForegroundColor(GLCD_COLOR_BLACK); 
	GLCD_DrawString(x, y, str); 
	GLCD_SetBackgroundColor(GLCD_COLOR_LIGHT_GREY);   
	for(i = 0; i < boxes; i++) {
		GLCD_DrawRectangle((x + 150) + (i * 34), y, 24, 24);
	}
	
	return;
}

void draw_string_box(int x, int y, char* str) 
{
	int x_offset = 10;
	int y_offset = 10;
	int height = 30;
	int width = (strlen(str) + 2) * 16; // 16 for each char (font) + 2 for padding
	int i = 0; 
	GLCD_SetForegroundColor(GLCD_COLOR_BLACK); // background box
	for(i = 0; i < height; i++) {
		GLCD_DrawHLine(x + x_offset, (y + y_offset) + i, width); 
	}
	GLCD_SetForegroundColor(FOREGROUND_BLUE); // foreground box
	for(i = 0; i < height; i++) {
		GLCD_DrawHLine(x, y + i, width); 
	}
	GLCD_SetBackgroundColor(FOREGROUND_BLUE);   
	GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
	GLCD_DrawString(x+16, y+5, str); 
	
	return;
}

void draw_top_bar(void) 
{
	int i = 0;
	//draw_background();
	draw_flat_background();
	GLCD_SetForegroundColor(FOREGROUND_BLUE);
	for(i = 0; i < 30; i++) {
		GLCD_DrawHLine(0, i, GLCD_WIDTH);
	}
	GLCD_SetBackgroundColor(FOREGROUND_BLUE);
	GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
	GLCD_SetFont(&GLCD_Font_16x24); 
	GLCD_DrawString(0, 5, "Home"); 
	GLCD_DrawString(80, 5, "Settings"); 
	GLCD_DrawString(224, 5, "Help"); 
	GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);

	return;
}

int main(void)
{
	
	HAL_Init(); /* Init Hardware Abstraction Layer */
	SystemClock_Config(); /* Config Clocks */
	
	//SER_Busy();
	/* Init GLCD */
	GLCD_Initialize(); 
	Touch_Initialize(); 
	GLCD_ClearScreen();    
	
	// Get touch
	sensors();
	
	return 0;
	
}

