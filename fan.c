/**
  ******************************************************************************
  * @file    fan.c
  * @author  T. Buckingham
  * @brief   Fan control functions
  *
  *          The file contains ::
  *           + Function to initialise the fan's PWM pin
	*						+ Basic function to set the fans speed
  *
  ******************************************************************************
  */

#include "stm32f7xx_hal.h"
#include "SystemClock.h"
#include "ui_elements.h"								// User Interface Elements
#include "fan.h"												// Fan functions and definitions

/*-- Global Variables --*/

TIM_HandleTypeDef htim2; /* TIM struct used for the PWM fan */
TIM_OC_InitTypeDef sConfigOC;


/*-- Function Prototypes --*/

/** @brief Runs the necessary init functions and starts the PWM on the pin. 
 *  @param  Void.
 *  @return Void.
 */
void fan_init(void);

/** @brief  Runs the fan at the specified speed by setting CCR1
 *  @param  speed The speed the fan is to run at, speeds are defined in fan.h
 *  @return Void.
 */
void fan_run(char speed);

/**
Pin PA15 configuration (TIM2_CH1) and initialization.
The only difference between the traditional GPIO initialization is the use of alternate function:
1) We need to specify the mode (gpio.Mode) as the alternate;
2) We need to map the right timer (or other peripheral) to the pin (gpio.Alternate).
**/
void PA15_Init(void){
	GPIO_InitTypeDef gpio;

	// Enable the clock for the base A
	__HAL_RCC_GPIOA_CLK_ENABLE();

	gpio.Pin = GPIO_PIN_15;
	gpio.Mode = GPIO_MODE_AF_PP;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio.Alternate = GPIO_AF1_TIM2;
	HAL_GPIO_Init(GPIOA, &gpio);
}


/**
Timer (TIM2) and Channel (CH1) initialization. 
**/
static void TIM2_Init(void){
	TIM_ClockConfigTypeDef sClockSourceConfig;
	
	//Timer configuration
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 16000-1;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 40-1;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&htim2);

	//Set the timer in PWM mode
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig);
	HAL_TIM_PWM_Init(&htim2);

	//Channel configuration
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1);

	PA15_Init();
}


void fan_run(char speed) 
{
	htim2.Instance->CCR1 = speed;
	return;
}


void fan_init(void) {
	//Reset of all peripherals, initializes the Flash interface and the Systick
	HAL_Init();
	//Enable peripheral clock for TIM2
	__HAL_RCC_TIM2_CLK_ENABLE();
	//Initialize TIM2, CH1 and PA15
	TIM2_Init();
	//Start PWM on TIM2_CH1 (PA15 a.k.a. D9)
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	//Set the compare register to 1
	htim2.Instance->CCR1 = 1;

}
