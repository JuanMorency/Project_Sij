#define F_CPU 1000000
#define min_PWM 0x01
#define max_PWM 0x06

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include "esc.h"
#include "interrupt.h"
#include "lcd.h"

int main()
{
	//char* buffers for printing stuff on the LCD
	char buffer[20];
	//create ESC object
	esc allESC;
	// initialize ESC
	allESC.initialize();
	//initialize INT
	initializeInterrupt();
	//initialize Radio Controller counter
	initializecounterPWMread();
	//initialize LCD
	initLCD(); 		/* configure LCD */
	
	while(1)
	{	
		if (button_falling)
		{
			OCR4A = 1400/2;
			OCR4B = 1400/2;
			OCR5A = 1400/2;
			OCR5B = 1400/2;
			_delay_ms(1000);
			button_falling = false;
		}
		else if (button_rising)
		{				
			button_rising = false;
		}
		else
		{
			//the divide by 2 here is required because of the way the ESC is set up
			//when we will have a frequency of 16MHz we will actually have to multiply by 8
			OCR4A = ch_3_pw/2;
			OCR4B = ch_3_pw/2;
			OCR5A = ch_3_pw/2;
			OCR5B = ch_3_pw/2;
			clearDisplay();
			sprintf(buffer, "%u    %u     ", ch_1_pw, ch_2_pw);
			LCD_WriteString(buffer);
			SetAdress(64); // goes to line 2 of LCD
			sprintf(buffer, "%u    %u     ", ch_3_pw, ch_4_pw);
			LCD_WriteString(buffer);
			_delay_ms(200);
		}

	}
	return 0;
}
