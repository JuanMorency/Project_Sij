#ifndef _MPU9255_H_
#define _MPU9255_H_

#include "I2C.h"
#include "debugLED.h"
#include "typeDef.h"

#define MPU9255_ADDRESS_AD0_LOW     0x68 // address pin low (GND), default for InvenSense evaluation board
#define MPU9255_ADDRESS_AD0_HIGH    0x69 // address pin high (VCC)
#define MPU9255_ADDRESS				MPU9255_ADDRESS_AD0_LOW


#define MPU9255_RA_SELF_TEST_X_GYRO			0x00
#define MPU9255_RA_SELF_TEST_Y_GYRO			0x01 
#define MPU9255_RA_SELF_TEST_Z_GYRO			0x02 
#define MPU9255_RA_SELF_TEST_X_ACCEL		0x0D 
#define MPU9255_RA_SELF_TEST_Y_ACCEL		0x0E 
#define MPU9255_RA_SELF_TEST_Z_ACCEL		0x0F 

#define MPU9255_RA_XG_OFFS_H				0x13 
#define MPU9255_RA_XG_OFFS_L				0x14
#define MPU9255_RA_YG_OFFS_H				0x15
#define MPU9255_RA_YG_OFFS_L				0x16
#define MPU9255_RA_ZG_OFFS_H				0x17
#define MPU9255_RA_ZG_OFFS_L				0x18

#define MPU9255_RA_SMPLRT_DIV				0x19
#define MPU9255_RA_CONFIG					0x1A
#define MPU9255_RA_GYRO_CONFIG				0x1B
#define MPU9255_RA_ACCEL_CONFIG				0x1C
#define MPU9255_RA_ACCEL_CONFIG_2			0x1D
#define MPU9255_RA_LP_ACCEL_ODR				0x1E
#define MPU9255_RA_WOM_THR					0x1F

#define MPU9255_RA_FIFO_EN					0x23
#define MPU9255_RA_I2C_MST_CTRL				0x24
#define MPU9255_RA_I2C_SLV0_ADDR			0x25
#define MPU9255_RA_I2C_SLV0_REG				0x26
#define MPU9255_RA_I2C_SLV0_CTRL			0x27
#define MPU9255_RA_I2C_SLV1_ADDR			0x28
#define MPU9255_RA_I2C_SLV1_REG				0x29
#define MPU9255_RA_I2C_SLV1_CTRL			0x2A
#define MPU9255_RA_I2C_SLV2_ADDR			0x2B
#define MPU9255_RA_I2C_SLV2_REG				0x2C
#define MPU9255_RA_I2C_SLV2_CTRL			0x2D
#define MPU9255_RA_I2C_SLV3_ADDR			0x2E
#define MPU9255_RA_I2C_SLV3_REG				0x2F
#define MPU9255_RA_I2C_SLV3_CTRL			0x30
#define MPU9255_RA_I2C_SLV4_ADDR			0x31
#define MPU9255_RA_I2C_SLV4_REG				0x32
#define MPU9255_RA_I2C_SLV4_DO				0x33
#define MPU9255_RA_I2C_SLV4_CTRL			0x34
#define MPU9255_RA_I2C_SLV4_DI				0x35

#define MPU9255_RA_I2C_MST_STATUS			0x36
#define MPU9255_RA_INT_PIN_CFG				0x37
#define MPU9255_RA_INT_ENABLE				0x38
#define MPU9255_RA_DMP_INT_STATUS			0x39
#define MPU9255_RA_INT_STATUS				0x3A

#define MPU9255_RA_ACCEL_XOUT_H				0x3B
#define MPU9255_RA_ACCEL_XOUT_L				0x3C
#define MPU9255_RA_ACCEL_YOUT_H				0x3D
#define MPU9255_RA_ACCEL_YOUT_L				0x3E
#define MPU9255_RA_ACCEL_ZOUT_H				0x3F
#define MPU9255_RA_ACCEL_ZOUT_L				0x40
#define MPU9255_RA_TEMP_OUT_H				0x41
#define MPU9255_RA_TEMP_OUT_L				0x42
#define MPU9255_RA_GYRO_XOUT_H				0x43
#define MPU9255_RA_GYRO_XOUT_L				0x44
#define MPU9255_RA_GYRO_YOUT_H				0x45
#define MPU9255_RA_GYRO_YOUT_L				0x46
#define MPU9255_RA_GYRO_ZOUT_H				0x47
#define MPU9255_RA_GYRO_ZOUT_L				0x48
#define MPU9255_RA_EXT_SENS_DATA_00			0x49
#define MPU9255_RA_EXT_SENS_DATA_01			0x4A
#define MPU9255_RA_EXT_SENS_DATA_02			0x4B
#define MPU9255_RA_EXT_SENS_DATA_03			0x4C
#define MPU9255_RA_EXT_SENS_DATA_04			0x4D
#define MPU9255_RA_EXT_SENS_DATA_05			0x4E
#define MPU9255_RA_EXT_SENS_DATA_06			0x4F
#define MPU9255_RA_EXT_SENS_DATA_07			0x50
#define MPU9255_RA_EXT_SENS_DATA_08			0x51
#define MPU9255_RA_EXT_SENS_DATA_09			0x52
#define MPU9255_RA_EXT_SENS_DATA_10			0x53
#define MPU9255_RA_EXT_SENS_DATA_11			0x54
#define MPU9255_RA_EXT_SENS_DATA_12			0x55
#define MPU9255_RA_EXT_SENS_DATA_13			0x56
#define MPU9255_RA_EXT_SENS_DATA_14			0x57
#define MPU9255_RA_EXT_SENS_DATA_15			0x58
#define MPU9255_RA_EXT_SENS_DATA_16			0x59
#define MPU9255_RA_EXT_SENS_DATA_17			0x5A
#define MPU9255_RA_EXT_SENS_DATA_18			0x5B
#define MPU9255_RA_EXT_SENS_DATA_19			0x5C
#define MPU9255_RA_EXT_SENS_DATA_20			0x5D
#define MPU9255_RA_EXT_SENS_DATA_21			0x5E
#define MPU9255_RA_EXT_SENS_DATA_22			0x5F
#define MPU9255_RA_EXT_SENS_DATA_23			0x60

