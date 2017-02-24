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
	//need to set up the magnetometer

	writeI2C(MP9255_ADDRESS,SMPLRT_DIV, 0x07); //divide sample rate by 8
	writeI2C(MP9255_ADDRESS,CONFIG, 0x06); //set low pass filter to 5Hz bandwidth
	writeI2C(MP9255_ADDRESS,GYRO_CONFIG, 0x10); //set gyro full scale to 1000 dps
	writeI2C(MP9255_ADDRESS,ACCEL_CONFIG, 0x01);//set accel full scale to 4g
	writeI2C(MP9255_ADDRESS,PWR_MGMT_1, 0x01); //Not sleep + clock 20 MHz
	
	if (readI2C(MP9255_ADDRESS, WHO_AM_I) != 0x73)
	{
		changeLCDText("Error reading Sensor");
		while(1); //lol
	}
	_delay_ms(100); 
	//BMP180_Init();
	InertMUInitialized  = true;

}

void IMU::takeMeasures()
{
	static uint8_t data[20];
	if(readI2C(MP9255_ADDRESS,ACCEL_XOUT_H, data,14) == 0) //problem with read I2C for more than 1 data. 
	{
		accRaw.X = (data[0] << 8) | data[1];
		accRaw.Y = (data[2] << 8) | data[3];
		accRaw.Z = (data[4] << 8) | data[5];
		tempRaw =	(data[6] << 8) | data[7];
		gyroRaw.X = (data[8] << 8) | data[9];
		gyroRaw.Y = (data[10] << 8) | data[11];
		gyroRaw.Z = (data[12] << 8) | data[13];
	}

    //writeI2C(MP9255_ADDRESS,0x37,0x02);//turn on Bypass Mode 
    //_delay_ms(10);
    //writeI2C(AK8963_ADDRESS,0x0A,0x01);
    //_delay_ms(10);
    //data[14]=readI2C(AK8963_ADDRESS,MAG_XOUT_L);
    //data[15]=readI2C(AK8963_ADDRESS,MAG_XOUT_H);
    //data[16]=readI2C(AK8963_ADDRESS,MAG_YOUT_L);
    //data[17]=readI2C(AK8963_ADDRESS,MAG_YOUT_H);
    ////data[18]=readI2C(AK8963_ADDRESS,MAG_ZOUT_L);
    ////data[19]=readI2C(AK8963_ADDRESS,MAG_ZOUT_H);
//
	//magRaw.X = (data[15] << 8) | data[14];
	//magRaw.Y = (data[17] << 8) | data[16];
	//magRaw.Z = (data[19] << 8) | data[18];	

	//CalTemperatureAndPressureAndAltitude();
	//pres = PressureVal;
	//temp = TemperatureVal;
	//alt =  AltitudeVal;
}



/**
  * @brief  Initializes gyroscopes offset
  * @param  None
  * @retval None
  */
void IMU::initGyrOffset(void)
{
	uint8_t i;
	int32_t	TempGx = 0, TempGy = 0, TempGz = 0;
	
 	for(i = 0; i < 32; i ++)
 	{
		static uint8_t data[6];
		static uint16_t gyro[3];
		if(readI2C(MP9255_ADDRESS,GYRO_XOUT_H, data,6) == 0)//problem with read I2C for more than 1 data.
		{
			gyro[0] = (data[0] << 8) | data[1];
			gyro[1] = (data[2] << 8) | data[3];
			gyro[2] = (data[4] << 8) | data[5];
		}
		TempGx += gyro[0];
		TempGy += gyro[1];
		TempGz += gyro[2];
		_delay_us(100);
	}

	gyrOffset.X = TempGx >> 5;
	gyrOffset.Y = TempGy >> 5;
	gyrOffset.Z = TempGz >> 5;

}