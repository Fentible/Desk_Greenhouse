/**
  ******************************************************************************
  * @file    pump.h
  * @author  T. Buckingham
  * @brief   Pump initialisation and control
  *
  *          The file contains ::
  *           + Function prototypes
  *
  ******************************************************************************
  */

#ifndef __PUMP_H /* Include guard */
#define __PUMP_H 

/** @brief  Activates the relay that in turn turns on the pump for x amount of time. 
 *  @param  time The amount of time the pump will be on for. 
 *  @return Void.
 */
void pump_start(int time);

/** @brief Initialises the necessary pins for activating the relay. 
 *  @param  Void.
 *  @return Void.
 */
void pump_init(void);

#endif
