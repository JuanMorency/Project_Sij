/**
******************************************************************************
* File Name         : MPU9255.cpp
* Description       : methods for data acquisition via I2C from the MPU9255
* Author			: Juan Morency Trudel
* Version           : 1.0.0
* Date				: June 2017
******************************************************************************
*/

#include "MPU9255.h"

bool mpu9255DataReady = false;


/** 
 * @brief Default constructor, uses default I2C address.
 * @see MPU9255_DEFAULT_ADDRESS
 */
MPU9255::MPU9255() {
    devAddr = MPU9255_ADDRESS<<1;
}

/** 
 * @brief Specific address constructor.
 * @param address I2C address
 * @see MPU9255_DEFAULT_ADDRESS
 * @see MPU9255_ADDRESS_AD0_LOW
 * @see MPU9255_ADDRESS_AD0_HIGH
 */
MPU9255::MPU9255(uint8_t address) {
    devAddr = address;
}

/** 
 * @brief Verify the I2C connection.
 * Make sure the device is connected and responds as expected.
 * @return True if connection is valid, false otherwise
 */
bool MPU9255::testConnection() {
	if (readI2C(devAddr, MPU9255_RA_WHO_AM_I) == 0x73){return true;}
	return false;
}

/** 
 * @brief Power on and prepare for general usage.
 * This will activate the device and take it out of sleep mode (which must be done
 * after start-up). This function also sets both the accelerometer and the gyroscope
 * to the appropriate sensitivity and sets
 * the clock source to use the X Gyro for reference, which is slightly better than
 * the default internal clock source. It also sets the digital low pass filter to 
 * the appropriate bandwidth. Sets up interrupts
 */
void MPU9255::initialize() {
	if(!(this->testConnection()))
	{
		turnDebugLedOn(1);
	}
	writeI2C(devAddr,MPU9255_RA_GYRO_CONFIG, MPU9255_GYRO_FS_1000<<3);
	writeI2C(devAddr,MPU9255_RA_ACCEL_CONFIG, MPU9255_ACCEL_FS_2<<3);
	writeI2C(devAddr,MPU9255_RA_ACCEL_CONFIG_2, MPU9255_DLPF_BW_5); //set low pass filter for acc to 5 Hz bandwidth


	////get the offset values for the accelerometer
	//if(readI2C(devAddr,MPU9255_RA_XA_OFFS_H, bufferI2C,6) == 0)
	//{
		//// the number is stored in 15 bits and not 16 in the MPU registers
		//accOffset.X = (bufferI2C[0] << 8) | (bufferI2C[1]&0xFE);
		//accOffset.Y = (bufferI2C[2] << 8) | (bufferI2C[3]&0xFE);
		//accOffset.Z = (bufferI2C[4] << 8) | (bufferI2C[5]&0xFE);
		//
		//accOffset.X = accOffset.X>>1;
		//accOffset.Y = accOffset.Y>>1;
		//accOffset.Z = accOffset.Z>>1;
	//}	
	//Seems better without offsets so don't use them
	
	writeI2C(devAddr,MPU9255_RA_PWR_MGMT_1, 0x02); //Not sleep + clock 20 MHz
	_delay_ms(10);		
	initGyrOffset(); //this will take 130 ms
	//set the LPF for the gyroscope and set the sample rate to the desired value
	writeI2C(devAddr,MPU9255_RA_SMPLRT_DIV, 0x13); //divide sample rate by 20 to have a 50 Hz sample rate for interrupts
	writeI2C(devAddr,MPU9255_RA_CONFIG, MPU9255_DLPF_BW_5); //set low pass filter for gyro and temp to 5 Hz bandwidth
	// check this reference for LPF bandwidth. might want to put higher
	// https://ulrichbuschbaum.wordpress.com/2015/01/18/using-the-mpu6050s-dlpf/
	
	//Interrupts on MPU9255 and making AK8973 available on the I2C bus
	//This makes the MPU9255 release the interrupt when the data is read and also allows access to AK8973 through the aux I2C bus
	writeI2C(devAddr,MPU9255_RA_INT_PIN_CFG, (1<<MPU9255_INTCFG_INT_RD_CLEAR_BIT)|(1<<MPU9255_INTCFG_I2C_BYPASS_EN_BIT));
	writeI2C(devAddr,MPU9255_RA_INT_ENABLE, 1<<0); //enable interrupt for raw data ready
	writeI2C(devAddr,MPU9255_RA_I2C_MST_CTRL, 1<<6); //delays the data ready interrupt to make sure data has been loaded to the registers
}

