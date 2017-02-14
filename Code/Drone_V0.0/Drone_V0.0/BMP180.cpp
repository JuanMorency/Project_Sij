/**
  ******************************************************************************
  * @file    BMP180.c
  * @author  Waveshare Team
  * @version V1.0
  * @date    29-August-2014
  * @brief   This file includes the BMP180 driver functions
  
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, WAVESHARE SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  ******************************************************************************
  */



#include "BMP180.h"

int16_t AC1, AC2, AC3, B1, B2, MB, MC, MD, _oss;  //oss = oversampling setting
uint16_t AC4, AC5, AC6;
int32_t B5, UT, UP, Pressure0 = MSLP;
int32_t PressureVal = 0, TemperatureVal = 0, AltitudeVal = 0; 
//Pressure in Pascal, Temperature in 0.1 C and Altitude in Meter


int32_t getPressure0()
{
	return Pressure0;
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
void BMP180_CalAvgValue(uint8_t *pIndex, int32_t *pAvgBuffer, int32_t InVal, int32_t *pOutVal)
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
void BMP180_StartTemperatureMeasurement()
{
	writeI2C(BMP180_ADDRESS, CONTROL, READ_TEMPERATURE);
}


/**
  * @brief  Start pressure measurement
  * @param  None
  * @retval None
  */
void BMP180_StartPressureMeasurement()
{
	writeI2C(BMP180_ADDRESS, CONTROL, READ_PRESSURE + (_oss << 6));
}


/**
  * @brief  Read uncompensated temperature
  * @param  None
  * @retval None
  */
void BMP180_ReadUncompensatedTemperature()
{
	uint8_t RegBuff[2];
	readI2C(BMP180_ADDRESS, CONTROL_OUTPUT, &RegBuff[0], 2);
  	UT = ((int32_t)RegBuff[0] << 8) + (int32_t)RegBuff[1]; 
}


/**
  * @brief  Read uncompensated pressure
  * @param  None
  * @retval None
  */
void BMP180_ReadUncompensatedPressure()
{
	uint8_t RegBuff[3];
	readI2C(BMP180_ADDRESS, CONTROL_OUTPUT, &RegBuff[0], 3);
  	UP = (((int32_t)RegBuff[0] << 16) + ((int32_t)RegBuff[1] << 8) + ((int32_t)RegBuff[2])) >> (8 -_oss); // uncompensated pressure value
}


/**
  * @brief  Calculate true temperature
  * @param  *pTrueTemperature: true temperature 
  * @retval None
  */
void BMP180_CalculateTrueTemperature(int32_t *pTrueTemperature)
{
	int32_t X1, X2;
	
  	X1 = ((UT - AC6) * AC5) >> 15;
  	X2 = (MC << 11) / (X1 + MD);
  	B5 = X1 + X2;
  	*pTrueTemperature = (B5 + 8) >> 4;
}


/**
  * @brief  Calculate true pressure
  * @param  *pTruePressure: true pressure
  * @retval None
  */
void BMP180_CalculateTruePressure(int32_t *pTruePressure)
{
	int32_t X1, X2, X3, B3, B6, P, Temp;
	uint32_t  B4, B7;
	
	B6 = B5 - 4000;             
	X1 = (B2* ((B6 * B6) >> 12)) >> 11;
	X2 = AC2 * B6 >> 11;
	X3 = X1 + X2;
	Temp = (((int32_t)AC1 << 2) + X3) << _oss;
	B3 = (Temp + 2) >> 2;
	X1 = (AC3 * B6) >> 13;
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
void BMP180_LocalpressureAvg(int32_t *pVal)
{
	uint8_t i;
	int32_t Sum = 0;
	char buffer[20];
	for(i = 0; i < 5; i ++)
	{
	  	BMP180_StartTemperatureMeasurement();
		_delay_ms(324); //4.5ms   324  // check to change to smaller
		BMP180_ReadUncompensatedTemperature();
		BMP180_StartPressureMeasurement();
		_delay_ms(540);//7.5ms    540   // check to change to smaller
		BMP180_ReadUncompensatedPressure();
		BMP180_CalculateTruePressure(&PressureVal);
		BMP180_CalculateTrueTemperature(&TemperatureVal);

		clearDisplay();
		sprintf(buffer, "UT:%i", UP);
		LCD_WriteString(buffer);
		SetAdress(64); // goes to line 2 of LCD
		sprintf(buffer, "Pres:%i ", PressureVal);
		LCD_WriteString(buffer);
		_delay_ms(1000);
		
		//sum the 3 last values
		if(i >= 2)
		{
			Sum += PressureVal;
		}
	}
	//export the mean of last 3 values
	*pVal = Sum / 3;
}


/** 
  * @brief  Calculating pressure at sea level
  * @param  None
  * @retval None
  */
void BMP180_PressureAtSeaLevel(void)
{  
	float Temp = 0.0f;
	BMP180_LocalpressureAvg(&PressureVal);
	Temp = (float)LOCAL_ADS_ALTITUDE / 4433000;
	Temp = (float)pow((1 - Temp), 5.255f);
	Pressure0 = (PressureVal - PRESSURE_OFFSET) / Temp;
}


/** 
  * @brief  Calculating absolute altitude
  * @param  *pAltitude: absolute altitude
  * @param  PressureVal: the pressure at the absolute altitude
  * @retval None
  */
void BMP180_CalculateAbsoluteAltitude(int32_t *pAltitude, int32_t PressureVal)
{
	*pAltitude = 4433000 * (1 - pow((PressureVal / (float)Pressure0), 0.1903)); 
}

/**
  * @brief  Read calibration data from the EEPROM of the BMP180
  * @param  None
  * @retval None
  */
void BMP180_ReadCalibrationData() 
{
	char buffer[20];
	uint8_t RegBuff[2];
	readI2C(BMP180_ADDRESS, CAL_AC1, RegBuff, 2);
  	AC1 = ((int16_t)RegBuff[0] <<8 | ((int16_t)RegBuff[1]));
	readI2C(BMP180_ADDRESS, CAL_AC2, RegBuff, 2);
  	AC2 = ((int16_t)RegBuff[0] <<8 | ((int16_t)RegBuff[1]));
	readI2C(BMP180_ADDRESS, CAL_AC3, RegBuff, 2);
  	AC3 = ((int16_t)RegBuff[0] <<8 | ((int16_t)RegBuff[1]));
	readI2C(BMP180_ADDRESS, CAL_AC4, RegBuff, 2);
  	AC4 = ((uint16_t)RegBuff[0] <<8 | ((uint16_t)RegBuff[1]));
	readI2C(BMP180_ADDRESS, CAL_AC5, RegBuff, 2);
  	AC5 = ((uint16_t)RegBuff[0] <<8 | ((uint16_t)RegBuff[1]));
	readI2C(BMP180_ADDRESS, CAL_AC6, RegBuff, 2);
  	AC6 = ((uint16_t)RegBuff[0] <<8 | ((uint16_t)RegBuff[1])); 
	readI2C(BMP180_ADDRESS, CAL_B1, RegBuff, 2);
  	B1 = ((int16_t)RegBuff[0] <<8 | ((int16_t)RegBuff[1])); 
	readI2C(BMP180_ADDRESS, CAL_B2, RegBuff, 2);
  	B2 = ((int16_t)RegBuff[0] <<8 | ((int16_t)RegBuff[1])); 
	readI2C(BMP180_ADDRESS, CAL_MB, RegBuff, 2);
  	MB = ((int16_t)RegBuff[0] <<8 | ((int16_t)RegBuff[1]));
	readI2C(BMP180_ADDRESS, CAL_MC, RegBuff, 2);
  	MC = ((int16_t)RegBuff[0] <<8 | ((int16_t)RegBuff[1]));
	readI2C(BMP180_ADDRESS, CAL_MD, RegBuff, 2);
  	MD = ((int16_t)RegBuff[0] <<8 | ((int16_t)RegBuff[1]));
	clearDisplay();
	sprintf(buffer, "1:%i 2:%i", AC1, AC2);
	LCD_WriteString(buffer);
	SetAdress(64); // goes to line 2 of LCD
	sprintf(buffer, "3:%i 4:%u ",AC3, AC4);
	LCD_WriteString(buffer);
	_delay_ms(10000);
	
}


/**
  * @brief  Configures hardware pressure sampling accuracy modes
  * @param  None
  * @retval None
  */
void BMP180_SetOversample(void)
{
	_oss = MODE_ULTRA_HIGHRES;
}


/**
  * @brief  initializes BMP180
  * @param  None
  * @retval None
  */
void BMP180_Init() 
{  
  	BMP180_SetOversample();
  	BMP180_ReadCalibrationData();
  	BMP180_PressureAtSeaLevel();
}


/**
  * @brief  Calculation of pressure and temperature and altitude for BMP180
  * @param  None
  * @retval None
  */
void CalTemperatureAndPressureAndAltitude()
{
	static uint8_t State = START_TEMPERATURE_MEASUREMENT;
	static BMP180_AvgTypeDef BMP180_Filter[3];
	int32_t PVal,AVal, TVal;

	switch(State)
	{
		case START_TEMPERATURE_MEASUREMENT:
			BMP180_StartTemperatureMeasurement();
			_delay_ms(5); //4.5ms
			State = READ_UT_AND_START_PRESSURE_MEASUREMENT;
			break;
			
		case READ_UT_AND_START_PRESSURE_MEASUREMENT:
			BMP180_ReadUncompensatedTemperature();
			BMP180_StartPressureMeasurement();
			_delay_ms(10);//7.5ms
			State = READ_UP_CAL_TRUE_PRESSURE_TEMPERATURE;
			break;
			
		case READ_UP_CAL_TRUE_PRESSURE_TEMPERATURE:
			BMP180_ReadUncompensatedPressure();
			BMP180_CalculateTruePressure(&PVal);
			BMP180_CalAvgValue(&BMP180_Filter[0].Index, BMP180_Filter[0].AvgBuffer, PVal - PRESSURE_OFFSET, &PressureVal);

			BMP180_CalculateAbsoluteAltitude(&AVal, PVal - PRESSURE_OFFSET);
			BMP180_CalAvgValue(&BMP180_Filter[1].Index, BMP180_Filter[1].AvgBuffer, AVal, &AltitudeVal);

			BMP180_CalculateTrueTemperature(&TVal);
			BMP180_CalAvgValue(&BMP180_Filter[2].Index, BMP180_Filter[2].AvgBuffer, TVal, &TemperatureVal);

			State = START_TEMPERATURE_MEASUREMENT;
			break;

		default:
			break;
	}	
}