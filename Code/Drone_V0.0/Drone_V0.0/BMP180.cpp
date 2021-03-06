/**
******************************************************************************
* File Name         : BMP180.cpp
* Description       : Initialization and communication functions for the BMP180 pressure sensor using a static class for ease
* Author			: Juan Morency Trudel
* Version           : 1.0.0
* Date				: June 2017
******************************************************************************
*/

#include "BMP180.h"

bool bmp180Initialized  = false;
bool bmp180DataReady = false;

//definition of static variables
int16_t BMP180::_oss; //oss = oversampling setting

uint8_t devAddr;
int16_t AC1, AC2, AC3, B1, B2=0, MB, MC, MD;
uint16_t AC4, AC5, AC6;
int32_t B5, UT, UP, Pressure0, PVal, AVal, TVal, PValOut;
int32_t PressureVal; //in Pascal
int32_t TemperatureVal; // in 0.1 Celsius
int32_t AltitudeVal; // in cm
AvgTypeDef BMP180_Filter[3]; // 0 is pressure, 1 is altitude and 2 is temperature
uint8_t RegBuff[3];


//constructor
BMP180::BMP180()
{
	devAddr = BMP180_ADDRESS<<1;
}

/**
 * @brief  initializes BMP180
 */
void BMP180::initialize() 
{
	for(int i = 0; i<8; i++)
	{
		BMP180_Filter[0].AvgBuffer[i] = MEAN_PRESSURE_AT_HOME;
		BMP180_Filter[1].AvgBuffer[i] = LOCAL_ADS_ALTITUDE;
		BMP180_Filter[2].AvgBuffer[i] = MEAN_TEMPERATURE_AT_HOME;
	}
	PressureVal = MEAN_PRESSURE_AT_HOME, TemperatureVal = MEAN_TEMPERATURE_AT_HOME, AltitudeVal = LOCAL_ADS_ALTITUDE, Pressure0 = MSLP;
	if(!(testConnection())){serialTransmit("BMP180 Test connection failed");}
  	SetOversample(MODE_ULTRA_LOW_POWER);
  	ReadCalibrationData();
  	PressureAtSeaLevel();	//sets pressure0  to calculate altitude from the delta pressure measured
	bmp180Initialized = true;
}

/** 
 * @brief Verifies the I2C connection.
 * Makes sure the device is connected and responds as expected.
 * @return True if connection is valid, false otherwise
 */
bool BMP180::testConnection()
{
    if (readI2C(devAddr, BMP180_RA_ID) == BMP180_ID) {return true;}
    return false;	
}

/** 
 * @brief returns the Pressure0 calculated from the PressureAtSeaLevel method
 * @return the Pressure0
 */
int32_t BMP180::getPressure0()
{
	return Pressure0;
}

/** 
 * @brief returns the PressureVal in Pascal
 * @return PressureVal
 */
int32_t BMP180::getPressure()
{
	return PressureVal;
}

/** 
 * @brief returns the temperature in 0.1 C
 * @return TemperatureVal
 */
int32_t BMP180::getTemperature()
{
	return TemperatureVal;
}

/** 
 * @brief returns the altitude in cm
 * @return AltitudeVal
 */
int32_t BMP180::getAltitude()
{
	return AltitudeVal;
}

/**
  * @brief Start temperature measurement by writing to a register on the BMP180
  */
void BMP180::StartTemperatureMeasurement()
{
	if(writeI2C(devAddr, BMP180_RA_CONTROL, READ_TEMPERATURE)) turnDebugLedOn(0);
}


/**
  * @brief Start pressure measurement by writing to a register on the BMP180
  */
void BMP180::StartPressureMeasurement()
{
	if(writeI2C(devAddr, BMP180_RA_CONTROL, READ_PRESSURE + (_oss << 6))) turnDebugLedOn(1);
}


/**
  * @brief Read uncompensated temperature, to be done after StartTemperatureMeasurement
  * has been called (and certain delay)
  */
