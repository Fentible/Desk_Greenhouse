/**
  ******************************************************************************
  * @file    moisture.c
  * @author  T. Buckingham
  * @brief   Moisture sensor related settings
  *
  *          The file contains ::
  *           + The air and water variables for determining the current moisture level
	*						+ A basic test function for the moisture sensor
  *
  ******************************************************************************
  */

#include "stm32f7xx_hal.h"                  // Device header
#include "GLCD_Config.h"                // Keil.STM32756G-EVAL::Board Support:Graphic LCD
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "SystemClock.h" 								// System Clock Configuration & Event Handler
#include "Board_Touch.h"								// Touch Screen
#include "stm32f7xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "stm32f7xx_hal_gpio.h"					// GPIO header
#include "stm32f7xx_hal.h"
#include "moisture.h"
#include "ui_elements.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "Serial.h" 
#include "pump.h"
#include "fan.h"
#include "moisture.h"

/*-- Definitions --*/
#define wait_delay HAL_Delay

/*-- External Variables --*/
extern GLCD_FONT GLCD_Font_6x8;
extern GLCD_FONT GLCD_Font_16x24;

const int AirValue = 650;                  /*                   air value the sensor reads                     */
const int WaterValue = 300;                /*                  water value the sensor reads                    */
int intervals = (AirValue - WaterValue)/3; /* interval value used to determine if the sensor is 'dry' or 'wet' */

// Local Functions

/** @brief Test function for the moisture sensor - [unused]
 *  @param Void.
 *  @return Void.
 */
void moisture(void);

void moisture(void)
{
	SER_Init();
	//SER_Write(temp, sensor);

	GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);
	GLCD_SetForegroundColor(GLCD_COLOR_BLACK);
	GLCD_SetFont(&GLCD_Font_16x24); 
	pump_init();
	fan_init(); 
  for (;;)
	{	
		HAL_Delay(2000); 
		fan_run(OFF);
		HAL_Delay(2000);
		fan_run(SLOW);
		HAL_Delay(2000);
		fan_run(MED);
		HAL_Delay(2000);
		fan_run(FAST);

	}
 
}

