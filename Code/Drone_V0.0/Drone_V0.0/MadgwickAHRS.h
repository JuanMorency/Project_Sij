/**
******************************************************************************
* File Name         : MadgwickAHRS.h
* Description       : header file for MadgwickAHRS.cpp
* Author			: SOH Madgwick, Juan Morency Trudel (Adaptation atmega2560 quadcopter)
* Version           : 1.0.0
* Date				:   29/09/2011  Initial release
						02/10/2011 Optimised for reduced CPU load
******************************************************************************
*/



#ifndef MadgwickAHRS_h
#define MadgwickAHRS_h

#include <math.h>
#include "debugLED.h"
#include "interrupt.h"


#define PI_FLOAT     3.14159265f
#define PIBY2_FLOAT  1.5707963f

#define betaDef		0.5f		// 2 * proportional gain

//----------------------------------------------------------------------------------------------------
// Variable declaration

extern volatile float beta;				// algorithm gain
extern volatile float q0, q1, q2, q3;	// quaternion of sensor frame relative to auxiliary frame
extern volatile float roll, pitch, yaw;	// Euler Angles
//following convention was used for yaw pitch roll positive direction: https://en.wikipedia.org/wiki/Flight_dynamics_(fixed-wing_aircraft)
//same as NASA
//yaw should be 0 when pointing to magnetic south pole

//---------------------------------------------------------------------------------------------------
// Function declarations


float invSqrt(float x);
void MadgwickAHRSupdate(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz);
void MadgwickAHRSupdateIMU(float ax, float ay, float az, float gx, float gy, float gz);
void calcEulerAngles(); //in degrees
float atan2_approximation2(float y, float x);
float asin_approximation(float x);

#endif
//=====================================================================================================
// End of file
//=====================================================================================================