void BMP180::ReadUncompensatedTemperature()
{
	static uint8_t RegBuff[2];
	readI2C(devAddr, BMP180_RA_CONTROL_OUTPUT, &RegBuff[0], 2);
  	UT = ((int32_t)RegBuff[0] << 8) + (int32_t)RegBuff[1];
	//sprintf(buffer, "UT: %li \n", UT);
	//serialTransmit(buffer);
}


/**
  * @brief  Read uncompensated pressure, to be done after StartTemperatureMeasurement
  * has been called (and certain delay)
  */
void BMP180::ReadUncompensatedPressure()
{
	static uint8_t RegBuff[3];
	readI2C(devAddr, BMP180_RA_CONTROL_OUTPUT, &RegBuff[0], 3);
  	UP = (((int32_t)RegBuff[0] << 16) + ((int32_t)RegBuff[1] << 8) + ((int32_t)RegBuff[2])) >> (8 -_oss); // uncompensated pressure value
	//sprintf(buffer, "UP: %li \n", UP);
	//serialTransmit(buffer);	
}


/**
  * @brief Calculate true temperature
  * @param  *pTrueTemperature: true temperature 
  */
void BMP180::CalculateTrueTemperature(int32_t *pTrueTemperature)
{
	static int32_t X1, X2;
	
  	X1 = ((UT - AC6) * AC5) >> 15;
  	X2 = ((int32_t)MC << 11) / (X1 + (int32_t)MD);
  	B5 = X1 + X2;
  	*pTrueTemperature = (B5 + 8) >> 4;
	//sprintf(buffer, "UT: %li, AC6: %u, AC5: %u, MC: %i, MD: %i, X1: %li, X2: %li, B5: %li, true temp: %li \n",UT, AC6, AC5, MC, MD, X1, X2, B5, *pTrueTemperature);
	//serialTransmit(buffer);
}


/**
  * @brief  Calculate true pressure
  * @param  *pTruePressure: true pressure
  * @retval None
  */
void BMP180::CalculateTruePressure(int32_t *pTruePressure)
{
	static int32_t X1, X2, X3, B3, B6, P, Temp;
	static uint32_t  B4, B7;
	
	B6 = B5 - 4000;             
	X1 = ((int32_t)B2* ((B6 * B6) >> 12)) >> 11;
	X2 = (int32_t)AC2 * B6 >> 11;
	X3 = X1 + X2;
	Temp = (((int32_t)AC1 << 2) + X3) << _oss;
	B3 = (Temp + 2) >> 2;
	X1 = ((int32_t)AC3 * B6) >> 13;
	X2 = (B1 * (B6 * B6 >> 12)) >> 16;
	X3 = ((X1 + X2) + 2) >> 2;
	B4 = (AC4 * (uint32_t) (X3 + 32768)) >> 15;
	B7 = ((uint32_t)UP - B3) * (50000 >> _oss);
  	if(B7 < 0x80000000)
  	{
   		P = (B7 << 1) / B4;
  	}	
  	else
  	{
  		P = (B7 / B4) << 1;
  	}

	X1 = (P >> 8) * (P >> 8);
	X1 = (X1 * 3038) >> 16;
	X2 = (-7357 * P) >> 16;
	
  	*pTruePressure = P + ((X1 + X2 + 3791) >> 4);
}

/**
  * @brief  Calculating average value of pressure for initialization purposes. 
  * Also updates the filter with these values to avoid starting at 0
  */
void BMP180::LocalpressureAvg()
{
	for(uint8_t i = 0; i < 20; i++)
	{
		//need to double the start command otherwise have some weird results...
		StartTemperatureMeasurement();
		_delay_ms(5); //4.5ms	
		StartTemperatureMeasurement();
		_delay_ms(5); //4.5ms	
		ReadUncompensatedTemperature();
		StartPressureMeasurement();
		_delay_ms(26);//25.5ms
		StartPressureMeasurement();
		_delay_ms(26);//25.5ms
		ReadUncompensatedPressure();
		
		//neglect the first couple values, they might be wrong
		if(i>3)
		{
			CalculateTrueTemperature(&TVal);
			updateAvg8Filter(&BMP180_Filter[2].Index, BMP180_Filter[2].AvgBuffer, TVal, &TemperatureVal);
			
			CalculateTruePressure(&PVal);
			updateAvg8Filter(&BMP180_Filter[0].Index, BMP180_Filter[0].AvgBuffer, PVal, &PressureVal);
			
		}
		
		//sprintf(buffer, "TemperatureVal:%li, PValOut:%li, Altitude:%li \n", TemperatureVal, PressureVal, AltitudeVal);
		//serialTransmit(buffer);
	}
}


