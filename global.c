#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f7xx_hal.h"                  // Device header
#include "GLCD_Config.h"                // Keil.STM32756G-EVAL::Board Support:Graphic LCD
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "SystemClock.h" 								// System Clock Configuration & Event Handler
#include "Board_Touch.h"								// Touch Screen
#define wait_delay HAL_Delay

extern GLCD_FONT GLCD_Font_6x8;
extern GLCD_FONT GLCD_Font_16x24;

