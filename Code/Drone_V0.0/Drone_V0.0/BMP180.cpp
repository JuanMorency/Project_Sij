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
	PressureVal = 101325, TemperatureVal = 27, AltitudeVal = 0, Pressure0 = MSLP;
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


/**
  * @brief  Digital filter
  * @param *pIndex:
  * @param *pAvgBuffer:
  * @param InVal:
  * @param pOutVal:
  *
  * @retval None
  *               
  */
void BMP180::CalAvgValue(uint8_t *pIndex, int32_t *pAvgBuffer, int32_t InVal, int32_t *pOutVal)
{	
	uint8_t i;
	
  	*(pAvgBuffer + ((*pIndex) ++)) = InVal;
  	*pIndex &= 0x07;
  	
  	*pOutVal = 0;
	for(i = 0; i < 8; i ++) 
  	{
    	*pOutVal += *(pAvgBuffer + i);
  	}
  	*pOutVal >>= 3;
}

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
	uint8_t RegBuff[2];
	readI2C(devAddr, BMP180_RA_CONTROL_OUTPUT, &RegBuff[0], 2);
  	UT = ((int32_t)RegBuff[0] << 8) + (int32_t)RegBuff[1];
}


/**
  * @brief  Read uncompensated pressure
  * @param  None
  * @retval None
  */
void BMP180::ReadUncompensatedPressure()
{
	uint8_t RegBuff[3];
	readI2C(devAddr, BMP180_RA_CONTROL_OUTPUT, &RegBuff[0], 3);
  	UP = (((int32_t)RegBuff[0] << 16) + ((int32_t)RegBuff[1] << 8) + ((int32_t)RegBuff[2])) >> (8 -_oss); // uncompensated pressure value
}


/**
  * @brief  Calculate true temperature
  * @param  *pTrueTemperature: true temperature 
  * @retval None
  */
void BMP180::CalculateTrueTemperature(int32_t *pTrueTemperature)
{
	int32_t X1, X2;
	
  	X1 = ((UT - AC6) * AC5) >> 15;
  	X2 = ((int32_t)MC << 11) / (X1 + (int32_t)MD);
  	B5 = X1 + X2;
  	*pTrueTemperature = (B5 + 8) >> 4;
}


/**
  * @brief  Calculate true pressure
  * @param  *pTruePressure: true pressure
  * @retval None
  */
void BMP180::CalculateTruePressure(int32_t *pTruePressure)
{
	int32_t X1, X2, X3, B3, B6, P, Temp;
	uint32_t  B4, B7;
	
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
void BMP180::LocalpressureAvg(int32_t *pVal)
{
	uint8_t i;
	int32_t Sum = 0;
	int32_t PVal, TVal;
	_delay_ms(500);
	for(i = 0; i < 10; i ++)
	{
		StartTemperatureMeasurement();
		_delay_ms(20); //4.5ms	
		ReadUncompensatedTemperature();
		StartPressureMeasurement();
		_delay_ms(50);//7.5ms
		ReadUncompensatedPressure();
		
		CalculateTruePressure(&PVal);
		CalAvgValue(&BMP180_Filter[0].Index, BMP180_Filter[0].AvgBuffer, PVal - PRESSURE_OFFSET, &PressureVal);

		//CalculateTrueTemperature(&TVal);
		//CalAvgValue(&BMP180_Filter[2].Index, BMP180_Filter[2].AvgBuffer, TVal, &TemperatureVal);
		_delay_ms(25);
	
		//
		//StartTemperatureMeasurement();
		//_delay_ms(5); //4.5ms   324  // check to change to smaller
		//ReadUncompensatedTemperature();
		//StartPressureMeasurement();
		//_delay_ms(26);//7.5ms    540   // check to change to smaller
		//ReadUncompensatedPressure();
		//CalculateTruePressure(&PressureVal);
		//CalculateTrueTemperature(&TemperatureVal);
		//
		////sum the 4 last values
		//if(i >= 16)
		//{
			//Sum += PressureVal;
		//}
		char data[20];
		char data2[20];
		sprintf(data, "t:%li", UT);
		sprintf(data2, "p:%li", UP);
		changeLCDText(data, data2);
	}
	
	
	//char data[20];
	//char data2[20];
	//sprintf(data, "t:%li", PressureVal);
	//sprintf(data2, "p:%li", PressureVal);
	//changeLCDText(data, data2);
	
	//export the mean of last 4 values
	//*pVal = Sum >>3;
	*pVal = PressureVal;
}


/** 
  * @brief  Calculating pressure at sea level
  * @param  None
  * @retval None
  */
void BMP180::PressureAtSeaLevel(void)
{  
	float Temp = 0.0f;
	LocalpressureAvg(&PressureVal);
	//Temp = (float)LOCAL_ADS_ALTITUDE / 4433000;
	//Temp = (float)pow((1 - Temp), 5.255f);
	//Pressure0 = (PressureVal - PRESSURE_OFFSET) / Temp;
	//Pressure0 = 101325;
}


/** 
  * @brief  Calculating absolute altitude
  * @param  *pAltitude: absolute altitude
  * @param  PressureVal: the pressure at the absolute altitude
  * @retval None
  */
void BMP180::CalculateAbsoluteAltitude(int32_t *pAltitude, int32_t PressureVal)
{
	*pAltitude = 4433000 * (1 - pow((PressureVal / (float)Pressure0), 0.1903)); 
}

/**
  * @brief  Read calibration data from the EEPROM of the BMP180
  * @param  None
  * @retval None
  */
void BMP180::ReadCalibrationData() 
{
	uint8_t RegBuff[2];
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
  * @brief  Calculation of pressure and temperature and altitude state machine for BMP180
  * @param  None
  * @retval None
  */
void BMP180::CalculateTemperaturePressureAndAltitude()
{
	static uint8_t State = START_TEMPERATURE_MEASUREMENT;
	int32_t PVal, AVal, TVal;

	switch(State)
	{
		case START_TEMPERATURE_MEASUREMENT:
			StartTemperatureMeasurement();
			//_delay_ms(5); //4.5ms
			//make sure you have the required delay here if IMU sampling is increased
			State = READ_UT_AND_START_PRESSURE_MEASUREMENT;
			break;
			
		case READ_UT_AND_START_PRESSURE_MEASUREMENT:
			ReadUncompensatedTemperature();
			StartPressureMeasurement();
			//_delay_ms(10);//7.5ms
			//make sure you have the required delay here if IMU sampling is increased
			State = READ_UP_CAL_TRUE_PRESSURE_TEMPERATURE;
			break;
			
		case READ_UP_CAL_TRUE_PRESSURE_TEMPERATURE:
			ReadUncompensatedPressure();
			
			CalculateTrueTemperature(&TVal);
			CalAvgValue(&BMP180_Filter[2].Index, BMP180_Filter[2].AvgBuffer, TVal, &TemperatureVal);
			
			CalculateTruePressure(&PVal);
			CalAvgValue(&BMP180_Filter[0].Index, BMP180_Filter[0].AvgBuffer, PVal - PRESSURE_OFFSET, &PressureVal);

			CalculateAbsoluteAltitude(&AVal, PVal - PRESSURE_OFFSET);
			CalAvgValue(&BMP180_Filter[1].Index, BMP180_Filter[1].AvgBuffer, AVal, &AltitudeVal);

			State = START_TEMPERATURE_MEASUREMENT; 
			break;

		default:
			break;
	}	
}
