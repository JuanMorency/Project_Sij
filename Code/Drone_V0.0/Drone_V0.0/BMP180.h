/**
******************************************************************************
* File Name         : BMP180.h
* Description       : header file for BMP180.cpp
* Author			: Juan Morency Trudel
* Version           : 1.0.0
* Date				: June 2017
******************************************************************************
*/

#ifndef __BMP180_H
#define __BMP180_H

#ifndef F_CPU
#define F_CPU 16000000
#endif

#include "I2C.h"
#include "lcd.h"
#include "debugLED.h"
#include "serial.h"
#include "typeDef.h"
#include "filter.h"

#define BMP180_ADDRESS				0xEE     // default I2C address  


/* ---- Registers ---- */
#define BMP180_RA_CAL_AC1           0xAA  // R   Calibration data (16 bits)
#define BMP180_RA_CAL_AC2           0xAC  // R   Calibration data (16 bits)
#define BMP180_RA_CAL_AC3           0xAE  // R   Calibration data (16 bits)    
#define BMP180_RA_CAL_AC4           0xB0  // R   Calibration data (16 bits)
#define BMP180_RA_CAL_AC5           0xB2  // R   Calibration data (16 bits)
#define BMP180_RA_CAL_AC6           0xB4  // R   Calibration data (16 bits)
#define BMP180_RA_CAL_B1            0xB6  // R   Calibration data (16 bits)
#define BMP180_RA_CAL_B2            0xB8  // R   Calibration data (16 bits)
#define BMP180_RA_CAL_MB            0xBA  // R   Calibration data (16 bits)
#define BMP180_RA_CAL_MC            0xBC  // R   Calibration data (16 bits)
#define BMP180_RA_CAL_MD            0xBE  // R   Calibration data (16 bits)
#define BMP180_RA_CONTROL           0xF4  // W   Control register 
#define BMP180_RA_CONTROL_OUTPUT    0xF6  // R   Output registers 0xF6=MSB, 0xF7=LSB, 0xF8=XLSB
#define BMP180_RA_SOFT_RESET		0xE0  // W	 reset
#define BMP180_RA_ID				0xD0  // R	 Read to check if I2C is working, see BMP180_ID

// BMP180 Modes
#define MODE_ULTRA_LOW_POWER		0 //oversampling=0, internalsamples=1, maxconvtimepressure=4.5ms, avgcurrent=3uA, RMSnoise_hPA=0.06, RMSnoise_m=0.5
#define MODE_STANDARD				1 //oversampling=1, internalsamples=2, maxconvtimepressure=7.5ms, avgcurrent=5uA, RMSnoise_hPA=0.05, RMSnoise_m=0.4
#define MODE_HIGHRES				2 //oversampling=2, internalsamples=4, maxconvtimepressure=13.5ms, avgcurrent=7uA, RMSnoise_hPA=0.04, RMSnoise_m=0.3
#define MODE_ULTRA_HIGHRES			3 //oversampling=3, internalsamples=8, maxconvtimepressure=25.5ms, avgcurrent=12uA, RMSnoise_hPA=0.03, RMSnoise_m=0.25

// Control register
#define READ_TEMPERATURE			0x2E 
#define READ_PRESSURE				0x34 

//Reset register 
#define BMP180_RESET				0xB6	

//Chip ID
#define BMP180_ID					0x55	

//Other
#define MSLP						101325          // Mean Sea Level Pressure = 1013.25 hPA (1hPa = 100Pa = 1mbar)
#define MEAN_PRESSURE_AT_LINTON		100200
#define MEAN_TEMPERATURE_AT_LINTON	240
#define LOCAL_ADS_ALTITUDE			7357			//cm     altitude of 4611 avenue Linton

enum
{
	START_TEMPERATURE_MEASUREMENT = 0,
	READ_UT_AND_START_PRESSURE_MEASUREMENT,
	READ_UP_AND_CAL_TRUE_PRESSURE_AND_TEMPERATURE,
};

class BMP180 {
	public:
	BMP180();

	void initialize();
	bool testConnection();
	void reset();
	
	int32_t getPressure0();
	int32_t getPressure();
	int32_t getTemperature();
	int32_t getAltitude();

	void CalculateTemperaturePressureAndAltitude();


	
	private:
	void updateFilter(uint8_t *pIndex, int32_t *pAvgBuffer, int32_t InVal, int32_t *pOutVal);
	void SetOversample(uint8_t mode);
	void ReadCalibrationData();
	void StartPressureMeasurement();
	void StartTemperatureMeasurement();
	void ReadUncompensatedTemperature();
	void ReadUncompensatedPressure();
	void CalculateTrueTemperature(int32_t *pTrueTemperature);
	void CalculateTruePressure(int32_t *pTruePressure);
	void CalculateAbsoluteAltitude(int32_t *pAltitude);
	void LocalpressureAvg();
	void PressureAtSeaLevel(void);
	uint8_t devAddr;
	
	int16_t AC1, AC2, AC3, B1, B2, MB, MC, MD, _oss;  //oss = oversampling setting
	uint16_t AC4, AC5, AC6;
	int32_t B5, UT, UP, Pressure0, PVal, AVal, TVal, PValOut;
	int32_t PressureVal; //in Pascal
	int32_t TemperatureVal; // in 0.1 Celsius
	int32_t AltitudeVal; // in cm
	AvgTypeDef BMP180_Filter[3]; // 0 is pressure, 1 is altitude and 2 is temperature
	uint8_t RegBuff[3];
};


#endif

