﻿#include "interrupt.h"

volatile bool flagEsc = 0;
volatile bool flagWs2812 = 0;
volatile bool flagSerial = 0;
volatile bool flagAk8963Fsm = 0;
volatile bool flagBmp180Fsm = 0;

XYZ16_TypeDef currentRawAcc;
XYZ16_TypeDef currentRawGyr; 
XYZ16_TypeDef currentRawMag;
int32_t currentRawTemp;
int32_t currentRawPress;

volatile unsigned long timer0OverflowCountAk8963 = 0, timer0OverflowCountMpu9255 = 0, timer0OverflowCountBmp180 = 0;
float deltaTimeAk8963 = 0.0f, deltaTimeMpu9255 = 0.0f, deltaTimeBmp180 = 0.0f;        // time taken for the between polls of the sensors
uint8_t lastUpdateAk8963 = 0, lastUpdateMpu9255 = 0, lastUpdateBmp180 = 0; // used to calculate integration interval
uint8_t Now = 0;        // used to calculate integration interval

uint8_t stateBmp180 = START_TEMPERATURE_MEASUREMENT;
uint8_t counterBmp180Fsm = 0;

/*
PCINT0 is unused
PCINT1 is used for RF controller
*/

/**
	* @brief Sets the registers for timer 0 for interrupt control and starts interrupts
    * @param none
	* @retval None
	*/
void startInterrupt()
{
	/* by setting the max count of the counter (OCR0A) and the prescaler (with TCCR0B), the frequency of the interrupts
	generated by the counter can be controlled with precision */
	OCR0A = 0xFF;	/*Sets the maximum value of the counter before going back to 0*/
	TCCR0B |= (1 << CS01); /*prescaler 001 = 1, 010 = 8, 011 = 64, 100 = 256, 101 = 1024*/
	TCCR0A |= (1 << WGM01); /*Sets the mode of the counter to CTC to clear counter once OCR0A is reached*/
	TIMSK0 |= (1 << OCIE0A)|(1 << TOIE0); /*enable interrupt when counter reaches OCR0A*/
	
	////Use INT2 to use the interrupts generated by the IMU. Might be too fast since it is at 1kHz. Deactivated for now. 
	//EICRA |= (1<<ISC20)|(1<<ISC21); /* Sets the rising edge of INT2 to trigger interrupts */
	//EIMSK |= (1<<INT2);	/* Enables INT2 */
	
	//sets the MPU9255 for first measurement of the I2C FSM
	phys_adress = MPU9255_ADDRESS<<1;
	data_adress = MPU9255_RA_ACCEL_XOUT_H;
	data_length = 14;
	startI2CForFsm();
	
	sei();	/* Turn interrupts on */

}


/**
	* @brief ISR for reading the pwm from the radio receiver. 
	*/
ISR(PCINT1_vect) {
	if(RFInitialized)
	{
		handleFSMRF();
					
		if (RFserialSlowDownCounter >= RF_SERIAL_SPEED_DIVIDER)
		{
			RFserialSlowDownCounter = 0;
			//sprintf(buffer, "1:%u 2:%u 3:%u 4:%u \n", ch_1_pw, ch_2_pw, ch_3_pw, ch_4_pw);
			//serialTransmit(buffer);
		}
		else
		{
			RFserialSlowDownCounter++;
		}
	}
}

/**
	* @brief ISR for reading the interrupt generated by the IMU
    * @param none
	* @retval None
	*/
ISR(INT2_vect) {
	if(imuInitialized)
	{
		//flagImu = true;
	}
}

/**
	* @brief ISR for the overflow on the counter 3 used for RF PWM measurements
    * @param none
	* @retval None
	*/
ISR(TIMER3_OVF_vect) {
	channel1OverflowCount++;
	channel2OverflowCount++;
	channel3OverflowCount++;
	channel4OverflowCount++;
}


ISR(TIMER0_OVF_vect){
	timer0OverflowCountAk8963++;
	timer0OverflowCountMpu9255++;
	timer0OverflowCountBmp180++;
}

/**
	* @brief ISR for the event counter. This sets the different flags at different frequencies 
	in order to control different hardware. This is where to add different flags if you want more interrupt
	routines
    * @param none
	* @retval None
	*/
ISR(TIMER0_COMPA_vect){	
	static unsigned int EscFlagCount;
	static unsigned int Ws2812FlagCount;
	static unsigned int serialFlagCount;
	static unsigned int Ak8963FlagCount;
	static unsigned int Bmp180FlagCount;

	if(escInitialized == true){
		if(EscFlagCount >= ESC_PERIOD){
			flagEsc = true;
			EscFlagCount = 0;
		}
		else {
			EscFlagCount++;
		}
	}

	if(ws2812Initialized == true){
		if(Ws2812FlagCount >= WS2812_PERIOD){
			flagWs2812 = true;
			Ws2812FlagCount = 0;
		}
		else {
			Ws2812FlagCount++;
		}
	}

	if(serialInitialized == true){
		if(serialFlagCount >= SERIAL_PERIOD){
			flagSerial = true;
			serialFlagCount = 0;
		}
		else {
			serialFlagCount++;
		}
	}

	if(ak8963Initialized == true){
		if(Ak8963FlagCount >= AK8963_PERIOD){
			flagAk8963Fsm = true;
			Ak8963FlagCount = 0;
		}
		else {
			Ak8963FlagCount++;
		}
	}
	
	if(bmp180Initialized == true){
		if(Bmp180FlagCount >= BMP180_PERIOD){
			flagBmp180Fsm = true;
			Bmp180FlagCount = 0;
		}
		else {
			Bmp180FlagCount++;
		}
	}
}


