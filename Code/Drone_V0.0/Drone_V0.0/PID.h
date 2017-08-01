/**
******************************************************************************
* File Name         : PID.h
* Description       : Header file for PID.cpp
* Author			: Juan Morency Trudel
* Version           : 1.0.0
* Date				: July 2017
******************************************************************************
*/


#ifndef PID_H_
#define PID_H_

#include "debugLED.h"
#include "serial.h"
#include "esc.h"
#include "MadgwickAHRS.h"

#define ROLL_KP 0.89f
#define ROLL_KI 0.01f
#define ROLL_KD 0.1f

#define PITCH_KP 0.89f
#define PITCH_KI 0.01f
#define PITCH_KD 0.1f

#define YAW_KP 0.89f
#define YAW_KI 0.01f
#define YAW_KD 0.1f

#define MAX_PID_INTEGRAL 90.0f

class PID
{
	public:
		PID(uint8_t pidId , float kp, float ki, float kd);
		
		uint8_t pidId;
		void updatePid(float current, float target);
		int16_t getAdjustment();
		void setDesiredAngle(float angle);
		float getDesiredAngle();
	private:
		float kp, ki, kd;
		float integral, error, derivative, lastError;
		float adjustmentRaw;
		float adjustmentEsc;
		float desiredAngle;
};


#endif /* PID_H_ */