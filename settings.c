/**
  ******************************************************************************
  * @file    SystemClock.c
  * @author  Generated by the STM32CubeMX
  * @brief   Clock configuration for the stm32f746g-disco
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
#include "ui_elements.h"								// UI Elements
#include "info.h"												// Info Screen
#include "settings.h"										// Settings screen
#include "serial.h"											// Serial communication

/*-- Global Definitions --*/
#define wait_delay HAL_Delay

/*-- Local Button Options --*/

#define TEMP_UP    3	 /* decrement user option for temperature setting */
#define TEMP_DOWN  4	 /* increment user option for temperature setting */
#define HUMID_UP   5	 /* decrement user option for humidity setting    */
#define HUMID_DOWN 6	 /* increment user option for humidity setting    */
#define CONFIRM 	 7	 /* confirm option to return back to home screen  */

/*-- External Variables --*/
extern GLCD_FONT GLCD_Font_6x8;
extern GLCD_FONT GLCD_Font_16x24;

/*-- Global Variables --*/

unsigned char max_temp = 100;  			/* user max temp - 'ideal' temp. - range would be 25-30      */
// unsigned char min_moisture_level; - not used as the device currently works off of 'dry' || 'wet'
unsigned char max_humidity = 100; 		/* user max humidity - 'ideal' humid. - range would be 50-70 */


/*-- Function Prototypes --*/

/** @brief Determines is a string box, or button, has been pressed by the user.
 *  @param x The x coordinate where the user has pressed.
 *  @param y The y coordinate where the user has pressed.
 *  @return Void.
 */
static signed char check_coords(int x, int y);

/** @brief Draws the settings screen to the user.
 *  @param Void.
 *  @return Void.
 */
void draw_settings(void);

/** @brief The control function for the settings screen.
 *  @param Void.
 *  @return Void.
 */
int settings(void);


static signed char check_coords(int x, int y) 
{
	
	if(0 <= y && y <= 30) { // top bar selection options
		if(0 <= x && x <= 64) { return HOME; }
		if(80 <= x && x <= 208) { return SETTINGS; }
		if(224 <= x && x <= 288) { return HELP; }
	} else if(125 <= y && y <= (125 + 30)) { // temp
		if(285 <= x && x <= (285 + (16 * 4))) { return TEMP_DOWN; }
		if(365 <= x && x <= (365 + (16 * 6))) { return TEMP_UP; }
	} else if(150 <= y && y <= (150 + 30)) { // humid
		if(285 <= x && x <= (285 + (16 * 4))) { return HUMID_DOWN; }
		if(365 <= x && x <= (365 + (16 * 6))) { return HUMID_UP; }
	}//else if(225 <= y && y <= (225 + 30))  { // confirm button
		//if(0 <= x && x <= 0 + (9 * 16)) { return HOME; }
	//} 

	return -1;
}

void draw_settings(void)
{
	char str[3];
	GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);   
	GLCD_SetForegroundColor(GLCD_COLOR_WHITE);   
	GLCD_SetFont(&GLCD_Font_16x24); 
	//draw_string_box(25, 225, "Confirm");
	GLCD_SetBackgroundColor(BACKGROUND_BLUE);  
	GLCD_DrawString(50, 125, "Temp");
	GLCD_DrawString(50, 150, "Humidity");
	draw_string_box(125 + (10*16), 125, "Down");
	draw_string_box(125 + (15*16), 125, "Up");
	draw_string_box(125 + (10*16), 150, "Down");
	draw_string_box(125 + (15*16), 150, "Up");
	GLCD_SetBackgroundColor(BACKGROUND_BLUE);
	sprintf(str, "%d", max_temp);
	GLCD_DrawString(200, 125, str);
	sprintf(str, "%d", max_humidity);
	GLCD_DrawString(200, 150, str);
	GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);
	return;
}

int settings(void)
{
	
	TOUCH_STATE tsc_state;
	int pressed = 0;
	GLCD_ClearScreen();  
	draw_top_bar();
	GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);
	draw_settings();
	for(;;){
		Touch_GetState(&tsc_state);
		if(tsc_state.pressed) {
			pressed = check_coords(tsc_state.x, tsc_state.y);
			/*
			 * Going to another screen i.e. 'info' to 'settings' returns the function to the main screen (sensors)
			 * with a return number, which is used by 'sensors' to then used to open the next screen - limiting the amount of functions open at once
			 */
			switch(pressed) {
				case HOME: 
					return HOME;
				case HELP: 
					return HELP;
				case SETTINGS:  
					wait_delay(100);
				case TEMP_UP:
					max_temp++;
					GLCD_ClearScreen();  
					draw_top_bar();
					GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);
					draw_settings();
					break;
				case TEMP_DOWN:
					max_temp--;
					GLCD_ClearScreen();  
					draw_top_bar();
					GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);
					draw_settings();
					break;
				case HUMID_UP: 
					max_humidity++;
					GLCD_ClearScreen();  
					draw_top_bar();
					GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);
					draw_settings();
					break;
				case HUMID_DOWN:
					max_humidity--;
					GLCD_ClearScreen();  
					draw_top_bar();
					GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);
					draw_settings();
					break;
				case -1: 
					wait_delay(100);
			}
			wait_delay(100);
		}
	}
}
