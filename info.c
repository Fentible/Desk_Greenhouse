/**
  ******************************************************************************
  * @file    info.c
  * @author  T. Buckingham
  * @brief   Information screen for the Desk Green House
  *
  *          The file contains ::
  *           + Information text box displayed to the user on the 'help' screen
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
#include "ui_elements.h"								// User interface functions
#include "settings.h"										// Settings screen 
#include "sensors.h"										// Sensors (the home) screen
#include "info.h"												// Info Screen	

/*-- Global Definitions --*/
#define wait_delay HAL_Delay

/*-- External Variables --*/
extern GLCD_FONT GLCD_Font_6x8;
extern GLCD_FONT GLCD_Font_16x24;

/*--  Function Prototypes --*/

/** @brief Determines is a string box, or button, has been pressed by the user.
 *  @param x The x coordinate where the user has pressed.
 *  @param y The y coordinate where the user has pressed.
 *  @return Void.
 */
static signed char check_coords(int x, int y);

/** @brief Draws the 'help' screen to the user. 
 *  @param Void.
 *  @return Void.
 */
void draw_info(void);


/** @brief The control function of the help screen. 
 *  @param Void.
 *  @return Void.
 */
int info(void);



static signed char check_coords(int x, int y) 
{
	
	if(0 <= y && y <= 30) { // top bar selection options
		if(0 <= x && x <= 64) { return HOME; }
		if(80 <= x && x <= 208) { return SETTINGS; }
		if(224 <= x && x <= 288) { return HELP; }
	}

	return -1;
}

void draw_info(void)
{
	// Variables init
	unsigned char x_offset = 10;
	unsigned char y_offset = 10;
	int x = 0, x_start = 0, x_end = 0;
	unsigned char height = 70;
	
	GLCD_SetFont(&GLCD_Font_6x8); 
	GLCD_SetBackgroundColor(BACKGROUND_BLUE);
	x_start = (GLCD_WIDTH/4) - 10;
	x_end = ((GLCD_WIDTH/4) + (43 * 6)) + 10;
	// Draw background black box
	for(x = (x_start + x_offset); x < (x_end + x_offset); x++) {
			GLCD_SetForegroundColor(GLCD_COLOR_BLACK);
			GLCD_DrawVLine(x, (95 - 10) + y_offset, height); 
	}
	// Draw forefront box
	for(x = x_start; x < x_end; x++) {
			GLCD_SetForegroundColor(LIGHT_GREY);
			GLCD_DrawVLine(x, (95 - 10), height); 
		}
	GLCD_SetBackgroundColor(LIGHT_GREY);
	GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
	GLCD_DrawString(GLCD_WIDTH/4, 95, "Pump :: the pump will turn on");
	GLCD_DrawString(GLCD_WIDTH/4, 105, "when the soil is dry at 3 hour intervals");
	GLCD_DrawString(GLCD_WIDTH/4, 115, "Fan :: the fan will turn on");
	GLCD_DrawString(GLCD_WIDTH/4, 125, "when either the temperature or");
	GLCD_DrawString(GLCD_WIDTH/4, 135, "humidity is higher than the user's settings");
	
	return;
}

int info(void)
{
	
	TOUCH_STATE tsc_state;
	int pressed = 0;

	GLCD_ClearScreen();  
	draw_top_bar();
	draw_info();
	GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);
	for(;;){
		Touch_GetState(&tsc_state);
		if(tsc_state.pressed) {
			pressed = check_coords(tsc_state.x, tsc_state.y);
			if(tsc_state.pressed) {
			pressed = check_coords(tsc_state.x, tsc_state.y);
			switch(pressed) {
				case -1: 
					wait_delay(100);
				  break;
				case HOME:
						return 0;
				case HELP: 
					  wait_delay(100);
				case SETTINGS:  
					  return SETTINGS;
				}
			}
		wait_delay(100);
		}
	}
}	
