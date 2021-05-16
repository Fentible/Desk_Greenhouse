/**
  ******************************************************************************
  * @file    info.h
  * @author  T. Buckingham
  * @brief   Information screen for the Desk Green House
  *
  *          The file contains ::
  *           + Function prototypes
  *
  ******************************************************************************
  */
	
#ifndef INFO_H_   /* Include guard */
#define INFO_H_

/** @brief Displays an info screen on the GLCD screen with some basic
 * 					information about the device
 *  @param  Void.
 *  @return int - next page for sensors.c to open.
 */
int info(void);

#endif // 

