#ifndef IMU_H
#define IMU_H

#include "lcd.h"
#include "I2C.h"
#include "MPU9255.h"
#include "AK8963.h"
#include "BMP180.h"
#include "typeDef.h"
#include "MadgwickAHRS.h"

#include <stdio.h>
#include <util/delay.h>

extern bool InertMUInitialized; //cannot name it IMUInitialized, seems to have a conflict with the class...

class IMU
{
	public:
	IMU();
	void initialize();
	void takeMeasures();
	int32_t pres, alt, temp;
	int16_t yaw, roll, pitch;
	XYZ16_TypeDef acc; //in G x10^(-4)
	XYZ16_TypeDef mag; 
	XYZ16_TypeDef rot; //in 0.1 degrees per second
	private:
	MPU9255 mpu9255;
	AK8963 ak8963;
	BMP180 bmp180;
};


#endif