#include "interrupt.h"

/*
Channel du radio controller
http://www.rcgroups.com/forums/showthread.php?t=1051701#post12275676
*/

/*
PCINT0 est configuré pour un bouton 
PCINT1 est configuré pour le radio controller
*/
bool button_rising = false;
bool button_falling = false;

static bool last_ch_1, last_ch_2, last_ch_3, last_ch_4;
static bool ch_1_counting, ch_2_counting, ch_3_counting, ch_4_counting;
uint16_t ch_1_pw, ch_2_pw, ch_3_pw, ch_4_pw;
static uint16_t count_ch_1, count_ch_2, count_ch_3, count_ch_4;
static bool timer_3_ovf, ch_1_ovf_rising, ch_2_ovf_rising, ch_3_ovf_rising, ch_4_ovf_rising;

void initializeInterrupt()
{
	PCICR |= (1<<PCIE0) | (1<<PCIE1); /* enable pin change interrupt 0 and 1 */
	PCMSK0 |= (1<<PCINT4); /* enable pin B0 to trigger PCINT0 */
	PCMSK1 |= (1<<PCINT12) | (1<<PCINT13) | (1<<PCINT14) | (1<<PCINT15); /* enable pin J3-J6 to trigger PCINT1 */
	sei();	/* Turn interrupts on */
}

