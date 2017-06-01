#include "IMU.h"

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
	//bmp180.initialize();
	InertMUInitialized  = true;
}

void IMU::takeMeasures()
{
	//mpu9255.updateRawData();
	//mpu9255.calculateAccRotTemp();
	ak8963.updateRawData();
	ak8963.calculateMag();
	
	//this function updates the raw values on it's own with a state machine. 
	//it is assumed that the delay between calls will be greater than 10 ms
	//which is the time required for the conversion of the pressure
	bmp180.CalculateTemperaturePressureAndAltitude();
	
	//this->acc.X = mpu9255.getAcceleration().X;
	//this->acc.Y = mpu9255.getAcceleration().Y;
	//this->acc.Z = mpu9255.getAcceleration().Z;
	//this->rot.X = mpu9255.getRotation().X;
	//this->rot.Y = mpu9255.getRotation().Y;
	//this->rot.Z = mpu9255.getRotation().Z;
	this->mag.X = ak8963.getMagneticField().X;
	this->mag.Y = ak8963.getMagneticField().Y;
	this->mag.Z = ak8963.getMagneticField().Z;
	//this->pres =  bmp180.getPressure();
	//this->temp =  bmp180.getPressure0();
	//this->alt =  bmp180.getAltitude();
}



