/**
  ******************************************************************************
  * @file    fan.h
  * @author  T. Buckingham
  * @brief   Fan initialisation and control
  *
  *          The file contains ::
  *           + Fan speed variables
  *
  ******************************************************************************
  */

#ifndef __FAN_H /* Include guard */
#define __FAN_H 


/*-- Defines the values for the PWM fan's speeds. --*/

#define SLOW  	 20	/*          slow fan speed           */
#define MED   	 15 /*         medium fan speed          */
#define FAST  	 10 /*          fast fan speed           */
#define F_FAST 	  5 /*         very fast speed 				   */	
#define OFF 	  200 /* turns fan off :: -1 may also work */

/**
Pin PA15 configuration (TIM2_CH1) and initialization.
The only difference between the traditional GPIO initialization is the use of alternate function:
1) We need to specify the mode (gpio.Mode) as the alternate;
2) We need to map the right timer (or other peripheral) to the pin (gpio.Alternate).
**/
void PA15_Init(void);

/**
Timer (TIM2) and Channel (CH1) initialization. 
**/
static void TIM2_Init(void);

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

#endif