void initializecounterPWMread()
{
	last_ch_1 = false, last_ch_2 = false, last_ch_3 = false, last_ch_4 = false;
	ch_1_counting = false, ch_2_counting = false, ch_3_counting = false, ch_4_counting = false;
	ch_1_pw = 0, ch_2_pw = 0, ch_3_pw = 0, ch_4_pw = 0;
	//counter for measuring pulse width of incoming radio signal
	TCCR3B |= (1 << CS30); //set-up counter in normal mode with prescaler = 1
	TIMSK3 |= (1 << TOIE3); // enable interrupt on overflow
}
//for button press
ISR(PCINT0_vect) {
	//read pin PB4 and if 1 this must be a rising edge change
	if (PINB & 0b00010000) button_rising = true;
	else button_falling = true;
	_delay_ms(20); //debounce
}
//for reading the pwm receiver for the radio receiver
ISR(PCINT1_vect) {
	//channel_1
	if (PINJ & 0b00001000 && last_ch_1 == false) //Rising edge
	{	last_ch_1 = true;
		ch_1_counting = true;
		//verify if there is an overflow before starting to count
		if(timer_3_ovf) 
			ch_1_ovf_rising = true;
		else 
			ch_1_ovf_rising = false;
		//record the counter value for the rising edge
		count_ch_1 = TCNT3L | TCNT3H << 8;		
	}
	else if (!(PINJ & 0b00001000) && last_ch_1 == true) //Falling edge
	{	last_ch_1 = false;
		//subtract current counter to old one in count_ch_1 and store variable in ch_1_pw
		//only if there is no overflow at rising and falling edge or if there is an overflow for both
		if((!timer_3_ovf && !ch_1_ovf_rising) || (timer_3_ovf && ch_1_ovf_rising)) 
			ch_1_pw = (TCNT3L | TCNT3H << 8) - count_ch_1;
		//else there is an overflow calculate the real pulse time by adding 2^16
		else 
			ch_1_pw = 65535 - count_ch_1 + (TCNT3L | TCNT3H << 8) + 1;				
		ch_1_counting = false;
		//remove the overflow flag only when no channel is counting to avoid problems 
		//when overflow happens when 2 or more channels are counting simultaneously
		if (!ch_1_counting && !ch_2_counting && !ch_3_counting && !ch_4_counting) timer_3_ovf=false;
	}
	
	
	//channel_2
	if (PINJ & 0b00010000 && last_ch_2 == false) //Rising edge
	{	last_ch_2 = true;
		ch_2_counting = true;
		//verify if there is an overflow before starting to count
		if(timer_3_ovf) ch_2_ovf_rising = true;
		else ch_2_ovf_rising = false;
		//record the counter value for the rising edge
		count_ch_2 = TCNT3L | TCNT3H << 8;
	}
	else if (!(PINJ & 0b00010000) && last_ch_2 == true) //Falling edge
	{	last_ch_2 = false;
		//subtract current counter to old one in count_ch_2 and store variable in ch_2_pw
		//only if there is no overflow at rising and falling edge or if there is an overflow for both
		if((!timer_3_ovf && !ch_2_ovf_rising) || (timer_3_ovf && ch_2_ovf_rising) ) ch_2_pw = (TCNT3L | TCNT3H << 8) - count_ch_2;
		//else there is an overflow calculate the real pulse time by adding 2^16
		else ch_1_pw = 65535 - count_ch_2 + (TCNT3L | TCNT3H << 8) + 1;
		ch_2_counting = false;
		//remove the overflow flag only when no channel is counting to avoid problems
		//when overflow happens when 2 channels are counting
		if (!ch_1_counting && !ch_2_counting && !ch_3_counting && !ch_4_counting) timer_3_ovf=false;
	}
	
	
	//channel_3
	if (PINJ & 0b00100000 && last_ch_3 == false) //Rising edge
	{	last_ch_3 = true;
		ch_3_counting = true;
		//verify if there is an overflow before starting to count
		if(timer_3_ovf) ch_3_ovf_rising = true;
		else ch_3_ovf_rising = false;
		//record the counter value for the rising edge
		count_ch_3 = TCNT3L | TCNT3H << 8;
	}
	else if (!(PINJ & 0b00100000) && last_ch_3 == true) //Falling edge
	{	last_ch_3 = false;
		//subtract current counter to old one in count_ch_3 and store variable in ch_3_pw
		//only if there is no overflow at rising and falling edge or if there is an overflow for both
		if((!timer_3_ovf && !ch_3_ovf_rising) || (timer_3_ovf && ch_3_ovf_rising) ) ch_3_pw = (TCNT3L | TCNT3H << 8) - count_ch_3;
		//else there is an overflow calculate the real pulse time by adding 2^16
		else ch_3_pw = 65535 - count_ch_3 + (TCNT3L | TCNT3H << 8) + 3;
		ch_3_counting = false;
		//remove the overflow flag only when no channel is counting to avoid problems
		//when overflow happens when 2 channels are counting
		if (!ch_1_counting && !ch_2_counting && !ch_3_counting && !ch_4_counting) timer_3_ovf=false;
	}
	
	
	//channel_4
	if (PINJ & 0b01000000 && last_ch_4 == false) //Rising edge
	{	last_ch_4 = true;
		ch_4_counting = true;
		//verify if there is an overflow before starting to count
		if(timer_3_ovf) ch_4_ovf_rising = true;
		else ch_4_ovf_rising = false;
		//record the counter value for the rising edge
		count_ch_4 = TCNT3L | TCNT3H << 8;
	}
	else if (!(PINJ & 0b01000000) && last_ch_4 == true) //Falling edge
	{	last_ch_4 = false;
		//subtract current counter to old one in count_ch_4 and store variable in ch_4_pw
		//only if there is no overflow at rising and falling edge or if there is an overflow for both
		if((!timer_3_ovf && !ch_4_ovf_rising) || (timer_3_ovf && ch_4_ovf_rising) ) ch_4_pw = (TCNT3L | TCNT3H << 8) - count_ch_4;
		//else there is an overflow calculate the real pulse time by adding 2^16
		else ch_4_pw = 65535 - count_ch_4 + (TCNT3L | TCNT3H << 8) + 4;
		ch_4_counting = false;
		//remove the overflow flag only when no channel is counting to avoid problems
		//when overflow happens when 2 channels are counting
		if (!ch_1_counting && !ch_2_counting && !ch_3_counting && !ch_4_counting) timer_3_ovf=false;
	}
	
	//do the same for every channel when cleaned up
	
}

ISR(TIMER3_OVF_vect) {
	timer_3_ovf=true;
	TIFR3 &= ~(1 << TOV3);
}