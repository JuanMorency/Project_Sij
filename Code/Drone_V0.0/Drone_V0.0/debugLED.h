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


#define LED0 PORTC0		//top LED
#define LED1 PORTC1
#define LED2 PORTC2
#define LED3 PORTC3
#define LED4 PORTC4
#define LED5 PORTC5
#define LED6 PORTC6
#define LED7 PORTC7		//bottom LED

void turnDebugLedOn(uint8_t ledNumber);
void turnDebugLedOff(uint8_t ledNumber);
void turnAllDebugLedOff();
void turnDebugLedOnAndWait(uint8_t ledNumber);

#endif
