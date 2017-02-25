#include "IMU.h"
#include "BMP180.h"
const unsigned char OSS = 0;
int16_t tempRaw;
uint32_t timer;
uint8_t i2cData[14];
bool InertMUInitialized  = false;

	char buffer[20];
	char buffer2[20];

//TODO initialize variables acc, gyr, etc.

//constructor
IMU::IMU()
{
	
}
void IMU::initialize()
{
	mpu9255.initialize();
	ak8963.initialize();
	//need to set up the magnetometer
	//BMP180_Init();
	InertMUInitialized  = true;

}

void IMU::takeMeasures()
{
	//static uint8_t data[20];
	mpu9255.updateRawData();
	mpu9255.calculateAccRotTemp();
	//ak8963.updateRawData();
	//ak8963.calculateMag();
	
	this->acc.X = mpu9255.getAcceleration().X;
	this->acc.Y = mpu9255.getAcceleration().Y;
	this->acc.Z = mpu9255.getAcceleration().Z;
	this->temp = mpu9255.getTemperature();
	this->rot.X = mpu9255.getRotation().X;
	this->rot.Y = mpu9255.getRotation().Y;
	this->rot.Z = mpu9255.getRotation().Z;
	//this->mag.X = ak8963.getMagneticField().X;
	//this->mag.Y = ak8963.getMagneticField().X;
	//this->mag.Z = ak8963.getMagneticField().X;



	//CalTemperatureAndPressureAndAltitude();
	//pres = PressureVal;
	//temp = TemperatureVal;
	//alt =  AltitudeVal;
}



