#include "IMU.h"

int ac1,ac2,ac3,b1,b2,mb,mc,md;
unsigned int ac4 ,ac5 ,ac6;


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
void IMU::initialize(I2C i2c)
{
	bmp180Calibration(i2c);
	TWBR = ((F_CPU / 400000L) - 16) / 2;

	i2c.write(IMU_ADDRESS,PWR_MGMT_1, 0x00); //Not sleep + clock 20 MHz
	i2c.write(IMU_ADDRESS,SMPLRT_DIV, 0x07); //divide sample rate by 8
	i2c.write(IMU_ADDRESS,CONFIG, 0x06); //set low pass filter to 5Hz bandwidth
	i2c.write(IMU_ADDRESS,GYRO_CONFIG, 0x10); //set gyro full scale to 1000 dps
	i2c.write(IMU_ADDRESS,ACCEL_CONFIG, 0x01);//set accel full scale to 4g

	if (i2c.read(IMU_ADDRESS, WHO_AM_I) != 0x73)
	{
		clearDisplay();
		LCD_WriteString("Error reading Sensor");
		while(1);
	}
}

void IMU::IMUTakeMeasures(I2C i2c)
{
	uint8_t data[18];
	if(i2c.read(IMU_ADDRESS,ACCEL_XOUT_H, data,18) == 0)
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
}


void bmp180Calibration(I2C i2c)
{
	ac1 = i2c.read(BMP180_ADDRESS, 0xAA);
	ac2 = i2c.read(BMP180_ADDRESS, 0xAC);
	ac3 = i2c.read(BMP180_ADDRESS, 0xAE);
	ac4 = i2c.read(BMP180_ADDRESS, 0xB0);
	ac5 = i2c.read(BMP180_ADDRESS, 0xB2);
	ac6 = i2c.read(BMP180_ADDRESS, 0xB4);
	b1 = i2c.read(BMP180_ADDRESS, 0xB6);
	b2 = i2c.read(BMP180_ADDRESS, 0xB8);
	mb = i2c.read(BMP180_ADDRESS, 0xBA);
	mc = i2c.read(BMP180_ADDRESS, 0xBC);
	md = i2c.read(BMP180_ADDRESS, 0xBE);
}

/**
  * @brief  Initializes gyroscopes offset
  * @param  None
  * @retval None
  */
void InitGyrOffset()
{
	uint8_t i;
	int32_t	TempGx = 0, TempGy = 0, TempGz = 0;
	
 	for(i = 0; i < 32; i ++)
 	{
		MPU9250_READ_GYRO();
		
		TempGx += gyro[0];
		TempGy += gyro[1];
		TempGz += gyro[2];

		Delay_us(100);
	}

	MPU9250_Offset.X = TempGx >> 5;
	MPU9250_Offset.Y = TempGy >> 5;
	MPU9250_Offset.Z = TempGz >> 5;

}
