#ifndef IMU_H
#define IMU_H

#include "lcd.h"
#include "I2C.h"
#include "MPU9255.h"
#include "AK8963.h"
#include "BMP180.h"
#include "typeDef.h"
#include "MadgwickAHRS.h"
#include "interrupt.h"
#include <math.h>

#include <stdio.h>
#include <util/delay.h>

#define M_PI_OVER_1800 M_PI/1800.0f

extern bool imuInitialized;     

extern uint32_t sumCountAk8963, sumCountMpu9255, sumCountBmp180; // used to control display output rate
extern float sumAk8963, sumMpu9255, sumBmp180;        // integration interval for both filter 

class IMU
{
	public:
		IMU();
		void initialize();
		void updateImuAndMadgwick();
		//int32_t pres, alt, temp;
		//XYZ16_TypeDef acc; //in G x10^(-4)
		//XYZ16_TypeDef mag; 
		//XYZ16_TypeDef rot; //in 0.1 degrees per second
		MPU9255 mpu9255;
		AK8963 ak8963;
		BMP180 bmp180;
	private:
		float xAccelerationMadgwick, yAccelerationMadgwick, zAccelerationMadgwick;
		float xRotationMadgwick, yRotationMadgwick, zRotationMadgwick;
		float xMagneticFieldMadgwick, yMagneticFieldMadgwick, zMagneticFieldMadgwick;
};


#endif