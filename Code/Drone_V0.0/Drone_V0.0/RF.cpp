/**
******************************************************************************
* File Name         : RF.cpp
* Description       : Class for RF receiver interface
* Author			: Juan Morency Trudel
* Version           : 1.0.0
* Date				: September 2016
******************************************************************************
*/

#include "RF.h"

int16_t ch_1_pw = CHANNEL_1_MEAN, ch_2_pw = CHANNEL_2_MEAN, ch_3_pw = CHANNEL_3_MIN_PWM, ch_4_pw = CHANNEL_4_MEAN;
bool RFInitialized = false;
AvgTypeDef RF_filter[4]; //one for each channel
bool flagRfOn = false;
bool rfActivity = false;

uint8_t channel1OverflowCount = 0, channel2OverflowCount = 0, channel3OverflowCount = 0, channel4OverflowCount = 0;
/*
* Channel of radio controller
* http://www.rcgroups.com/forums/showthread.php?t=1051701#post12275676

* Pinout for radio receiver
* J2 = CH4
* J3 = CH3
* J4 = CH2
* J5 = CH1
*/


/**
	* @brief set up PCINT1 to trigger interrupt, set up counter 3 to measure the pulse width of the incoming PWM, and
	* initialize variables used to measure PWM and starts the counter
	*/
void initRF()
{
	PCICR |= (1<<PCIE1); /* enable pin change interrupt 1 */
	//PCMSK0 |= (1<<PCINT4); /* enable pin B0 to trigger PCINT0 */
	PCMSK1 |= (1<<PCINT11) | (1<<PCINT12) | (1<<PCINT13) | (1<<PCINT14); /* enable pin J3-J6 to trigger PCINT1 */
	
	//counter for measuring pulse width of incoming radio signal
	TCCR3B |= (1 << CS31); //set-up counter in normal mode with prescaler = 8
	TIMSK3 |= (1 << TOIE3); // enable interrupt on overflow
	RFInitialized = true;
}

/**
	* @brief ISR for reading the pwm from the radio receiver. How it works: When the interrupt is detected
	* on a pin change, checks which channel triggered it by storing all the values of the pins and then checking which
	* one changed. To measure the width of the pulse, a counter (2 MHz) is used to measure the time it took
	* from a rising edge to a falling edge. This pulse width is the important information and is stored in the
	* ch_x_pw variables. The counter always runs to avoid having issues when 2 channels are triggered at the same time
	* and overflow protection is implemented. 
	*/
