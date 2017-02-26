#include "AK8963.h"
#include "lcd.h"
#include "MPU9255.h"

/** Default constructor, uses default I2C address.
 * @see AK8963_DEFAULT_ADDRESS
 */
AK8963::AK8963() {
    devAddr = AK8963_ADDRESS<<1;
}

/** Specific address constructor.
 * @param address I2C address
 * @see AK8963_DEFAULT_ADDRESS
 * @see AK8963_ADDRESS_00
 */
AK8963::AK8963(uint8_t address) {
    devAddr = address;
}

/** Power on and prepare for continuous usage.
 */
void AK8963::initialize() {
	if(!(this->testConnection())){turnDebugLedOn(2);}
	this->readAdjustment();
	// Change to appropriate mode
	writeI2C(devAddr, AK8963_RA_CNTL1, AK8963_MODE_CONTINUOUS_100HZ);
	_delay_ms(10);
	mRes = 10.0*4912.0/32760.0; // Proper scale to return milliGauss
	bias.X=0;
	bias.Y=0;
	bias.Z=0;
}

/** Verify the I2C connection.
 * Make sure the device is connected and responds as expected.
 * @return True if connection is valid, false otherwise
 */
bool AK8963::testConnection() {
    if (readI2C(devAddr, AK8963_RA_WIA) == AK8963_WIA_DEVICE_ID) {return true;}
    return false;
}


void AK8963::reset() {
    writeI2C(devAddr, AK8963_RA_CNTL2, 0x01);
}


void AK8963::readAdjustment() {

	// Power down magnetometer
	writeI2C(devAddr, AK8963_RA_CNTL1, AK8963_MODE_POWERDOWN); 
	_delay_ms(10);
	//set mode to fuse access mode
	writeI2C(devAddr, AK8963_RA_CNTL1, AK8963_MODE_FUSEROM);
	_delay_ms(10);
	
	//read the adjustment value
	readI2C(devAddr, AK8963_RA_ASAX, buffer, 3);
	this->adjustment.X = buffer[0];
	this->adjustment.Y = buffer[1];
	this->adjustment.Z = buffer[2];

	 // calculation to do on these adjustments values
	 //verify best way to do this if don't want to use floats
	 adjustment.X =  (float)(adjustmentRaw.X - 128)/256.0f + 1.0f;   // Return x-axis sensitivity adjustment values, etc.
	 adjustment.Y =  (float)(adjustmentRaw.Y - 128)/256.0f + 1.0f;
	 adjustment.Z =  (float)(adjustmentRaw.Z - 128)/256.0f + 1.0f;

	// Power down magnetometer
	writeI2C(devAddr, AK8963_RA_CNTL1, AK8963_MODE_POWERDOWN);
	_delay_ms(10);
	
	
	
	//might need to use reset in this function at some point
}


/**
  * @brief  Reads the raw values in the AK8963 registers and updates the objects variables
  * @param  None
  * @retval None
  */
void AK8963::updateRawData()
{
 
	//Read 7 datas here to store status 2 register in the buffer
	//Status 2 has to be read in continuous operation in order for the sensor to continue
	//updating data. Also it tells us if an overflow occurred


	if(readI2C(devAddr, AK8963_RA_ST1) & (1<<AK8963_ST1_DRDY_BIT)) // update only if data is ready 
	{ 
		if(readI2C(devAddr,AK8963_RA_HXL, buffer,7) == 0)
		{
			if(!(buffer[6] & (1<<AK8963_ST2_HOFL_BIT)))
			{
				magRaw.X = (buffer[1] << 8) | buffer[0];
				magRaw.Y = (buffer[3] << 8) | buffer[2];
				magRaw.Z = (buffer[5] << 8) | buffer[4];
			}
			//debug
			else
			{
				turnDebugLedOn(4);
			}
		}
		//debug
		else{
			turnDebugLedOn(3);
		}		
	}
	//debug
	else
	{
		turnDebugLedOn(7);
	}
}

XYZfloat_TypeDef AK8963::getMagneticField()
{
	return this->mag;
}

/**
  * @brief  Updates the value of mag from the last raw values gotten
  * @param  None
  * @retval None
  */
void AK8963::calculateMag()
{
	this->mag.X = this->magRaw.X*mRes*adjustment.X - bias.X;
	this->mag.Y = this->magRaw.Y*mRes*adjustment.Y - bias.Y;
	this->mag.Z = this->magRaw.Z*mRes*adjustment.Z - bias.Z;
	//TODO Do the calculations to adjust these values with offset and other stuff required
	
	
	
	    //mx = (float)magCount[0]*mRes*magCalibration[0] - magbias[0];  // get actual magnetometer value, this depends on scale being set
	    //my = (float)magCount[1]*mRes*magCalibration[1] - magbias[1];
	    //mz = (float)magCount[2]*mRes*magCalibration[2] - magbias[2];
	    //
	    //void getMres() {
		    //switch (Mscale)
		    //{
			    //// Possible magnetometer scales (and their register bit settings) are:
			    //// 14 bit resolution (0) and 16 bit resolution (1)
			    //case MFS_14BITS:
			    //mRes = 10.0*4912.0/8190.0; // Proper scale to return milliGauss
			    //break;
			    //case MFS_16BITS:
			    //mRes = 10.0*4912.0/32760.0; // Proper scale to return milliGauss
			    //break;
		    //}
	    //}
		//https://github.com/kriswiner/MPU-9250/blob/master/STM32F401/MPU9250.h
}
