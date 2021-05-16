/**
  ******************************************************************************
  * @file    Serial.c
  * @author  T. Buckingham
  * @brief   Moisture sensor related settings
  *
  *          The file contains ::
  *           + Functions for transmitting and recieving bytes 
	*						+ Input and output buffers
	* 					+ Counters to state what sensor is currently being transmitted recieved			
  *
  ******************************************************************************
  */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include <stm32f7xx_hal_gpio.h>					// GPIO header
#include "GLCD_Config.h"                // Keil.STM32756G-EVAL::Board Support:Graphic LCD
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "Serial.h"											// Serial communication between the arduino
#include "dwt_delay.h" 									// Non-HAL delay - for uS



/*-- Global Variables --*/

int i = 0; /* counter declared as global to speed up the serial reciever */
 
GPIO_InitTypeDef read_pin;  /* pin used for recieving data     */
GPIO_InitTypeDef write_pin; /* pin used for transmitting data  */

/*-- Input/Output buffers --*/

unsigned char sensor_num;				/* current sensor data being received or transmitted */
unsigned char input_buffer[3];  /* buffer array used for recieving sensor data       */
unsigned char output_buffer[3]; /* buffer array used for transmitting sensor data    */

/*-- Function Prototypes --*/

uint32_t DWT_Delay_Init(void);

/** @brief Serial interrupt handler. Reads in a buffer from the Arduino.
 *  @param Void.
 *  @return Void.
 */
void EXTI9_5_IRQHandler(void);

/** @brief Determines is a string box, or button, has been pressed by the user.
 *  @param Void.
 *  @return Void.
 */
void SER_Init(void);

/** @brief Function to transmit an 8 bit buffer to the arduino
 *  @param Void.
 *  @return Void.
 */
void SER_Write(unsigned char buffer);

/** @brief Function to tell the arduino to stop and reset as the STM32 is busy - [unused]
 *  @param Void.
 *  @return Void.
 */
void SER_Busy(void);

/** @brief Function to tell the arduino begin transmitting as the STM32 is ready - [unused]
 *  @param Void.
 *  @return Void.
 */
void SER_Ready(void);


void SER_Init(void) 
{

	GPIO_InitTypeDef GPIO_InitStruct;

  __HAL_RCC_GPIOG_CLK_ENABLE(); 
	DWT_Init();
	// Input pin
  GPIO_InitStruct.Pin 		= GPIO_PIN_6;
  GPIO_InitStruct.Mode 		= GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull 		= GPIO_PULLDOWN;
  GPIO_InitStruct.Speed 	= GPIO_SPEED_FAST;
	GPIO_InitStruct.Type = GPIOG;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
	/* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
	read_pin = GPIO_InitStruct;
	
	// Output pin
  GPIO_InitStruct.Pin 		= GPIO_PIN_7;
  GPIO_InitStruct.Mode 		= GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull 		= GPIO_NOPULL;
  GPIO_InitStruct.Speed 	= GPIO_SPEED_FAST;
	GPIO_InitStruct.Type = GPIOG;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  write_pin = GPIO_InitStruct;
	
	return;
	
}

void EXTI9_5_IRQHandler(void) 
{
	DWT_Delay(100);
	for(i = 0; i < 8; i++) {
			input_buffer[sensor_num] = (input_buffer[sensor_num] & ~(1UL << i)) | (HAL_GPIO_ReadPin(read_pin.Type, read_pin.Pin) << i); // DATA
			DWT_Delay(100);
	}
	sensor_num++;
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
}

void SER_Busy(void) 
{
	
	HAL_GPIO_WritePin(write_pin.Type, write_pin.Pin, GPIO_PIN_SET);
	return;
}

void SER_Ready(void)
{
	
	HAL_GPIO_WritePin(write_pin.Type, write_pin.Pin, GPIO_PIN_RESET);
	return;
}

void SER_Write(unsigned char buffer)
{
	
	int i = 0;
	DWT_Init();
	for(i = 0; i < 10; i++) { // BUFFER
			HAL_GPIO_WritePin(write_pin.Type, write_pin.Pin, GPIO_PIN_SET);
			HAL_Delay(500);
	}
	HAL_GPIO_WritePin(write_pin.Type, write_pin.Pin, GPIO_PIN_RESET);
	//DWT_Delay(100);
	for(i = 0; i < 8; i++) {
		//HAL_GPIO_WritePin(write_pin.Type, write_pin.Pin, (buffer & ( 1 << i )) >> i); // gives warning but will be alternative to the below if timing is affected
		HAL_GPIO_WritePin(write_pin.Type, write_pin.Pin, (buffer & ( 1 << i )) >> i  == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);  // - removes warning but may affect timing
		DWT_Delay(100);
		
	}

	return;
}
