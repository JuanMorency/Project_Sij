#include "IMU.h"
#include "BMP180.h"
const unsigned char OSS = 0;
double accX, accY, accZ,gyroX, gyroY, gyroZ;
double gyroXangle, gyroYangle,compAngleX, compAngleY,kalAngleX, kalAngleY;
int16_t tempRaw;
uint32_t timer;
uint8_t i2cData[14];

bool InertMUInitialized  = false;

//constructor
IMU::IMU()
{
}
void IMU::initialize()
{
	
	//need to set up the magnetometer

	if (readI2C(IMU_ADDRESS, WHO_AM_I) != 0x73)
	{
		changeLCDText("Error reading Sensor");
		while(1); //lol
	}

	writeI2C(IMU_ADDRESS,PWR_MGMT_1, 0x00); //Not sleep + clock 20 MHz
	writeI2C(IMU_ADDRESS,SMPLRT_DIV, 0x07); //divide sample rate by 8
	writeI2C(IMU_ADDRESS,CONFIG, 0x06); //set low pass filter to 5Hz bandwidth
	writeI2C(IMU_ADDRESS,GYRO_CONFIG, 0x10); //set gyro full scale to 1000 dps
	writeI2C(IMU_ADDRESS,ACCEL_CONFIG, 0x01);//set accel full scale to 4g

	//BMP180_Init();
	InertMUInitialized  = true;
}

void IMU::TakeMeasures()
{
	uint8_t data[18];
	if(readI2C(IMU_ADDRESS,ACCEL_XOUT_H, data,18) == 0)//problem with read I2C for more than 1 data. 
	{
		accx = (data[0] << 8) | data[1];
		accy = (data[2] << 8) | data[3];
		accz = (data[4] << 8) | data[5];
		temp = (data[6] << 8) | data[7];
		gyrx = (data[8] << 8) | data[9];
		gyry = (data[10] << 8) | data[11];
		gyrz = (data[12] << 8) | data[13];
		pres = (data[14] << 8) | data[15]; //to set up
		pres = (data[16] << 8) | data[17]; // to set
	}
	//CalTemperatureAndPressureAndAltitude();
	//pres = PressureVal;
	//temp = TemperatureVal;
	//alt =  AltitudeVal;
}