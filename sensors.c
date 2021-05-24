/**
  ******************************************************************************
  * @file    sensors.c
  * @author  T. Buckingham
  * @brief   Sensor screen displayed to the user and output sensor handler
  *
  *          The file contains ::
  *           + Functions for displaying the current input readigns to the user
	*						+ A handler function to handle turning the pump and fans on or off
	* 					+ Local definitions 
	*							- Button options
	*							- Pump setting flags
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
#include "ui_elements.h"								// User Interface Elements
#include "info.h"												// Info Screen
#include "settings.h"										// Settings screen
#include "serial.h"											// Serial communication
#include "sensors.h"										// Sensors handler and user screen
#include "fan.h"												// Fan functions and definitions
#include "moisture.h"										// Moisture sensor definitions and variables
#include "pump.h"												// Pump functions and definitions

/*-- Global Definitions --*/
#define wait_delay HAL_Delay

/*-- Local Button Options --*/

#define TEMP  3	 /* button option for the temperature box */
#define HUMID 4	 /* button option for the humidity box    */
#define MOIST 5  /* button option for the moisture box    */
#define RESET 6  /* button option for the reset option    */
#define TEST  7  /* button option for the test option     */

/*-- Moisture Flag --*/

#define DRY   0b00000000 /*            if the Arduino states the moisture sensor is dry                 */
#define WET   0b00000001 /*            if the Arduino states the moisture sensor is wet                 */
#define V_WET 0b00000010 /* if the Arduino states the moisture sensor is very wet, used the same as wet */

#define PUMP_DELAY 10800 /* pump timer based on the regular tranmissions of the Arduino */

/*-- External Variables --*/
extern GLCD_FONT GLCD_Font_6x8;
extern GLCD_FONT GLCD_Font_16x24;

/*-- Global Variables --*/

int fan_speed; 	/* speed the fan should run at see fan.h for speeds */
int pump_tick;	/* current pump tick [unused] would be used for psuedo pump clock */

/*-- Current readings --*/

unsigned char pump_time 				= 0;  /*                  how long the pump should run for                */
unsigned char temp_value 				= 0;	/*           current temperature reading from the Arduino           */
unsigned char humidity_value 		= 0;  /*             current humidity reading from the Arduino            */
unsigned char soilMoistureValue = 0;	/* current soil moisture value - currently a flag of 'wet' or 'dry' */

unsigned char* sensor_values[3];      /* array to store and iterate over values */

char* sensor_names[] = {"Temperature\0", "Humidity\0",  "Moisture\0"}; /* used for iterating over and displaying the sensor boxes */


/*-- Function Prototypes --*/

/** @brief Handles whether the fan and/or pump should turn on based on
					 whether the readings are greater than the max values or if the 
					 moisture flag is 'wet' or 'dry'. Fan speed is determined by the
					 difference between the max value and current reading.
 *  @param Void.
 *  @return Void.
 */
void handler(void);

/** @brief Basic function used to test if the peripherals are working - used by the user.
 *  @param Void.
 *  @return Void.
 */
void test(void);

/** @brief Determines is a string box, or button, has been pressed by the user.
 *  @param x The x coordinate where the user has pressed.
 *  @param y The y coordinate where the user has pressed.
 *  @return Void.
 */
static signed char check_coords(int x, int y);

/** @brief Draws a box with a background, containing the sensor name and current value.
 *  @param Void.
 *  @return Void.
 */
void draw_sensor_box(char* name, int amount);


/** @brief Draws athe sensors screen with boxes, buttons and top bar.
 *  @param Void.
 *  @return Void.
 */
void draw_sensors(void);

/** @brief The control function of the sensors screen. Handles return values from other screens. 
 *  @param Void.
 *  @return Void.
 */
void sensors(void);


void handler(void)
{

	double temp_diff = 0;
	double humid_diff = 0;
	int pump_time = 0;
	soilMoistureValue = input_buffer[2];
	//temp_chnge = ( abs(temp_value - input_buffer[0]) / ((temp_value + input_buffer[0]) / 2.0) ) * 100;
	//humid_chnge = ( abs(humidity_value - input_buffer[1]) / ((humidity_value + input_buffer[1]) / 2.0) ) * 100;
	temp_value = input_buffer[0];
	humidity_value = input_buffer[1];

	// temperature handling && humidity handling 20-90% - fan control
	if(temp_value > max_temp && humidity_value > max_humidity) {
		temp_diff =  ( abs(temp_value - max_temp) / ((temp_value + max_temp) / 2.0) ) * 100; // temp temp_difference
		humid_diff =  ( abs(humidity_value - max_humidity) / ((humidity_value + max_humidity) / 2.0) ) * 100; // temp temp_difference
		
		if     (( 0 <= temp_diff && temp_diff < 25)   || (0 <= humid_diff && humid_diff < 25))    { fan_speed = SLOW;   }
		else if((25 <= temp_diff && temp_diff < 50)   || (25 <= humid_diff && humid_diff < 50))   { fan_speed = MED;    }
		else if((50 <= temp_diff && temp_diff < 75)   || (50 <= humid_diff && humid_diff < 75))   { fan_speed = FAST;   }
		else if((75 <= temp_diff && temp_diff <= 100) || (75 <= humid_diff && humid_diff <= 100)) { fan_speed = F_FAST; }
		
	} else { fan_speed = OFF; }

	if(soilMoistureValue == 0x00) { 
		pump_time = 5000; 
	}
	//if(pump_tick-- == 0) {
	pump_start(pump_time);
		//pump_tick = PUMP_DELAY;
	//}
	fan_run(fan_speed);
	
	return;
}

