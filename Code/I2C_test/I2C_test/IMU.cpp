#include "IMU.h"
#include "BMP180.h"
const unsigned char OSS = 0;
double accX, accY, accZ,gyroX, gyroY, gyroZ;
double gyroXangle, gyroYangle,compAngleX, compAngleY,kalAngleX, kalAngleY;
int16_t tempRaw;
uint32_t timer;
uint8_t i2cData[14];

//constructor
IMU::IMU()
{
}
void IMU::initialize()
{
	writeI2C(IMU_ADDRESS,PWR_MGMT_1, 0x00); //Not sleep + clock 20 MHz
	writeI2C(IMU_ADDRESS,SMPLRT_DIV, 0x07); //divide sample rate by 8
	writeI2C(IMU_ADDRESS,CONFIG, 0x06); //set low pass filter to 5Hz bandwidth
	writeI2C(IMU_ADDRESS,GYRO_CONFIG, 0x10); //set gyro full scale to 1000 dps
	writeI2C(IMU_ADDRESS,ACCEL_CONFIG, 0x01);//set accel full scale to 4g

	if (readI2C(IMU_ADDRESS, WHO_AM_I) != 0x73)
	{
		clearDisplay();
		LCD_WriteString("Error reading Sensor");
		while(1);
	}
	BMP180_Init();
}

void IMU::IMUTakeMeasures()
{
	uint8_t data[18];
	if(readI2C(IMU_ADDRESS,ACCEL_XOUT_H, data,18) == 0)
	{
		accx = (data[0] << 8) | data[1];
		accy = (data[2] << 8) | data[3];
		accz = (data[4] << 8) | data[5];
		accy = (data[6] << 8) | data[7];
		temp = (data[8] << 8) | data[9];
		gyrx = (data[10] << 8) | data[11];
		gyry = (data[12] << 8) | data[13];
		gyrz = (data[14] << 8) | data[15];
		pres = (data[16] << 8) | data[17];
	}
	CalTemperatureAndPressureAndAltitude();
	pres = PressureVal;
	temp = TemperatureVal;
	alt =  AltitudeVal;
}