/**
  * @brief  Initializes gyroscopes offset by taking 128 measures and taking a mean
  */
void MPU9255::initGyrOffset()
{
	uint8_t i;
	int32_t	TempGx = 0, TempGy = 0, TempGz = 0;
	
 	for(i = 0; i < 128; i ++)
 	{
		static uint16_t gyro[3];
		if(readI2C(devAddr,MPU9255_RA_GYRO_XOUT_H, bufferI2C,6) == 0)
		{
			gyro[0] = (bufferI2C[0] << 8) | bufferI2C[1];
			gyro[1] = (bufferI2C[2] << 8) | bufferI2C[3];
			gyro[2] = (bufferI2C[4] << 8) | bufferI2C[5];
		}
		TempGx += gyro[0];
		TempGy += gyro[1];
		TempGz += gyro[2];
		_delay_ms(1);
	}
	gyrOffset.X = 0;
	gyrOffset.Y = 0;
	gyrOffset.Z = 0;
	if((TempGx >> 7) <= GYR_OFFSET_MAX_CALIBRATION){gyrOffset.X = TempGx >> 7;}
	if((TempGy >> 7) <= GYR_OFFSET_MAX_CALIBRATION){gyrOffset.Y = TempGy >> 7;}
	if((TempGz >> 7) <= GYR_OFFSET_MAX_CALIBRATION){gyrOffset.Z = TempGz >> 7;}
}

/**
  * @brief  Reads the raw values in the MPU9255 registers and updates the objects variables
  */
void MPU9255::updateRawData()
{
	if(readI2C(devAddr,MPU9255_RA_ACCEL_XOUT_H, bufferI2C,14) == 0)
	{
		accRaw.X = (bufferI2C[0] << 8) | bufferI2C[1];
		accRaw.Y = (bufferI2C[2] << 8) | bufferI2C[3];
		accRaw.Z = (bufferI2C[4] << 8) | bufferI2C[5];
		tempRaw =  (bufferI2C[6] << 8) | bufferI2C[7];
		gyrRaw.X = (bufferI2C[8] << 8) | bufferI2C[9];
		gyrRaw.Y = (bufferI2C[10] << 8) | bufferI2C[11];
		gyrRaw.Z = (bufferI2C[12] << 8) | bufferI2C[13];
	}
	else{
		turnDebugLedOn(3);
	}
}

/**
* @brief Updates the objects raw sensor data variables with data from the buffer filled by the I2C FSM
  */
void MPU9255::updateRawDataInterrupt()
{
	accRaw.X = (dataReadBuffer[0] << 8) | dataReadBuffer[1];
	accRaw.Y = (dataReadBuffer[2] << 8) | dataReadBuffer[3];
	accRaw.Z = (dataReadBuffer[4] << 8) | dataReadBuffer[5];
	tempRaw =  (dataReadBuffer[6] << 8) | dataReadBuffer[7];
	gyrRaw.X = (dataReadBuffer[8] << 8) | dataReadBuffer[9];
	gyrRaw.Y = (dataReadBuffer[10] << 8) | dataReadBuffer[11];
	gyrRaw.Z = (dataReadBuffer[12] << 8) | dataReadBuffer[13];
}

/**
  * @brief  Updates the value of acc and gyr and temp from the last raw values gotten
  */
