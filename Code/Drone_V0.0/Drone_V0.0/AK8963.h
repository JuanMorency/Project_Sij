// I2Cdev library collection - AK8963 I2C device class header file
// Based on AKM AK8963 datasheet, 10/2013
// 8/27/2011 by Jeff Rowberg <jeff@rowberg.net>
// Updates should (hopefully) always be available at https://github.com/jrowberg/i2cdevlib
//
// Changelog:
//     2016-01-02 - initial release based on AK8975 code
//

/* ============================================
I2Cdev device library code is placed under the MIT license
Copyright (c) 2011 Jeff Rowberg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
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

#define AK8963_MODE_POWERDOWN           0x0
#define AK8963_MODE_SINGLE              0x11
#define AK8963_MODE_CONTINUOUS_8HZ      0x12
#define AK8963_MODE_EXTERNAL            0x14
#define AK8963_MODE_CONTINUOUS_100HZ    0x6
#define AK8963_MODE_SELFTEST            0x18
#define AK8963_MODE_FUSEROM             0x1F

#define AK8963_RES_14_BIT               0
#define AK8963_RES_16_BIT               1

#define AK8963_CNTL2_SRST_BIT           0

#define AK8963_ASTC_SELF_BIT            6

#define AK8963_I2CDIS_DISABLE           0x1B

#define AK8963_WIA_DEVICE_ID			0x48

class AK8963 {
    public:
        AK8963();
        AK8963(uint8_t address);

        void initialize();
        bool testConnection();
        void reset();
		void readAdjustment();
		XYZ16_TypeDef getMagneticField();
		void calculateMag();
		void updateRawData();
		
    private:
        uint8_t devAddr;
        uint8_t buffer[6];
        uint8_t mode;
		XYZ16_TypeDef mag, magRaw, adjustment;
};

#endif /* _AK8963_H_ */
