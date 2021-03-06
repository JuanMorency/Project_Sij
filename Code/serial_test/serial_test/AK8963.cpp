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
	bias.X=AK8973_MAG_OFFSET_X;
	bias.Y=AK8973_MAG_OFFSET_Y;
	bias.Z=AK8973_MAG_OFFSET_Z;
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
	this->adjustmentRaw.X = buffer[0];
	this->adjustmentRaw.Y = buffer[1];
	this->adjustmentRaw.Z = buffer[2];
	
	// Power down magnetometer
	writeI2C(devAddr, AK8963_RA_CNTL1, AK8963_MODE_POWERDOWN);
	_delay_ms(10);
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

XYZ16_TypeDef AK8963::getMagneticField()
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
	//resolution is 32768
	//full scale mag field is 4912 micro Tesla = 49120 milliGauss
	//Formula for adjustment: Hadj = H*(adj+128)/256
	//Full formula gives:
	
	mag.X = (int16_t)(((int32_t)this->magRaw.X*49120>>7)*(adjustmentRaw.X+128)>>16) - bias.X;
	mag.Y = (int16_t)(((int32_t)this->magRaw.Y*49120>>7)*(adjustmentRaw.Y+128)>>16) - bias.Y;
	mag.Z = (int16_t)(((int32_t)this->magRaw.Z*49120>>7)*(adjustmentRaw.Z+128)>>16) - bias.Z;
}
