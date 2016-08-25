/*
 * test_uC_1.cpp
 *
 * Created: 8/25/2016 1:06:20 PM
 * Author : juan-
 */ 
#ifndef F_CPU
#define F_CPU 1000000UL // 1 MHz clock speed
#endif

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRD = 0xFF; //Makes PORTC as Output
	while(1) //infinite loop
	{
		PORTD = 0xFF; //Turns ON All LEDs
		_delay_ms(1000); //1 second delay
		PORTD= 0x00; //Turns OFF All LEDs
		_delay_ms(1000); //1 second delay
	}
}