void test(void)
{
	
	pump_start(3000);
	fan_run(SLOW);
	HAL_Delay(3000);
	fan_run(MED);
	HAL_Delay(3000);
	fan_run(FAST);
	HAL_Delay(3000);
	fan_run(OFF);
	
	return;
}


static signed char check_coords(int x, int y) 
{
	
	if(0 <= y && y <= 30) { // top bar selection options
		if(0 <= x && x <= 64) { return HOME; }
		if(80 <= x && x <= 208) { return SETTINGS; }
		if(224 <= x && x <= 288) { return HELP; }
	} else if(75 <= y && y <= (75 + 75)) { // sensor buttons
		if(25 <= x && x <= (25 + (16 * 11))) { return TEMP; }
		if(201 <= x && x <= (201 + (16 * 11))) { return HUMID; }
		if(402 <= x && x <= (402 + (16 * 11))) { return MOIST; }
	} else if(225 <= y && y <= (225 + 30)) { // bottom options
		if(25 <= x && x <= (25 + (16 * 7))) { return RESET; }
		if(150 <= x && x <= (150 + (16 * 8))) { return SETTINGS; }
		if(325 <= x && x <= (325 + (16 * 6))) { return TEST; }
	}

	return -1;
}


void draw_sensors_box(void)
{
	
	int height = 75;
	int width = (strlen(sensor_names[0]) + 2) * 8; // 16 for each char (font) + 2 for padding
	int i = 0, j = 0, x = 50, y = 75, x_offset = 10, y_offset = 10;
	char str[3];
	GLCD_SetFont(&GLCD_Font_6x8);
	GLCD_SetForegroundColor(GLCD_COLOR_BLACK);
	for(j = 0; j < 3; j++) {
		for(i = 0; i < height; i++) {
			GLCD_DrawHLine(x + x_offset, (y + y_offset) + i, width); 
		}
		x += (width + 25);
	}
	x = 50;
	for(j = 0; j < 3; j++) {
		for(i = 0; i < height; i++) {
			GLCD_SetForegroundColor(LIGHT_GREY);
			GLCD_DrawHLine(x, y + i, width); 
		}
		GLCD_SetBackgroundColor(LIGHT_GREY);   
		GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
		GLCD_SetFont(&GLCD_Font_6x8);
		GLCD_DrawString(x+16, y+5, sensor_names[j]);
		GLCD_SetFont(&GLCD_Font_16x24);
		if(j == 2) {
			if(soilMoistureValue == 0) {
				GLCD_DrawString(x+40, y+30, "Dry");
			} else {
				GLCD_DrawString(x+40, y+30, "Wet");
			}
		} else {
			sprintf(str, "%d", *sensor_values[j]);
			GLCD_DrawString(x+40, y+30, str); 
		}
		x += (width + 25);
	}
	return;
}

void draw_sensors(void)
{
	draw_sensors_box();
	GLCD_SetFont(&GLCD_Font_16x24);
	draw_string_box(25, 225, "Reset");
	draw_string_box(150, 225, "Settings");
	draw_string_box(325, 225, "Test");
	GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);
	return;
}


void sensors(void)
{
	
	TOUCH_STATE tsc_state;
	
	int pressed = 0;
	sensor_values[0] = &temp_value;
	sensor_values[1] = &humidity_value;
	sensor_values[2] = &soilMoistureValue;
	
	pump_init(); 
	fan_init(); 
	GLCD_ClearScreen();  
	draw_top_bar();
	GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);   
	draw_sensors();
	SER_Init();
	//SER_Ready();
	for(;;){
		Touch_GetState(&tsc_state);
		if(tsc_state.pressed) {
			pressed = check_coords(tsc_state.x, tsc_state.y);
			switch(pressed) {
				case -1: 
					wait_delay(100);
					//GLCD_ClearScreen(); 
					//draw_top_bar();
					//GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);   
					//draw_sensors();
					break;
				case HOME:
				  GLCD_ClearScreen(); 
					draw_top_bar();
					GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);   
					draw_sensors();
					wait_delay(100);
					break;
				case SETTINGS: 
					pressed = settings();
				  break;
				case HELP: 
					pressed = info();
					break;
				case TEST: 
					test();
				  pressed = -1;
					break;
				case RESET:
					temp_value = 0;
					humidity_value = 0;
				  soilMoistureValue = 0;
					input_buffer[0] = 0;
					input_buffer[1] = 0;
					input_buffer[2] = 0;
					sensor_num = 0;
					GLCD_ClearScreen(); 
					draw_top_bar();
					GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);   
					draw_sensors();
					wait_delay(100);
					break;
			}
		}
		wait_delay(100);
		if(sensor_num == 3) { // if all sensors info retrieved, update screen
			HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
			//SER_Busy();
			handler();		
			GLCD_ClearScreen();  
			draw_top_bar();
			GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);  
			draw_sensors();
			sensor_num = 0;
			HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
			//SER_Ready();
		}
	}

}