/** 
  * @brief  Calculate pressure at sea level using mean pressure measured and altitude
  */
void BMP180::PressureAtSeaLevel(void)
{  
	float Temp = 0.0f;
	LocalpressureAvg();
	Temp = (float)LOCAL_ADS_ALTITUDE / 4433000;
	Temp = (float)pow((1 - Temp), 5.255f);
	Pressure0 = (PressureVal) / Temp;
	//sprintf(buffer, "pres0:%li, pres:%li \n",Pressure0, PressureVal);
	//serialTransmit(buffer);
}


/** 
  * @brief  Calculating absolute altitude
  * @param  *pAltitude: absolute altitude
  */
void BMP180::CalculateAbsoluteAltitude(int32_t *pAltitude)
{
	*pAltitude = (int32_t)(4433000 * (1 - pow((PressureVal / (float)Pressure0), 0.1903)));
	//sprintf(buffer, "pressure %li, pressure0: %li, alt:%li \n",PressureVal, Pressure0, *pAltitude);
	//serialTransmit(buffer);
}

/**
  * @brief  Read calibration data from the EEPROM of the BMP180
  */
void BMP180::ReadCalibrationData() 
{
	readI2C(devAddr, BMP180_RA_CAL_AC1, RegBuff, 2);
  	AC1 = ((int16_t)RegBuff[0] <<8 | ((int16_t)RegBuff[1]));
	readI2C(devAddr, BMP180_RA_CAL_AC2, RegBuff, 2);
  	AC2 = ((int16_t)RegBuff[0] <<8 | ((int16_t)RegBuff[1]));
	readI2C(devAddr, BMP180_RA_CAL_AC3, RegBuff, 2);
  	AC3 = ((int16_t)RegBuff[0] <<8 | ((int16_t)RegBuff[1]));
	readI2C(devAddr, BMP180_RA_CAL_AC4, RegBuff, 2);
  	AC4 = ((uint16_t)RegBuff[0] <<8 | ((uint16_t)RegBuff[1]));
	readI2C(devAddr, BMP180_RA_CAL_AC5, RegBuff, 2);
  	AC5 = ((uint16_t)RegBuff[0] <<8 | ((uint16_t)RegBuff[1]));
	readI2C(devAddr, BMP180_RA_CAL_AC6, RegBuff, 2);
  	AC6 = ((uint16_t)RegBuff[0] <<8 | ((uint16_t)RegBuff[1])); 
	readI2C(devAddr, BMP180_RA_CAL_B1, RegBuff, 2);
  	B1 = ((int16_t)RegBuff[0] <<8 | ((int16_t)RegBuff[1])); 
	readI2C(devAddr, BMP180_RA_CAL_B2, RegBuff, 2);
  	B2 = ((int16_t)RegBuff[0] <<8 | ((int16_t)RegBuff[1])); 
	readI2C(devAddr, BMP180_RA_CAL_MB, RegBuff, 2);
  	MB = ((int16_t)RegBuff[0] <<8 | ((int16_t)RegBuff[1]));
	readI2C(devAddr, BMP180_RA_CAL_MC, RegBuff, 2);
  	MC = ((int16_t)RegBuff[0] <<8 | ((int16_t)RegBuff[1]));
	readI2C(devAddr, BMP180_RA_CAL_MD, RegBuff, 2);
  	MD = ((int16_t)RegBuff[0] <<8 | ((int16_t)RegBuff[1]));
	//sprintf(buffer, "calibration: AC1: %i, AC2: %i, AC3: %i, AC4: %u, AC5: %u, AC6: %u, B1: %i, B2: %i, MB: %i, MC: %i, MD: %i \n",AC1, AC2, AC3, AC4, AC5, AC6,B1, B2, MB, MC, MD);
	//serialTransmit(buffer);
}


