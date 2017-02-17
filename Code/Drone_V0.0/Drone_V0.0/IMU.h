#ifndef IMU_H
#define IMU_H

#define PWR_MGMT_1 0x6B //Power Management. Typical values:0x00(run mode)
#define SMPLRT_DIV 0x19
#define CONFIG 0x1A
#define GYRO_CONFIG 0x1B
#define ACCEL_CONFIG 0x1C

#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40

#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42

#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48


#define MAG_XOUT_L		0x03
#define MAG_XOUT_H		0x04
#define MAG_YOUT_L		0x05
#define MAG_YOUT_H		0x06
#define MAG_ZOUT_L		0x07
#define MAG_ZOUT_H		0x08

#define	WHO_AM_I		0x75	//identity of the device


#include "lcd.h"
#include "I2C.h"
#include "BMP180.h"
#include <stdio.h>
#include <util/delay.h>

extern bool InertMUInitialized; //cannot name it IMUInitialized, seems to have a conflict with the class...

typedef struct
{
	int16_t X;
	int16_t Y;
	int16_t Z;
}XYZ16_TypeDef;

class IMU
{
	public:
	IMU();
	void initialize();
	void takeMeasures();
	void initGyrOffset();
	uint16_t pres, alt;
	XYZ16_TypeDef gyrOffset, accRaw, gyroRaw, magRaw;
};


#endif