void handleFSMRF(void){	
	//initialize variables
	static bool last_ch_1 = false, last_ch_2 = false, last_ch_3 = false, last_ch_4 = false;
	static uint16_t count_ch_1, count_ch_2, count_ch_3, count_ch_4;
	
	//channel_1
	if (PINJ & 0b00100000 && last_ch_1 == false) //Rising edge
	{	
		last_ch_1 = true; channel1OverflowCount = 0; //set the overflow count to 0
		count_ch_1 = TCNT3L | TCNT3H << 8; //record the counter value for the rising edge
	}
	else if (!(PINJ & 0b00100000) && last_ch_1 == true) //Falling edge
	{	
		last_ch_1 = false;
		// Subtract current counter to old one in count_ch_1 and store variable in ch_1_pw, add 2^16 if overflow
		if(channel1OverflowCount == 0) ch_1_pw = (TCNT3L | TCNT3H << 8) - count_ch_1;
		else if (channel1OverflowCount == 1) ch_1_pw = 65535 - count_ch_1 + (TCNT3L | TCNT3H << 8); 
		else serialTransmit("RF channel 1 overflow error");
	}
	
	//channel_2
	if (PINJ & 0b00010000 && last_ch_2 == false) //Rising edge
	{	
		last_ch_2 = true; channel2OverflowCount = 0; //set the overflow count to 0
		count_ch_2 = TCNT3L | TCNT3H << 8; //record the counter value for the rising edge
	}
	else if (!(PINJ & 0b00010000) && last_ch_2 == true) //Falling edge
	{
		last_ch_2 = false;
		// Subtract current counter to old one in count_ch_2 and store variable in ch_2_pw, add 2^16 if overflow
		if(channel2OverflowCount == 0) ch_2_pw = (TCNT3L | TCNT3H << 8) - count_ch_2;
		else if (channel2OverflowCount == 1) ch_2_pw = 65535 - count_ch_2 + (TCNT3L | TCNT3H << 8);
		else serialTransmit("RF channel 2 overflow error");
	}
	
	//channel_3
	if (PINJ & 0b00001000 && last_ch_3 == false) //Rising edge
	{
		last_ch_3 = true; channel3OverflowCount = 0; //set the overflow count to 0
		count_ch_3 = TCNT3L | TCNT3H << 8; //record the counter value for the rising edge
	}
	else if (!(PINJ & 0b00001000) && last_ch_3 == true) //Falling edge
	{
		last_ch_3 = false;
		// Subtract current counter to old one in count_ch_3 and store variable in ch_3_pw, add 2^16 if overflow
		if(channel3OverflowCount == 0) ch_3_pw = (TCNT3L | TCNT3H << 8) - count_ch_3;
		else if (channel3OverflowCount == 1) ch_3_pw = 65535 - count_ch_3 + (TCNT3L | TCNT3H << 8);
		else serialTransmit("RF channel 3 overflow error");
	}
	
	//channel_4
	if (PINJ & 0b00000100 && last_ch_4 == false) //Rising edge
	{	
		last_ch_4 = true; channel4OverflowCount = 0; //set the overflow count to 0
		count_ch_4 = TCNT3L | TCNT3H << 8; //record the counter value for the rising edge
	}
	else if (!(PINJ & 0b00000100) && last_ch_4 == true) //Falling edge
	{
		last_ch_4 = false;
		// Subtract current counter to old one in count_ch_4 and store variable in ch_4_pw, add 2^46 if overflow
		if(channel4OverflowCount == 0) ch_4_pw = (TCNT3L | TCNT3H << 8) - count_ch_4;
		else if (channel4OverflowCount == 1) ch_4_pw = 65535 - count_ch_4 + (TCNT3L | TCNT3H << 8);
		else serialTransmit("RF channel 4 overflow error");
	}

}

/** 
 * @brief converts the RF value to a degree angle 
 * @param angleId : YAW, PITCH or ROLL
 * @return float of the desired angle in degrees
 */
float getDesiredAngleFromRf(uint8_t angleId)
{
	switch(angleId)
	{
		case YAW:
			if (ch_4_pw > MIN_PWM_DETECT)
			{
				if (ch_4_pw-CHANNEL_4_MEAN > YAW_RF_THRESHOLD || ch_4_pw-CHANNEL_4_MEAN < -YAW_RF_THRESHOLD)
				{
					return (float)(ch_4_pw-CHANNEL_4_MEAN)*CHANNEL_4_MULTIPLIER*DESIRED_YAW_SENSITIVITY_TO_RF_MULTIPLIER;				
				}
				else return 0;
			}
			else return 0;
			break;
		case PITCH:
			if (ch_2_pw > MIN_PWM_DETECT)
			{			
				if (ch_2_pw-CHANNEL_2_MEAN > PITCH_RF_THRESHOLD || ch_2_pw-CHANNEL_2_MEAN < -PITCH_RF_THRESHOLD)
				{
					return (float)(ch_2_pw-CHANNEL_2_MEAN)*CHANNEL_2_MULTIPLIER;
				}
				else return 0;
			}
			else return 0;
			break;
		case ROLL:
			if (ch_1_pw > MIN_PWM_DETECT)
			{
				if (ch_1_pw-CHANNEL_1_MEAN > ROLL_RF_THRESHOLD || ch_1_pw-CHANNEL_1_MEAN < -ROLL_RF_THRESHOLD)
				{
					return (float)(ch_1_pw-CHANNEL_1_MEAN)*CHANNEL_1_MULTIPLIER;
				}
				else return 0;
			}
			else return 0;
			break;
		default:
			turnDebugLedOn(1);
			return 0;
			break;

	}
}