#define MPU9255_RA_I2C_SLV0_DO				0x63
#define MPU9255_RA_I2C_SLV1_DO				0x64
#define MPU9255_RA_I2C_SLV2_DO				0x65
#define MPU9255_RA_I2C_SLV3_DO				0x66
#define MPU9255_RA_I2C_MST_DELAY_CTRL		0x67
#define MPU9255_RA_SIGNAL_PATH_RESET		0x68
#define MPU9255_RA_MOT_DETECT_CTRL			0x69
#define MPU9255_RA_USER_CTRL				0x6A
#define MPU9255_RA_PWR_MGMT_1				0x6B
#define MPU9255_RA_PWR_MGMT_2				0x6C

#define MPU9255_RA_FIFO_COUNTH				0x72
#define MPU9255_RA_FIFO_COUNTL				0x73
#define MPU9255_RA_FIFO_R_W					0x74
#define MPU9255_RA_WHO_AM_I					0x75

#define MPU9255_RA_XA_OFFS_H				0x77 //[14:7] XA_OFFS
#define MPU9255_RA_XA_OFFS_L				0x78 //[6:0]
#define MPU9255_RA_YA_OFFS_H				0x7A //[14:7] YA_OFFS
#define MPU9255_RA_YA_OFFS_L				0x7B //[6:0]
#define MPU9255_RA_ZA_OFFS_H				0x7D //[14:7] ZA_OFFS
#define MPU9255_RA_ZA_OFFS_L				0x7E //[6:0]

#define MPU9255_DLPF_BW_256					0x00
#define MPU9255_DLPF_BW_188					0x01
#define MPU9255_DLPF_BW_98					0x02
#define MPU9255_DLPF_BW_42					0x03
#define MPU9255_DLPF_BW_20					0x04
#define MPU9255_DLPF_BW_10					0x05
#define MPU9255_DLPF_BW_5					0x06

#define MPU9255_GYRO_FS_250					0x00
#define MPU9255_GYRO_FS_500					0x01
#define MPU9255_GYRO_FS_1000				0x02	
#define MPU9255_GYRO_FS_2000				0x03

#define MPU9255_ACCEL_FS_2					0x00
#define MPU9255_ACCEL_FS_4					0x01
#define MPU9255_ACCEL_FS_8					0x02
#define MPU9255_ACCEL_FS_16					0x03

#define MPU9255_INTCFG_INT_LEVEL_BIT        7
#define MPU9255_INTCFG_INT_OPEN_BIT         6
#define MPU9255_INTCFG_LATCH_INT_EN_BIT     5
#define MPU9255_INTCFG_INT_RD_CLEAR_BIT     4
#define MPU9255_INTCFG_FSYNC_INT_LEVEL_BIT  3
#define MPU9255_INTCFG_FSYNC_INT_EN_BIT     2
#define MPU9255_INTCFG_I2C_BYPASS_EN_BIT    1
#define MPU9255_INTCFG_CLKOUT_EN_BIT        0


#define GYR_OFFSET_MAX_CALIBRATION			100

#define ACC_X_OFFSET						-200
#define ACC_Y_OFFSET						-125
#define ACC_Z_OFFSET						-420

//min x:-9800	max x:10200   
//min y:-9950	max y: 10200
//min z:-9850	max z:10690


extern bool mpu9255DataReady;

class MPU9255 {
    public:
        MPU9255();
        MPU9255(uint8_t address);

        void initialize();
        bool testConnection();
		void initGyrOffset();
		void updateRawData();
		void updateRawDataInterrupt();
		void calculateAccRotTemp();
		
        int16_t getTemperature();
        XYZ16_TypeDef getRotation();
		int16_t getRotationX();
		int16_t getRotationY();
		int16_t getRotationZ();
        XYZ16_TypeDef getAcceleration();
		int16_t getAccelerationX();
		int16_t getAccelerationY();
		int16_t getAccelerationZ();

		void setRawAcceleration(XYZ16_TypeDef inputAcc);
		void setRawRotation(XYZ16_TypeDef inputGyr);
		void setRawTemperature(uint16_t inputTemp);
		
		
    private:
        uint8_t devAddr;
        uint8_t bufferI2C[14];
		XYZ16_TypeDef gyrOffset, accOffset;
		XYZ16_TypeDef accRaw, gyrRaw;
		XYZ16_TypeDef acc; //in G x10^(-4)
		XYZ16_TypeDef gyr; //in 0.1 degrees per second
		XYZ16_TypeDef tempData; //in 0.1 degrees per second
		uint16_t tempRaw, temp;
};

#endif /* _MPU9255_H_ */
