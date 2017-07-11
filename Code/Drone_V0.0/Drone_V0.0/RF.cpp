#include "RF.h"

uint16_t ch_1_pw = 0, ch_2_pw = 0, ch_3_pw = 0, ch_4_pw = 0;
bool RFInitialized = false;
AvgTypeDef RF_filter[4]; //one for each channel

uint8_t channel1OverflowCount = 0, channel2OverflowCount = 0, channel3OverflowCount = 0, channel4OverflowCount = 0;
/*
Channel du radio controller
http://www.rcgroups.com/forums/showthread.php?t=1051701#post12275676

*pinout for radio receiver
* J2 = CH4
* J3 = CH3
* J4 = CH2
* J5 = CH1
*/


/**
	* @brief set up PCINT1 to trigger interrupt, set up counter 3 to measure the pulse width of the incoming PWM, and
	initialize variables used to measure PWM and starts the counter
    * @param none
	* @retval None
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
    * @param none
	* @retval None
	*/
void handleFSMRF(void){
	//initialize variables
	static bool last_ch_1 = false, last_ch_2 = false, last_ch_3 = false, last_ch_4 = false;
	static bool ch_1_counting = false, ch_2_counting = false, ch_3_counting = false, ch_4_counting = false;
	static uint16_t count_ch_1, count_ch_2, count_ch_3, count_ch_4;
	static bool timer_3_ovf,ch_2_ovf_rising,ch_3_ovf_rising,ch_4_ovf_rising;
	static uint16_t last_ch_4_pw = (CHANNEL_4_MAX_PWM+CHANNEL_4_MIN_PWM)/2;
	
	////channel_1
	//if (PINJ & 0b00100000 && last_ch_1 == false) //Rising edge
	//{	last_ch_1 = true; ch_1_counting = true;
		//channel1OverflowCount = 0; //set the overflow count to 0
		//count_ch_1 = TCNT3L | TCNT3H << 8; //record the counter value for the rising edge
	//}
	//else if (!(PINJ & 0b00100000) && last_ch_1 == true) //Falling edge
	//{	last_ch_1 = false;
		////subtract current counter to old one in count_ch_1 and store variable in ch_1_pw
		////only if there is no overflow at rising and falling edge or if there is an overflow for both
		//if(channel1OverflowCount == 0) ch_1_pw = (TCNT3L | TCNT3H << 8) - count_ch_1;
		//else ch_1_pw = 65535 - count_ch_1 + (TCNT3L | TCNT3H << 8) + 1; //else there is an overflow calculate the real pulse time by adding 2^16
		//ch_1_counting = false;
	//}
	
	
	//channel_1
	if (PINJ & 0b00100000 && last_ch_1 == false) //Rising edge
	{	last_ch_1 = true; ch_1_counting = true;
		channel1OverflowCount = 0; //set the overflow count to 0
		count_ch_1 = TCNT3L | TCNT3H << 8; //record the counter value for the rising edge
	}
	else if (!(PINJ & 0b00100000) && last_ch_1 == true) //Falling edge
	{	last_ch_1 = false;
		//subtract current counter to old one in count_ch_1 and store variable in ch_1_pw
		//only if there is no overflow at rising and falling edge or if there is an overflow for both
		if(channel1OverflowCount == 0) ch_1_pw = (TCNT3L | TCNT3H << 8) - count_ch_1;
		else if (channel1OverflowCount == 1) ch_1_pw = 65535 - count_ch_1 + (TCNT3L | TCNT3H << 8) + 1; //else there is an overflow calculate the real pulse time by adding 2^16
		else serialTransmit("RF channel 1 overflow error");
		ch_1_counting = false;
	}
	
	////channel_2
	//if (PINJ & 0b00010000 && last_ch_2 == false) //Rising edge
	//{	last_ch_2 = true; ch_2_counting = true;
		//if(timer_3_ovf) ch_2_ovf_rising = true; //verify if there is an overflow before starting to count
		//else ch_2_ovf_rising = false;
		//count_ch_2 = TCNT3L | TCNT3H << 8; //record the counter value for the rising edge
	//}
	//else if (!(PINJ & 0b00010000) && last_ch_2 == true) //Falling edge
	//{	last_ch_2 = false;
		////subtract current counter to old one in count_ch_2 and store variable in ch_2_pw
		////only if there is no overflow at rising and falling edge or if there is an overflow for both
		//if((!timer_3_ovf && !ch_2_ovf_rising) || (timer_3_ovf && ch_2_ovf_rising)) ch_2_pw = (TCNT3L | TCNT3H << 8) - count_ch_2;
		//else ch_2_pw = 65535 - count_ch_2 + (TCNT3L | TCNT3H << 8) + 1; //else there is an overflow calculate the real pulse time by adding 2^16
		//ch_2_counting = false;
	//}
	//
	////channel_3
	//if (PINJ & 0b00001000 && last_ch_3 == false) //Rising edge
	//{	last_ch_3 = true; ch_3_counting = true;
		//if(timer_3_ovf) ch_3_ovf_rising = true; //verify if there is an overflow before starting to count
		//else ch_3_ovf_rising = false;
		//count_ch_3 = TCNT3L | TCNT3H << 8; //record the counter value for the rising edge
	//}
	//else if (!(PINJ & 0b00001000) && last_ch_3 == true) //Falling edge
	//{	last_ch_3 = false;
		////subtract current counter to old one in count_ch_3 and store variable in ch_3_pw
		////only if there is no overflow at rising and falling edge or if there is an overflow for both
		//if((!timer_3_ovf && !ch_3_ovf_rising) || (timer_3_ovf && ch_3_ovf_rising)) ch_3_pw = (TCNT3L | TCNT3H << 8) - count_ch_3;
		//else ch_3_pw = 65535 - count_ch_3 + (TCNT3L | TCNT3H << 8) + 1; //else there is an overflow calculate the real pulse time by adding 2^16
		//ch_3_counting = false;
	//}
	//
	////channel_4
	//if (PINJ & 0b00000100 && last_ch_4 == false) //Rising edge
	//{	last_ch_4 = true; ch_4_counting = true;
		//if(timer_3_ovf) ch_4_ovf_rising = true; //verify if there is an overflow before starting to count
		//else ch_4_ovf_rising = false;
		//count_ch_4 = TCNT3L | TCNT3H << 8; //record the counter value for the rising edge
	//}
	//else if (!(PINJ & 0b00000100) && last_ch_4 == true) //Falling edge
	//{	last_ch_4 = false;
		////subtract current counter to old one in count_ch_4 and store variable in ch_4_pw
		////only if there is no overflow at rising and falling edge or if there is an overflow for both
		//if((!timer_3_ovf && !ch_4_ovf_rising) || (timer_3_ovf && ch_4_ovf_rising)) ch_4_pw = (TCNT3L | TCNT3H << 8) - count_ch_4;
		//else ch_4_pw = 65535 - count_ch_4 + (TCNT3L | TCNT3H << 8) + 1; //else there is an overflow calculate the real pulse time by adding 2^16
		//ch_4_counting = false;
	//}
	//
	////remove the overflow flag only when no channel is counting to avoid problems
	////when overflow happens when 2 or more channels are counting simultaneously
	//if (!ch_1_counting && !ch_2_counting && !ch_3_counting && !ch_4_counting) timer_3_ovf=false;			
	////do the same for every channel when cleaned up	

}