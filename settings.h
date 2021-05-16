/**
  ******************************************************************************
  * @file    settings.h
  * @author  T. Buckingham
  * @brief   User defined settings used in sensors.c's handler
  *
  *          The file contains ::
  *           + External variables defined by the user for controller the devices'
	*							temperature, humidity and moisture level
  *
  ******************************************************************************
  */

#ifndef SETTINGS_H_   /* Include guard */
#define SETTINGS_H_

/* -- User defined device settings -- */


/**
	* Defines the maximum temperature of the device - set by the user
	*/
extern unsigned char max_temp; 
/**
	* Defines the minimum moisture level of the soil - set by the user
	*/
extern unsigned char min_moisture_level;
/**
	* Defines the maximum humidity of the device - set by the user
	*/
extern unsigned char max_humidity;

/** @brief Displays the settings screen to the user by drawing the 
 * 					UI elements. 
 *  @param Void.
 *  @return int that tells the sensors.c what screen to display next
 */
int settings(void);

#endif // 
