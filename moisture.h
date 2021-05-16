/**
  ******************************************************************************
  * @file    moisture.h
  * @author  T. Buckingham
  * @brief   Moisture sensor and values handler
  *
  *          The file contains ::
  *           + Variables for calculating the current moisture level
  *
  ******************************************************************************
  */

#ifndef MOISTURE_H /* Include guard */
#define MOISTURE_H

/** @brief Currently used as a testing function for the moisture sensor. 
 *  @param Void.
 *  @return Void.
 */
void moisture(void);


/* -- Moisture sensor values -- */


/**
	* Defines air value reading of the sensor
	*/
extern const int AirValue; 

/**
	* Defines water value reading of the sensor
	*/
extern const int WaterValue;  

/**
	* Intervals used indetermining if the reading is considered 'dry' or 'wet'
	*/
extern int intervals;


#endif
