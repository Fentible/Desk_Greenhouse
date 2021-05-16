/**
  ******************************************************************************
  * @file    Serial.h
  * @author  T. Buckingham
  * @brief   Serial communication between the STM32 and the Arduino
  *
  *          The file contains ::
  *           + Buffers for inputting and outputting data using bit banging
  *
  ******************************************************************************
  */

#ifndef __SERIAL_H /* Include guard */
#define __SERIAL_H 


/* -- Buffers and counter used for serial communication -- */

/**
	* Counter to track of what sensor data is being recieved/transmitted
	*/
extern unsigned char sensor_num;

/**
	* Input buffer used to store recieved sensor data
	*/
extern unsigned char input_buffer[3];

/**
	* Output buffer used to store outgoing sensor data
	*/
extern unsigned char output_buffer[3];

/** @brief Initialises the necessary pins for input and output, and sets interrupts. 
 *  @param  Void.
 *  @return Void.
 */
void SER_Init(void);

/** @brief Reads incoming data from the Arduino - [deprecated] as an interrupt handler is used. 
 *  @param  buffer The 8 bits buffer used to store the recieved data. 
 *  @return Void.
 */
void SER_Read(unsigned char buffer);

/** @brief  Sends a buffer to the Arduino using the transmit pin. [not currently used]
 *  @param  buffer The 8 bit buffer being transmitted.
 *  @return Void.
 */
void SER_Write(unsigned char buffer);

/** @brief Tells the Arduino that the STM32 is busy and cannot recieve data. 
 * 					The Arduino will reset its transmission if any are partially sent.
 *  @param  Void.
 *  @return Void.
 */
void SER_Busy(void);

/** @brief Tells the Arduino that the STM32 is ready to recieve data. 
 *  @param  Void.
 *  @return Void.
 */
void SER_Ready(void);

#endif

