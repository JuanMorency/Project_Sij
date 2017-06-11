#include "BMP180.h"

/** Default constructor, uses default I2C address.
 * @see BMP180_ADDRESS
 */
BMP180::BMP180() {
    devAddr = BMP180_ADDRESS;
}


/**
  * @brief  initializes BMP180
  * @param  None
  * @retval None
  */
void BMP180::initialize() 
{
	for(int i = 0; i<8; i++)
	{
		BMP180_Filter[0].AvgBuffer[i] = MEAN_PRESSURE_AT_LINTON;
		BMP180_Filter[1].AvgBuffer[i] = LOCAL_ADS_ALTITUDE;
		BMP180_Filter[2].AvgBuffer[i] = MEAN_TEMPERATURE_AT_LINTON;
		
	}
	
	
	PressureVal = MEAN_PRESSURE_AT_LINTON, TemperatureVal = MEAN_TEMPERATURE_AT_LINTON, AltitudeVal = LOCAL_ADS_ALTITUDE, Pressure0 = MSLP;
	if(!(this->testConnection())){turnDebugLedOn(2);}
  	SetOversample(MODE_ULTRA_LOW_POWER);
  	ReadCalibrationData();
  	PressureAtSeaLevel();	//sets pressure0  to calculate altitude from the delta pressure measured
	
}

/** Verify the I2C connection.
 * Make sure the device is connected and responds as expected.
 * @return True if connection is valid, false otherwise
 */
bool BMP180::testConnection()
{
    if (readI2C(devAddr, BMP180_RA_ID) == BMP180_ID) {return true;}
    return false;	
}

void BMP180::reset()
{
		
}


int32_t BMP180::getPressure0()
{
	return Pressure0;
}

int32_t BMP180::getPressure()
{
	return PressureVal;
}

int32_t BMP180::getTemperature()
{
	return TemperatureVal;
}

int32_t BMP180::getAltitude()
{
	return AltitudeVal;
}


///**
  //* @brief  Digital filter
  //* @param *pIndex:
  //* @param *pAvgBuffer:
  //* @param InVal:
  //* @param pOutVal:
  //*
  //* @retval None
  //*               
  //*/
//void BMP180::updateFilter(uint8_t *pIndex, int32_t *pAvgBuffer, int32_t InVal, int32_t *OutVal)
//{	
	//uint8_t i;
	//
  	//*(pAvgBuffer + ((*pIndex) ++)) = InVal; // increase index by one and insert value at that position
  	//*pIndex &= 0x07;	// if index is higher than 7, return index to 0
  	//
	  //
	////calculate the mean of all the values in the buffer
  	//*OutVal = 0;
	//for(i = 0; i < 8; i ++) 
  	//{
    	//*OutVal += *(pAvgBuffer + i);
  	//}
  	//*OutVal >>= 3;
//}

/**
  * @brief  Start temperature measurement
  * @param  None
  * @retval None
  */
void BMP180::StartTemperatureMeasurement()
{
	if(writeI2C(devAddr, BMP180_RA_CONTROL, READ_TEMPERATURE)) turnDebugLedOn(0);
}


/**
  * @brief  Start pressure measurement
  * @param  None
  * @retval None
  */
void BMP180::StartPressureMeasurement()
{
	if(writeI2C(devAddr, BMP180_RA_CONTROL, READ_PRESSURE + (_oss << 6))) turnDebugLedOn(1);
}


/**
  * @brief  Read uncompensated temperature
  * @param  None
  * @retval None
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
  * @brief  Read uncompensated pressure
  * @param  None
  * @retval None
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
  * @brief  Calculate true temperature
  * @param  *pTrueTemperature: true temperature 
  * @retval None
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
  * @brief  Calculating average value of pressure
  * @param  *pVal: the average value of pressure
  * @retval None
  */
void BMP180::LocalpressureAvg()
{
	for(uint8_t i = 0; i < 10; i++)
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
  * @brief  Calculating pressure at sea level
  * @param  None
  * @retval None
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
  * @param  PressureVal: the pressure at the absolute altitude
  * @retval None
  */
void BMP180::CalculateAbsoluteAltitude(int32_t *pAltitude)
{
	*pAltitude = (int32_t)(4433000 * (1 - pow((PressureVal / (float)Pressure0), 0.1903)));
	//sprintf(buffer, "pressure %li, pressure0: %li, alt:%li \n",PressureVal, Pressure0, *pAltitude);
	//serialTransmit(buffer);
}

/**
  * @brief  Read calibration data from the EEPROM of the BMP180
  * @param  None
  * @retval None
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
  * @param  None
  * @retval None
  */
void BMP180::SetOversample(uint8_t mode)
{
	_oss = mode;
}


/**
  * @brief  Calculation of pressure and temperature and altitude state machine for BMP180 with FSM. 
  This function should not be called at more than 100 Hz due to the assumed delay between state.
  For some weird reason, the start temperature and pressure function must be called twice... maybe try to optimize this
  * @param  None
  * @retval None
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
			//make sure you have the required delay here if IMU sampling is increased
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
			//make sure you have the required delay here if IMU sampling is increased
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
			break;

		default:
			State = START_TEMPERATURE_MEASUREMENT; 
			break;
	}	
}
