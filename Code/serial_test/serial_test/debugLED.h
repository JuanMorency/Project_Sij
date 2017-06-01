/**
  ******************************************************************************
	* File Name         : debugLED.h
	* Description       : header for debugLED.cpp
	* Author			: Juan Morency Trudel
	* Version           : 1.0.0
	* Date				: February, 2017
  ******************************************************************************
  */

#ifndef DEBUGLED_H
#define DEBUGLED_H

#ifndef F_CPU
# define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

#define LED0 PORTC0		//top LED (Blue)
#define LED1 PORTC1		//Red
#define LED2 PORTC2		//Yellow
#define LED3 PORTC3		//Blue
#define LED4 PORTC4		//Red
#define LED5 PORTC5		//Yellow
#define LED6 PORTC6		//Blue
#define LED7 PORTC7		//bottom LED (Red)

void turnDebugLedOn(uint8_t ledNumber);
void turnDebugLedOff(uint8_t ledNumber);
void turnAllDebugLedOff();
void turnDebugLedOnAndWait(uint8_t ledNumber);

#endif
