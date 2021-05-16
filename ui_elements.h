/**
  ******************************************************************************
  * @file    ui_elements.h
  * @author  T. Buckingham
  * @brief   Sensors screen and outputs sensors handler
  *
  *          The file contains ::
  *           + Option definitions for the top menu bar
	*						+ Colours used throughout the project for UI elements
  *
  ******************************************************************************
  */

#ifndef UI_ELEMENTS_H_   /* Include guard */
#define UI_ELEMENTS_H_


/**
	* Global button options - mostly used in the top menu bar
	*/
#define HOME 0
#define SETTINGS 1
#define HELP 2

/**
	* Defines the colour values used throughout the UI elements.
	*/
#define BACKGROUND_BLUE 0x02D9
#define FOREGROUND_BLUE 0x52DF
#define TEXT_BLUE 			0x5F5F
#define TEXT_WHITE 			0x739C
#define LIGHT_GREY			0x4A52

/** @brief  Draws a pseudo button on the screen by drawing a box then drawing a string on top.
 * 					The padding uniform and adjusts to the strings length. 
 *  @param  x The x position of the start button.
 *  @param  y The y position of the start button. 
 *  @param  str The string that will be drawn in the button. 
 *  @return Void.
 */
void draw_string_box(int x, int y, char* str);

/** @brief Draws the top menu bar with the options 'Home', 'Settings' and 'Help'.
						These options sensors.c , settings.c and info.c respectively.
 *  @param  Void.
 *  @return Void.
 */
void draw_top_bar(void);

/** @brief Draws a string with checkboxes to the right of it. 
 *  @param  x The x position of the start of the string.
 *  @param  y The y position of the string. 
 *  @param  str The string to be drawn before the boxes.
 *  @param  boxes The number of boxes drawn after the string. 
 *  @return Void.
 */
void draw_check_option(int x, int y, char* str, int boxes);

#endif // 
