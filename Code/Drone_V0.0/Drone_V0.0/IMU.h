#ifndef IMU_H
#define IMU_H

#include "lcd.h"
#include "I2C.h"
//#include "BMP180.h"
#include "MPU9255.h"
#include "AK8963.h"
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
	uint16_t pres, alt, temp;
	uint16_t yaw, roll, pitch;
	XYZ16_TypeDef acc, rot, mag;
	
	private:
	MPU9255 mpu9255;
	AK8963 ak8963;
};


#endif