/**
	* @brief ISR for reading the interrupt generated by the USART 
    * @param none
	* @retval None
	*/
ISR(USART0_RX_vect) {
	if(serialInitialized)
	{
		serialReceive();
	}
}

/**
	* @brief ISR for reading the interrupt generated by the USART
    * @param none
	* @retval None
	*/
ISR(USART0_TX_vect) {
	if(serialInitialized)
	{
		serialTransmitINT();
	}
}

ISR(TWI_vect) {
	if(TwiInterruptInitialized)
	{
		if(handleFsmI2c() == 0) // I2C read has completed successfully
		{			
			// Update the current values in the interrupt buffers
			switch(lastRead)
			{
				case WRITE:
					break;
				case MPU9255_READ:
					currentRawAcc.X = (dataReadBuffer[0] << 8) | dataReadBuffer[1];
					currentRawAcc.Y = (dataReadBuffer[2] << 8) | dataReadBuffer[3];
					currentRawAcc.Z = (dataReadBuffer[4] << 8) | dataReadBuffer[5];
					currentRawGyr.X = (dataReadBuffer[8] << 8) | dataReadBuffer[9];
					currentRawGyr.Y = (dataReadBuffer[10] << 8) | dataReadBuffer[11];
					currentRawGyr.Z = (dataReadBuffer[12] << 8) | dataReadBuffer[13];
					mpu9255DataReady = true;
					break;
				case AK8963_READ:
					currentRawMag.X = (dataReadBuffer[1] << 8) | dataReadBuffer[0];
					currentRawMag.Y = (dataReadBuffer[3] << 8) | dataReadBuffer[2];
					currentRawMag.Z = (dataReadBuffer[5] << 8) | dataReadBuffer[4];
					ak8963DataReady = true;				
					break;
				case BMP180_READ_TEMPERATURE:
					currentRawTemp = ((int32_t)dataReadBuffer[0] << 8) | (int32_t)dataReadBuffer[1];
					break;
				case BMP180_READ_PRESSURE:
					currentRawPress = (((int32_t)dataReadBuffer[0] << 16) | ((int32_t)dataReadBuffer[1] << 8) | (int32_t)dataReadBuffer[2])>> (8 - BMP180::_oss);
					bmp180DataReady = true;	
					break;
				default:
					lastRead = MPU9255_READ;
					break;
			}
			
			// Put the right address for the next read depending on flags.
			// Default is MPU9255 and AK8963/BMP180 happen at the frequency
			// set in interrupt.h
			if(flagAk8963Fsm)
			{
				flagAk8963Fsm = false;
				lastRead = AK8963_READ;
				phys_adress = AK8963_ADDRESS<<1;
				data_adress = AK8963_RA_HXL;
				data_length = 7;
				// Send start condition
				TWCR = 0;
				TWCR = (1<<TWINT)|(1<<TWIE)|(1<<TWSTA)|(1<<TWEN);
			}
			else if(flagBmp180Fsm)
			{
				flagBmp180Fsm = false;
				switch(stateBmp180)
				{
					case START_TEMPERATURE_MEASUREMENT:
						lastRead = WRITE;
						phys_adress = BMP180_ADDRESS<<1;
						data_adress = BMP180_RA_CONTROL;
						dataToWrite[0] = READ_TEMPERATURE;
						data_length = 1;
		
						// to double this state before passing to the next one
						if(counterBmp180Fsm > 0)
						{
							counterBmp180Fsm = 0;
							stateBmp180 = READ_UT_AND_START_PRESSURE_MEASUREMENT;
						}
						else
						{
							counterBmp180Fsm++;
						}
						break;
					case READ_UT_AND_START_PRESSURE_MEASUREMENT:
						if(counterBmp180Fsm == 0)
						{
							lastRead = BMP180_READ_TEMPERATURE;
							phys_adress = BMP180_ADDRESS<<1;
							data_adress = BMP180_RA_CONTROL_OUTPUT;
							data_length = 2;
							counterBmp180Fsm++;	
						}
						else 
						{
							lastRead = WRITE;
							phys_adress = BMP180_ADDRESS<<1;
							data_adress = BMP180_RA_CONTROL;
							dataToWrite[0] = READ_PRESSURE + (BMP180::_oss << 6);
							data_length = 1;
							if(counterBmp180Fsm >= 2)
							{
								stateBmp180 = READ_UP_AND_CAL_TRUE_PRESSURE_AND_TEMPERATURE;
								counterBmp180Fsm = 0;
							}
							else
							{
								counterBmp180Fsm++;
							}
						}
						break;
					case READ_UP_AND_CAL_TRUE_PRESSURE_AND_TEMPERATURE:
						lastRead = BMP180_READ_PRESSURE;
						phys_adress = BMP180_ADDRESS<<1;
						data_adress = BMP180_RA_CONTROL_OUTPUT;
						data_length = 3;
						stateBmp180 = START_TEMPERATURE_MEASUREMENT;			
						break;
					default:
						stateBmp180 = START_TEMPERATURE_MEASUREMENT;
						counterBmp180Fsm = 0;
						break;
				}
				// Send start condition
				TWCR = 0;
				TWCR = (1<<TWINT)|(1<<TWIE)|(1<<TWSTA)|(1<<TWEN);
			}
			else
			{
				lastRead = MPU9255_READ;	
				phys_adress = MPU9255_ADDRESS<<1;
				data_adress = MPU9255_RA_ACCEL_XOUT_H;
				data_length = 14;
				// Send start condition
				TWCR = 0;
				TWCR = (1<<TWINT)|(1<<TWIE)|(1<<TWSTA)|(1<<TWEN);			
			}

		}
	}
}