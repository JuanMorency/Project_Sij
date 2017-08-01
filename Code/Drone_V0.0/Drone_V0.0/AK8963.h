/**
******************************************************************************
* File Name         : AK8963.h
* Description       : header file for AK8963.cpp
* Author			: Juan Morency Trudel
* Version           : 1.0.0
* Date				: June 2017
******************************************************************************
*/

#ifndef _AK8963_H_
#define _AK8963_H_

#include "I2C.h"
#include "debugLED.h"
#include "typeDef.h"

#define AK8963_ADDRESS_00               0x0C
#define AK8963_ADDRESS_01               0x0D
#define AK8963_ADDRESS_10               0x0E
#define AK8963_ADDRESS_11               0x0F
#define AK8963_ADDRESS					AK8963_ADDRESS_00

#define AK8963_RA_WIA                   0x00
#define AK8963_RA_INFO                  0x01
#define AK8963_RA_ST1                   0x02
#define AK8963_RA_HXL                   0x03
#define AK8963_RA_HXH                   0x04
#define AK8963_RA_HYL                   0x05
#define AK8963_RA_HYH                   0x06
#define AK8963_RA_HZL                   0x07
#define AK8963_RA_HZH                   0x08
#define AK8963_RA_ST2                   0x09
#define AK8963_RA_CNTL1                 0x0A
#define AK8963_RA_CNTL2                 0x0B
#define AK8963_RA_ASTC                  0x0C
#define AK8963_RA_TS1                   0x0D // SHIPMENT TEST, DO NOT USE
#define AK8963_RA_TS2                   0x0E // SHIPMENT TEST, DO NOT USE
#define AK8963_RA_I2CDIS                0x0F
#define AK8963_RA_ASAX                  0x10
#define AK8963_RA_ASAY                  0x11
#define AK8963_RA_ASAZ                  0x12

#define AK8963_ST1_DRDY_BIT             0
#define AK8963_ST1_DOR_BIT              1

#define AK8963_ST2_HOFL_BIT             3
#define AK8963_ST2_BITM_BIT             4

#define AK8963_CNTL1_MODE_BIT           3
#define AK8963_CNTL1_MODE_LENGTH        4
#define AK8963_CNTL1_RES_BIT            4

#define AK8963_CNTL2_RESET              0x01


//These have BIT enables to have a 16 bit output
#define AK8963_MODE_POWERDOWN           0x00
#define AK8963_MODE_SINGLE              0x01
#define AK8963_MODE_CONTINUOUS_8HZ      0x12
#define AK8963_MODE_EXTERNAL            0x14
#define AK8963_MODE_CONTINUOUS_100HZ    0x16
#define AK8963_MODE_SELFTEST            0x18
#define AK8963_MODE_FUSEROM             0x1F

#define AK8963_RES_14_BIT               0
#define AK8963_RES_16_BIT               1

#define AK8963_CNTL2_SRST_BIT           0

#define AK8963_ASTC_SELF_BIT            6

#define AK8963_I2CDIS_DISABLE           0x1B

#define AK8963_WIA_DEVICE_ID			0x48



// very approximate calibration values gotten from Juan's room
// Your own calibration should be done by orientating yourself to 
// true north 
//#define AK8973_MAG_OFFSET_X				-5		//milliGauss
//#define AK8973_MAG_OFFSET_Y				-34		//milliGauss
//#define AK8973_MAG_OFFSET_Z				-522	//milliGauss


//x -590 to 310
//y -260 to 640
//z -750 to 180

#define AK8973_MAG_OFFSET_X				140		//milliGauss
#define AK8973_MAG_OFFSET_Y				-190	//milliGauss
#define AK8973_MAG_OFFSET_Z				285		//milliGauss

extern bool ak8963Initialized; 
extern bool ak8963DataReady;

class AK8963 {
    public:
        AK8963();
        AK8963(uint8_t address);

        void initialize();
        bool testConnection();
        void reset();
		void readAdjustment();
		XYZ16_TypeDef getMagneticField();
		XYZ16_TypeDef getRawMagneticField();
		int16_t getMagneticFieldX();
		int16_t getMagneticFieldY();
		int16_t getMagneticFieldZ();
		void calculateMag();
		void updateRawData();
		void setRawMagneticField(XYZ16_TypeDef inputMag);
		void measureMeanRawMagneticField();
				
    private:
        uint8_t devAddr;
        uint8_t buffer[7];
        uint8_t mode;
		//mag is in milliGauss
		//Earth magnetic field is around 530 milligauss at our location
		XYZ16_TypeDef mag, magRaw, adjustmentRaw, bias;

};

#endif /* _AK8963_H_ */
