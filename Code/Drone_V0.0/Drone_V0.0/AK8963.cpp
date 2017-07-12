/**
******************************************************************************
* File Name         : AK8963.cpp
* Description       : Initialization and communication functions for the AK8963 magnetic sensor embedded in the MPU9255
* Author			: Juan Morency Trudel, Simon Poirier
* Version           : 1.0.0
* Date				: June 2017
******************************************************************************
*/

#include "AK8963.h"
#include "MPU9255.h"

bool ak8963Initialized = false; 
bool ak8963DataReady = false;	

/** 
 * @brief Default constructor, uses default I2C address.
 */
AK8963::AK8963() {
	//shift on address because 7 bits
    devAddr = AK8963_ADDRESS<<1;
}

/** 
 * @brief Specific address constructor.
 * @param address I2C address
 */
AK8963::AK8963(uint8_t address) {
    devAddr = address;
}

/** 
 * @brief Power on and prepare for continuous usage.
 */
void AK8963::initialize() {
	if(!(this->testConnection())){turnDebugLedOn(3);}
	this->readAdjustment();
	// Change to appropriate mode
	writeI2C(devAddr, AK8963_RA_CNTL1, AK8963_MODE_CONTINUOUS_100HZ);
	_delay_ms(10);
	bias.X=AK8973_MAG_OFFSET_X;
	bias.Y=AK8973_MAG_OFFSET_Y;
	bias.Z=AK8973_MAG_OFFSET_Z;
	ak8963Initialized = true; 
}

/** 
 * @brief Verify the I2C connection.
 * Make sure the device is connected and responds as expected.
 * @retval True if connection is valid, false otherwise
 */
bool AK8963::testConnection() {
    if (readI2C(devAddr, AK8963_RA_WIA) == AK8963_WIA_DEVICE_ID) {return true;}
    return false;
}

/** 
 * @brief Resets the AK8963
 */
void AK8963::reset() {
    writeI2C(devAddr, AK8963_RA_CNTL2, 0x01);
}

/** 
 * @brief Reads the adjustments values via I2C on the AK8963 and
 * store those in the adjustmentRaw variable.
 * Powers down magnetometer after use, hence make sure to start
 * it again after use
 */
void AK8963::readAdjustment() {

	// Power down magnetometer
	writeI2C(devAddr, AK8963_RA_CNTL1, AK8963_MODE_POWERDOWN); 
	_delay_ms(10);
	//set mode to fuse access mode
	writeI2C(devAddr, AK8963_RA_CNTL1, AK8963_MODE_FUSEROM);
	_delay_ms(10);
	
	//read the adjustment value
	readI2C(devAddr, AK8963_RA_ASAX, buffer, 3);
	this->adjustmentRaw.X = buffer[0];
	this->adjustmentRaw.Y = buffer[1];
	this->adjustmentRaw.Z = buffer[2];
	
	// Power down magnetometer
	writeI2C(devAddr, AK8963_RA_CNTL1, AK8963_MODE_POWERDOWN);
	_delay_ms(10);
}


/**
  * @brief  Reads the raw values in the AK8963 registers and updates the objects variables
  */
void AK8963::updateRawData()
{
 
	//Read 7 datas here to store status 2 register in the buffer
	//Status 2 has to be read in continuous operation in order for the sensor to continue
	//updating data. Also it tells us if an overflow occurred


	if(readI2C(devAddr, AK8963_RA_ST1) & (1<<AK8963_ST1_DRDY_BIT)) // update only if data is ready 
	{ 
		if(readI2C(devAddr,AK8963_RA_HXL, buffer,7) == 0) // update only if I2C gets no errors
		{
			if(!(buffer[6] & (1<<AK8963_ST2_HOFL_BIT))) // update only if no overflow
			{
				magRaw.X = (buffer[1] << 8) | buffer[0];
				magRaw.Y = (buffer[3] << 8) | buffer[2];
				magRaw.Z = (buffer[5] << 8) | buffer[4];
			}
			//debug
			else
			{
				turnDebugLedOn(4); // data not ready
			}
		}
		//debug
		else{
			turnDebugLedOn(3); // I2C error
		}		
	}
	//debug
	else
	{
		turnDebugLedOn(7); // AK8963 overflow
	}
}

/**
  * @brief  Returns the magnetic field in an int_16 format. 
  * the updateRawData must be called before this to ensure the latest data
  */
XYZ16_TypeDef AK8963::getMagneticField()
{
	return this->mag;
}

/**
  * @brief  Returns the X magnetic field in an int_16 format. 
  * the updateRawData must be called before this to ensure the latest data
  */
int16_t AK8963::getMagneticFieldX()
{
	return this->mag.X;
}

/**
  * @brief  Returns the Y magnetic field in an int_16 format. 
  * the updateRawData must be called before this to ensure the latest data
  */
int16_t AK8963::getMagneticFieldY()
{
	return this->mag.Y;
}

/**
  * @brief  Returns the Y magnetic field in an int_16 format. 
  * the updateRawData must be called before this to ensure the latest data
  */
int16_t AK8963::getMagneticFieldZ()
{
	return this->mag.Z;
}

/**
  * @brief  Updates the value of mag from the last raw values gotten
  */
void AK8963::calculateMag()
{
	//resolution is 32768
	//full scale mag field is 4912 micro Tesla = 49120 milliGauss
	//Formula for adjustment: Hadj = H*(adj+128)/256
	//Full formula gives:
	
	mag.X = (int16_t)(((int32_t)this->magRaw.X*49120>>7)*(adjustmentRaw.X+128)>>16) + bias.X;
	mag.Y = (int16_t)(((int32_t)this->magRaw.Y*49120>>7)*(adjustmentRaw.Y+128)>>16) + bias.Y;
	mag.Z = (int16_t)(((int32_t)this->magRaw.Z*49120>>7)*(adjustmentRaw.Z+128)>>16) + bias.Z;
}

/**
  * @brief  Sets the magnetic field strength of the object
  * @param XYZ16_TypeDef of the input magnetic field strength
  */
void AK8963::setRawMagneticField(XYZ16_TypeDef inputMag)
{
	this->magRaw = inputMag;
}