/**
  * @brief  Configures hardware pressure sampling accuracy modes
  */
void BMP180::SetOversample(uint8_t mode)
{
	_oss = mode;
}


/**
 * @brief Read raw values and calculation of pressure, temperature and altitude for BMP180 with FSM. 
 * This function should not be called at more than 100 Hz due to the assumed delay between state. The actual update rate 
 * of the data will be the rate this function is called divided by 5 (since the FSM currently has 5 states).
  For some weird reason, the start temperature and pressure function must be called twice... maybe try to optimize this
  */
void BMP180::CalculateTemperaturePressureAndAltitude()
{
	static uint8_t State = START_TEMPERATURE_MEASUREMENT;
	static uint8_t counter = 0;

	switch(State)
	{
		case START_TEMPERATURE_MEASUREMENT:
			StartTemperatureMeasurement();
			//_delay_ms(5); //4.5ms
			
			//to double this state before passing to the next one
			if(counter > 0)
			{
				counter = 0;
				State = READ_UT_AND_START_PRESSURE_MEASUREMENT;
			}
			else
			{
				counter++;
			}
			break;
			
		case READ_UT_AND_START_PRESSURE_MEASUREMENT:
			if(counter == 0)
			{
				ReadUncompensatedTemperature();
			}
			StartPressureMeasurement();
			//_delay_ms(10);//7.5ms
			
			//to double this state before passing to the next one
			if(counter > 0)
			{
				counter = 0;
				State = READ_UP_AND_CAL_TRUE_PRESSURE_AND_TEMPERATURE;
			}
			else
			{
				counter++;
			}
			
			break;
			
		case READ_UP_AND_CAL_TRUE_PRESSURE_AND_TEMPERATURE:
			ReadUncompensatedPressure();
			
			CalculateTrueTemperature(&TVal);
			updateAvg8Filter(&BMP180_Filter[2].Index, BMP180_Filter[2].AvgBuffer, TVal, &TemperatureVal);
						
			CalculateTruePressure(&PVal);
			updateAvg8Filter(&BMP180_Filter[0].Index, BMP180_Filter[0].AvgBuffer, PVal, &PressureVal);
			PressureVal = PVal;

			CalculateAbsoluteAltitude(&AVal);
			updateAvg8Filter(&BMP180_Filter[1].Index, BMP180_Filter[1].AvgBuffer, AVal, &AltitudeVal);

			State = START_TEMPERATURE_MEASUREMENT; 
			bmp180DataReady = true;
			break;

		default:
			State = START_TEMPERATURE_MEASUREMENT; 
			break;
	}	
}

/**
  * @brief  Calculates true values of the temperature, pressure and altitude
  */
void BMP180::calculateTrueValues()
{
	CalculateTrueTemperature(&TVal);
	updateAvg8Filter(&BMP180_Filter[2].Index, BMP180_Filter[2].AvgBuffer, TVal, &TemperatureVal);
	
	CalculateTruePressure(&PVal);
	updateAvg8Filter(&BMP180_Filter[0].Index, BMP180_Filter[0].AvgBuffer, PVal, &PressureVal);
	PressureVal = PVal;

	CalculateAbsoluteAltitude(&AVal);
	updateAvg8Filter(&BMP180_Filter[1].Index, BMP180_Filter[1].AvgBuffer, AVal, &AltitudeVal);
}


/**
  * @brief  Sets the temperature of the object
  * @param int32_t of the input temperature
  */
void BMP180::setRawTemperature(int32_t inputTemp)
{
	UT = inputTemp;
}

/**
  * @brief  Sets the pressure of the object
  * @param int32_t of the input pressure
  */
void BMP180::setRawPressure(int32_t inputPress)
{
	UP = inputPress;
}