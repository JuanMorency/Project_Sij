#define F_CPU 1000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "esc.cpp"
#include "esc.h"

int main()
{
	//create ESC object
	esc allESC;
	// initialize ESC
	allESC.initialize();
	
	// run forever
	while(1)
	{
		// increment from 0 to 4096
		for (uint8_t pulse_width_H = 0; pulse_width_H < 8; pulse_width_H++)
		{
			// set pulse width
			OCR4AH = pulse_width_H;
			OCR4BH = pulse_width_H;
			OCR5AH = pulse_width_H;
			OCR5BH = pulse_width_H;
			for(uint8_t pulse_width_L = 0; pulse_width_L < 0xFF; pulse_width_L++)
			{
				// set pulse width
				OCR4AH = pulse_width_L;
				OCR4BH = pulse_width_L;
				OCR5AH = pulse_width_L;
				OCR5BH = pulse_width_L;
				_delay_ms(1);
			}
		}
		
		// increment from 0 to 4096
		for (uint8_t pulse_width_H = 8; pulse_width_H > 0; pulse_width_H--)
		{
			// set pulse width
			OCR4AH = pulse_width_H;
			OCR4BH = pulse_width_H;
			OCR5AH = pulse_width_H;
			OCR5BH = pulse_width_H;
			for(uint8_t pulse_width_L = 0xFF; pulse_width_L > 0x00; pulse_width_L--)
			{
				// set pulse width
				OCR4AH = pulse_width_L;
				OCR4BH = pulse_width_L;
				OCR5AH = pulse_width_L;
				OCR5BH = pulse_width_L;
				_delay_ms(1);
			}
		}
		// repeat this forever
	}
	return 0;
}
