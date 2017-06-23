/**
  ******************************************************************************
	* File Name         : esc.h
	* Description       : header for esc.cpp
	* Author			: Juan Morency Trudel, Simon Poirier
	* Version           : 1.0.0
	* Date				: September, 2016
  ******************************************************************************
  */

#ifndef ESC_H
#define ESC_H

#ifndef F_CPU
#define F_CPU 16000000
#endif

#define DELAY_ESC 5000 //time to initialize the ESC in microseconds
#define BL 1
#define FL 2
#define FR 3
#define BR 4
#define ESC_INIT_PW 8000 //initial pulse width, should try to find a dynamic way to set this.

#include <avr/io.h>
#include <util/delay.h>

class Esc
{
	int escNumber;
public:
	Esc(int escPosition);
	void set(uint16_t pwm);	
};

	void initializeESC();
	extern bool escInitialized;
#endif
