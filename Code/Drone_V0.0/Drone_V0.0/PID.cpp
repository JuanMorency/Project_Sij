/**
******************************************************************************
* File Name         : PID.cpp
* Description       : Class for PID control
* Author			: Juan Morency Trudel
* Version           : 1.0.0
* Date				: July 2017
******************************************************************************
*/

#include "PID.h"


/** 
 * @brief Default constructor
 * @param pidId: the number identifying the PID
 * @param Kp: proportional multiplier of PID
 * @param Ki: integral multiplier of PID
 * @param Kd: derivative multiplier of PID
 */
PID::PID(uint8_t pidId, float kp, float ki, float kd) 
{
	this->pidId = pidId;
	this->kp = kp;
	this->ki = ki;
	this->kd = kd;
	adjustmentRaw = 0;
	adjustmentEsc = 0;
	desiredAngle = 0;
	if (pidId == YAW)
	{
		desiredAngle = yaw;
	}
}


/** 
 * @brief Trivial PID controller
 * @param current : the input value to compare to desired value 
 * @param target : desired value for the PID loop to converge to
 * @return control variable of the PID
 */
void PID::updatePid(float current, float target)
{
	// Calculate error
	error = target - current;
	
	// Calculate the integral
	integral = integral + error;
	//set max integral to avoid too much accumulation when drone does not move
	if(integral >= MAX_PID_INTEGRAL)
	{
		integral = MAX_PID_INTEGRAL;
	}
	else if(integral <= -MAX_PID_INTEGRAL)
	{
		integral = -MAX_PID_INTEGRAL;
	}

	
	// Calculate the derivative
	derivative = error - lastError;
	
	// Update lastError for the derivative
	lastError = error;
		
	// return control variable
	adjustmentRaw = (kp*error+ki*integral+kd*derivative);
	adjustmentEsc = adjustmentRaw*ESC_INIT_PW/180;
}

int16_t PID::getAdjustment()
{
	return (int16_t)adjustmentEsc;
}

void PID::setDesiredAngle(float angle)
{
	if(angle >= 180)
	{
		desiredAngle = angle - 360;
	}
	else if(angle <= -180)
	{
		desiredAngle = angle + 360;
	}
	else
	{
		desiredAngle = angle;		
	}


}

float PID::getDesiredAngle()
{
	return desiredAngle;
}