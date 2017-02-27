#ifndef IMU_H
#define IMU_H

#include "lcd.h"
#include "I2C.h"
#include "MPU9255.h"
#include "AK8963.h"
#include "BMP180.h"
#include "typeDef.h"

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
	XYZ16_TypeDef acc, mag;
	XYZ16_TypeDef rot;
	private:
	MPU9255 mpu9255;
	AK8963 ak8963;
	BMP180 bmp180;
};


#endif