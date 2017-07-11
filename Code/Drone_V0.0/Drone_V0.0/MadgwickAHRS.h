//=====================================================================================================
// MadgwickAHRS.h
//=====================================================================================================
//
// Implementation of Madgwick's IMU and AHRS algorithms.
// See: http://www.x-io.co.uk/node/8#open_source_ahrs_and_imu_algorithms
//
// Date			Author          Notes
// 29/09/2011	SOH Madgwick    Initial release
// 02/10/2011	SOH Madgwick	Optimised for reduced CPU load
//
//=====================================================================================================
#ifndef MadgwickAHRS_h
#define MadgwickAHRS_h

#include <math.h>
#include "debugLED.h"
#include "interrupt.h"

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

void MadgwickAHRSupdate(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz);
void MadgwickAHRSupdateIMU(float ax, float ay, float az, float gx, float gy, float gz);
void calcEulerAngles(); //in degrees
float atan2_approximation2(float y, float x);
float asin_approximation(float x);

#endif
//=====================================================================================================
// End of file
//=====================================================================================================
