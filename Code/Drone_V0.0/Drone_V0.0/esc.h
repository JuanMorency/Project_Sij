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
#define ESC_INIT_PW 2000 // initial pulse width
#define ESC_MAX_PW 4000 // max pulse width

#include <avr/io.h>
#include <util/delay.h>

extern uint16_t yawAdjustment;
extern uint16_t pitchAdjustment;
extern uint16_t rollAdjustment;

extern uint16_t FlSpeed;
extern uint16_t BlSpeed;
extern uint16_t BrSpeed;
extern uint16_t FrSpeed;

class Esc
{
	int escNumber;
public:
	Esc(int escPosition);
	void set(uint16_t pwm);	
};

	void initializeESC();
	void calibrateESC();
	extern bool escInitialized;
#endif
