#include "MPU9255.h"

/** Default constructor, uses default I2C address.
 * @see MPU9255_DEFAULT_ADDRESS
 */
MPU9255::MPU9255() {
    devAddr = MPU9255_ADDRESS<<1;
}

/** Specific address constructor.
 * @param address I2C address
 * @see MPU9255_DEFAULT_ADDRESS
 * @see MPU9255_ADDRESS_AD0_LOW
 * @see MPU9255_ADDRESS_AD0_HIGH
 */
MPU9255::MPU9255(uint8_t address) {
    devAddr = address;
}

/** Verify the I2C connection.
 * Make sure the device is connected and responds as expected.
 * @return True if connection is valid, false otherwise
 */
bool MPU9255::testConnection() {
	if (readI2C(devAddr, MPU9255_RA_WHO_AM_I) == 0x73){return true;}
	return false;
}

/** Power on and prepare for general usage.
 * This will activate the device and take it out of sleep mode (which must be done
 * after start-up). This function also sets both the accelerometer and the gyroscope
 * to the appropriate sensitivity and sets
 * the clock source to use the X Gyro for reference, which is slightly better than
 * the default internal clock source. It also sets the digital low pass filter to 
 * the appropriate bandwidth. Sets up interrupts
 */
void MPU9255::initialize() {
	writeI2C(devAddr,MPU9255_RA_SMPLRT_DIV, 0x13); //divide sample rate by 20 to have a 50 Hz sample rate for interrupts
	writeI2C(devAddr,MPU9255_RA_CONFIG, MPU9255_DLPF_BW_5); //set low pass filter for gyro and temp to 5 Hz bandwidth
	writeI2C(devAddr,MPU9255_RA_GYRO_CONFIG, MPU9255_GYRO_FS_1000<<3); 
	writeI2C(devAddr,MPU9255_RA_ACCEL_CONFIG, MPU9255_ACCEL_FS_4<<3);
	writeI2C(devAddr,MPU9255_RA_ACCEL_CONFIG_2, MPU9255_DLPF_BW_5); //set low pass filter for acc to 5 Hz bandwidth
	// check this reference for LPF 
	// https://ulrichbuschbaum.wordpress.com/2015/01/18/using-the-mpu6050s-dlpf/
	
	writeI2C(devAddr,MPU9255_RA_PWR_MGMT_1, 0x01); //Not sleep + clock 20 MHz
	//This makes the MPU9255 release the interrupt when the data is read and also allows access to AK8973 through the slave I2C bus
	writeI2C(MPU9255_ADDRESS,MPU9255_RA_INT_PIN_CFG, (1<<MPU9255_INTCFG_INT_RD_CLEAR_BIT)|(1<<MPU9255_INTCFG_I2C_BYPASS_EN_BIT));
	writeI2C(devAddr,MPU9255_RA_INT_ENABLE, 1<<0); //enable interrupt for raw data ready
	writeI2C(devAddr,MPU9255_RA_I2C_MST_CTRL, 1<<6); //delays the data ready interrupt to make sure data has been loaded to the registers
	
	EICRA |= (1<<ISC20)|(1<<ISC21); /* Sets the rising edge of INT2 to trigger interrupts */
	EIMSK |= (1<<INT2);	/* Enables INT2 */
	
	if(!(this->testConnection()))
	{
		turnDebugLedOn(1);
	}
}

/**
  * @brief  Initializes gyroscopes offset
  * @param  None
  * @retval None
  */
void MPU9255::initGyrOffset()
{
	uint8_t i;
	int32_t	TempGx = 0, TempGy = 0, TempGz = 0;
	
 	for(i = 0; i < 32; i ++)
 	{
		static uint8_t data[6];
		static uint16_t gyro[3];
		if(readI2C(devAddr,MPU9255_RA_GYRO_XOUT_H, data,6) == 0)//problem with read I2C for more than 1 data.
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

/**
  * @brief  Reads the raw values in the MPU9255 registers and updates the objects variables
  * @param  None
  * @retval None
  */
void MPU9255::updateRawData()
{
	if(readI2C(devAddr,MPU9255_RA_ACCEL_XOUT_H, buffer,14) == 0)
	{
		accRaw.X = (buffer[0] << 8) | buffer[1];
		accRaw.Y = (buffer[2] << 8) | buffer[3];
		accRaw.Z = (buffer[4] << 8) | buffer[5];
		tempRaw =	(buffer[6] << 8) | buffer[7];
		gyrRaw.X = (buffer[8] << 8) | buffer[9];
		gyrRaw.Y = (buffer[10] << 8) | buffer[11];
		gyrRaw.Z = (buffer[12] << 8) | buffer[13];
	}
	else{
		turnDebugLedOn(3);
	}
}

/**
  * @brief  Updates the value of acc and gyr and temp from the last raw values gotten
  * @param  None
  * @retval None
  */
void MPU9255::calculateAccRotTemp()
{
	this->temp = this->tempRaw;
	this->acc = this->accRaw;
	this->gyr = this->gyrRaw;
	
	//TODO Do the calculations to adjust these values with offset and other stuff required
}


/**
  * @brief  Returns temperature
  * @retval int16_t of the temperature
  */
int16_t MPU9255::getTemperature()
{
	return this->temp;
}

/**
  * @brief  Returns gyroscope measurement
  * @retval XYZ16_TypeDef of the rotation
  */
XYZ16_TypeDef MPU9255::getRotation()
{
	return this->gyr;
}

/**
  * @brief  Returns accelerometer measurement
  * @retval XYZ16_TypeDef of the accelerometer
  */
XYZ16_TypeDef MPU9255::getAcceleration()
{
	return this->acc;
}