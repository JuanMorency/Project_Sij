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

/** Power on and prepare for general usage.
 * No specific pre-configuration is necessary for this device.
 */
void AK8963::initialize() {
	//this->releaseMPU9255I2CMasterBus();
	this->readAdjustment();
	writeI2C(devAddr,AK8963_RA_CNTL1, AK8963_MODE_CONTINUOUS_100HZ); //set in continuous mode 2 and 16 bit output
	if(!(this->testConnection()))
	{
		turnDebugLedOn(2);
	}
}

/** Verify the I2C connection.
 * Make sure the device is connected and responds as expected.
 * @return True if connection is valid, false otherwise
 */
bool AK8963::testConnection() {
    if (readI2C(devAddr, AK8963_RA_WIA) == AK8963_WIA_DEVICE_ID) {return true;}
	char buffer1[20];
	sprintf(buffer1, "devID:%x", readI2C(devAddr, AK8963_RA_CNTL2));
	changeLCDText(buffer1,buffer1);
    return false;
}


void AK8963::reset() {
    writeI2C(devAddr, AK8963_RA_CNTL2, 0x01);
}

//// CNTL2 register
//void AK8963::releaseMPU9255I2CMasterBus() {
	//writeI2C(MPU9255_ADDRESS,MPU9255_RA_INT_PIN_CFG, (1<<MPU9255_INTCFG_INT_RD_CLEAR_BIT)|(1<<MPU9255_INTCFG_I2C_BYPASS_EN_BIT));
//}

void AK8963::readAdjustment() {
	//set mode to fuse access mode
	buffer[0] = writeI2C(devAddr, AK8963_RA_CNTL1, AK8963_MODE_FUSEROM);
	
	char data[20];
	//debug
	sprintf(data, "I2Cdebug:%u", buffer[0]);
	changeLCDText(data,data);
	
	
	//read the adjustment value
	readI2C(devAddr, AK8963_RA_ASAX, buffer, 3);
	this->adjustment.X = buffer[0];
	this->adjustment.Y = buffer[1];
	this->adjustment.Z = buffer[2];

	//set mode back to 
	writeI2C(devAddr, AK8963_RA_CNTL2, AK8963_MODE_CONTINUOUS_100HZ);
	
	
	//might need to use reset in this function at some point
}


/**
  * @brief  Reads the raw values in the AK8963 registers and updates the objects variables
  * @param  None
  * @retval None
  */
void AK8963::updateRawData()
{
	if(readI2C(devAddr,AK8963_RA_HXL, buffer,6) == 0)
	{
		magRaw.X = (buffer[0] << 8) | buffer[1];
		magRaw.Y = (buffer[2] << 8) | buffer[3];
		magRaw.Z = (buffer[4] << 8) | buffer[5];
	}
	else{
		turnDebugLedOn(3);
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
	this->mag = this->magRaw;
	//TODO Do the calculations to adjust these values with offset and other stuff required
}
