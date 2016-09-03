#define F_CPU 1000000
#define min_PWM 0x01
#define max_PWM 0x06

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "esc.h"
#include "interrupt.h"

int main()
{
	//create ESC object
	esc allESC;
	// initialize ESC
	allESC.initialize();
	//initialize INT
	initializeInterrupt();
	
	// run forever
	while(1)
	{
		if (button_rising)
		{
			OCR4AH = 0x02;
			OCR4AL = 0x80;
			_delay_ms(1000);
			button_rising = 0;
		}
		else if (button_falling)
		{
			OCR4AH = 0x03;
			OCR4AL = 0x00;
			_delay_ms(1000);
			button_falling = 0;
		}
		else
		{
			OCR4AH = 0x02;
			OCR4AL = 0x00;	
		}


	}
	return 0;
}
