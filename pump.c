/**
  ******************************************************************************
  * @file    pump.c
  * @author  T. Buckingham
  * @brief   Pump control functions
  *
  *          The file contains ::
  *           + Basic function to run the pump	
  *
  ******************************************************************************
  */

#include <stm32f7xx_hal_gpio.h>					// GPIO header
#include "stm32f7xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "ui_elements.h"								// User interface functions
#include "pump.h"												// Pump functions and definitions

/*-- Global Variables --*/

GPIO_InitTypeDef pump_pin; /* the pin struct used for the pump */

/*-- Function Protoypes --*/

/** @brief Runs the pump for a given amount of time (in milliseconds).
 *  @param Void.
 *  @return Void.
 */
void pump_start(int time);

/** @brief Initialises the pin used to activate the relay and thus the pump - pin D7.
 *  @param Void.
 *  @return Void.
 */
void pump_init(void);


void pump_init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStruct;

  __HAL_RCC_GPIOI_CLK_ENABLE(); 
	// Output pump pin
  GPIO_InitStruct.Pin 		= GPIO_PIN_3;
  GPIO_InitStruct.Mode 		= GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull 		= GPIO_NOPULL;
  GPIO_InitStruct.Speed 	= GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Type 		= GPIOI;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);
	pump_pin = GPIO_InitStruct;
	HAL_GPIO_WritePin(pump_pin.Type, pump_pin.Pin, GPIO_PIN_RESET);
	
	return;
}

void pump_start(int time)
{
	HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
	HAL_GPIO_WritePin(pump_pin.Type, pump_pin.Pin, GPIO_PIN_SET);
	HAL_Delay(time);
	HAL_GPIO_WritePin(pump_pin.Type, pump_pin.Pin, GPIO_PIN_RESET);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
	
	return;
}
