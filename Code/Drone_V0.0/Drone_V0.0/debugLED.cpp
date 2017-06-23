/**
  ******************************************************************************
	* File Name         : debugLED.cpp
	* Description       : Implement methods for controlling the Debug LED on PORT C of custom drone board iteration 1
	* Author			: Juan Morency Trudel
	* Version           : 1.0.0
	* Date				: February, 2017
  ******************************************************************************
  */


#include "debugLED.h"

/**
 * @brief Easily change the one LED on
 * @param ledNumber: the number of the LED that you want to turn on
 */
void turnDebugLedOn(uint8_t ledNumber)
{
	if (ledNumber <= 7)
	{
		DDRC = 0xFF;
		PORTC |= 1<<ledNumber;
	}
	
}

/**
 * @brief Turns one the debug LED off
 * @param ledNumber: the number of the LED that you want to turn on
 */
void turnDebugLedOff(uint8_t ledNumber)
{
	if (ledNumber <= 7)
	{
		PORTC &= !(1<<ledNumber);
	}
}

/**
	* @brief Turns all the debug LED off
	*/
void turnAllDebugLedOff()
{
	PORTC = 0xFF;
}

/**
	* @brief Easily change one LED on and wait forever
	* @param ledNumber: the number of the LED that you want to turn on
	*/
void turnDebugLedOnAndWait(uint8_t ledNumber)
{
	PORTC = 0xFF;
	while(1);
}