void MPU9255::calculateAccRotTemp()
{
	
	//in G x10^(-4)
	acc.X = (int16_t)((int32_t)(accRaw.X)*20000>>15)+ACC_X_OFFSET; //divide by 2^15 which is the max number of int16
	acc.Y = (int16_t)((int32_t)(accRaw.Y)*20000>>15)+ACC_Y_OFFSET;
	acc.Z = (int16_t)((int32_t)(accRaw.Z)*20000>>15)+ACC_Z_OFFSET;

	
	// calculate the resolution to transform the bit value in 0.1 degrees per second
	gyr.X = (int16_t)((int32_t)(gyrRaw.X - gyrOffset.X)*10000>>15); //divide by 2^15 which is the max number of int16
	gyr.Y = (int16_t)((int32_t)(gyrRaw.Y - gyrOffset.Y)*10000>>15);
	gyr.Z = (int16_t)((int32_t)(gyrRaw.Z - gyrOffset.Z)*10000>>15);

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
  * @brief  Returns raw gyroscope measurement
  * @retval XYZ16_TypeDef of the rotation
  */
XYZ16_TypeDef MPU9255::getRawRotation()
{
	return this->gyrRaw;
}

/**
  * @brief  Returns X gyroscope measurement
  * @retval int16_t of the rotation
  */
int16_t MPU9255::getRotationX()
{
	return this->gyr.X;
}

/**
  * @brief  Returns Y gyroscope measurement
  * @retval int16_t of the rotation
  */
int16_t MPU9255::getRotationY()
{
	return this->gyr.Y;
}

/**
  * @brief  Returns Z gyroscope measurement
  * @retval int16_t of the rotation
  */
int16_t MPU9255::getRotationZ()
{
	return this->gyr.Z;
}


/**
  * @brief  Returns accelerometer measurement
  * @retval XYZ16_TypeDef of the raw accelerometer
  */
XYZ16_TypeDef MPU9255::getRawAcceleration()
{
	return this->accRaw;
}


/**
  * @brief  Returns accelerometer measurement
  * @retval XYZ16_TypeDef of the accelerometer
  */
XYZ16_TypeDef MPU9255::getAcceleration()
{
	return this->acc;
}

/**
  * @brief  Returns X accelerometer measurement
  * @retval int16_t of the X accelerometer
  */
int16_t MPU9255::getAccelerationX()
{
	return this->acc.X;
}

/**
  * @brief  Returns Y accelerometer measurement
  * @retval int16_t of the Y accelerometer
  */
int16_t MPU9255::getAccelerationY()
{
	return this->acc.Y;
}

/**
  * @brief  Returns Z accelerometer measurement
  * @retval int16_t of the Z accelerometer
  */
int16_t MPU9255::getAccelerationZ()
{
	return this->acc.Z;
}

/**
  * @brief  Sets the acceleration of the object
  * @param XYZ16_TypeDef of the input acceleration
  */
 void MPU9255::setRawAcceleration(XYZ16_TypeDef inputAcc)
{
	this->accRaw = inputAcc;
}

/**
  * @brief  Sets the rotation of the object
  * @param XYZ16_TypeDef of the input rotation
  */
 void MPU9255::setRawRotation(XYZ16_TypeDef inputGyr)
{
	this->gyrRaw = inputGyr;
}

/**
  * @brief  Sets the temperature of the object
  * @param XYZ16_TypeDef of the input temperature
  */
 void MPU9255::setRawTemperature(uint16_t inputTemp)
{
	this->tempRaw = inputTemp;
}

/**
  * @brief  Measures the mean acceleration and rotation from 10 samples and stores it in acc/gyr
  */
void MPU9255::measureMeanRawAccelerationAndRotation()
{
	static int32_t sumAccX = 0, sumAccY = 0, sumAccZ = 0;
	static int32_t sumRotX = 0, sumRotY = 0, sumRotZ = 0;
	for(int i = 0; i < 5; i++)
	{
		updateRawData();
	}
	for(int i = 0; i < 10; i++)
	{
		updateRawData();
		sumAccX += accRaw.X;
		sumAccY += accRaw.Y;
		sumAccZ += accRaw.Z;
		sumRotX += gyrRaw.X;
		sumRotY += gyrRaw.Y;
		sumRotZ += gyrRaw.Z;
	}
	accRaw.X = sumAccX/10;
	accRaw.Y = sumAccY/10;
	accRaw.Z = sumAccZ/10;
	gyrRaw.X = sumRotX/10;
	gyrRaw.Y = sumRotY/10;
	gyrRaw.Z = sumRotZ/10;
}