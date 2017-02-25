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

/** Power on and prepare for continous usage.
 */
void AK8963::initialize() {
	this->readAdjustment();
	writeI2C(devAddr,AK8963_RA_CNTL1, AK8963_MODE_CONTINUOUS_100HZ); //set in continuous mode 2 and 16 bit output
	if(!(this->testConnection())){turnDebugLedOn(2);}
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

//// CNTL2 register
//void AK8963::releaseMPU9255I2CMasterBus() {
	//writeI2C(MPU9255_ADDRESS,MPU9255_RA_INT_PIN_CFG, (1<<MPU9255_INTCFG_INT_RD_CLEAR_BIT)|(1<<MPU9255_INTCFG_I2C_BYPASS_EN_BIT));
//}

void AK8963::readAdjustment() {
	//set mode to fuse access mode
	buffer[0] = writeI2C(devAddr, AK8963_RA_CNTL1, AK8963_MODE_FUSEROM);
	
	//might need a little delay here
	
	//read the adjustment value
	readI2C(devAddr, AK8963_RA_ASAX, buffer, 3);
	this->adjustment.X = buffer[0];
	this->adjustment.Y = buffer[1];
	this->adjustment.Z = buffer[2];

	////set mode back to 
	//writeI2C(devAddr, AK8963_RA_CNTL2, AK8963_MODE_CONTINUOUS_100HZ);
	
	
	//might need to use reset in this function at some point
}


/**
  * @brief  Reads the raw values in the AK8963 registers and updates the objects variables
  * @param  None
  * @retval None
  */
void AK8963::updateRawData()
{
	
	//writeI2C(devAddr,AK8963_RA_CNTL1, AK8963_MODE_SINGLE);
	//char data[20];
	//sprintf(data, "mag:%i", readI2C(devAddr,AK8963_RA_HYH));
	//changeLCDText(data, data);
	//writeI2C(MPU9255_ADDRESS,MPU9255_RA_INT_PIN_CFG, (1<<MPU9255_INTCFG_INT_RD_CLEAR_BIT)|(1<<MPU9255_INTCFG_I2C_BYPASS_EN_BIT));
	//_delay_ms(1);
	
	
	if(readI2C(devAddr,AK8963_RA_HXL, buffer,6) == 0)
	{
		magRaw.X = (buffer[1] << 8) | buffer[0];
		magRaw.Y = (buffer[3] << 8) | buffer[2];
		magRaw.Z = (buffer[5] << 8) | buffer[4];
	}
	else{
		turnDebugLedOn(3);
	}
	
	////debug
	//char data[20];
	//char data2[20];
	//sprintf(data, "x %i y %i",magRaw.X,magRaw.Y);
	//sprintf(data2, "z :%i",magRaw.Z);
	//changeLCDText(data, data2);
	////end debug
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
