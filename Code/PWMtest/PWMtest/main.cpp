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
//#include "fonction.h"

int main()
{
	uint16_t temp = 550;
	//char* buffers for the use of itoa
	char str[17];
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
	//LCD_WriteString("  Simon va se"); 	/* The 2 initial space are for centering */
	//SetAdress(64); // goes to line 2 of LCD
	//LCD_WriteString("  chier dessus"); 
	// run forever
	while(1)
	{
		
		if (button_falling)
		{
			OCR4A = 1300;			
			OCR4B = 1300;	
			//OCR4AL = 0x00;
			//OCR4AH = 0x03;
			//OCR4BL = 0x00;
			//OCR4BH = 0x03;
			_delay_ms(1000);
			button_falling = false;
		}
		else if (button_rising)
		{		
			//OCR4AL = 0x00;
			//OCR4AH = 0x02;
			//OCR4BL = 0x00;
			//OCR4BH = 0x02;			
			button_rising = false;
		}
		else
		{
			OCR4A = ch_3_pw;
			OCR4B = ch_3_pw;
		}
			clearDisplay();
			sprintf(str, "%u    %u     ", OCR4A, OCR4B);
			LCD_WriteString(str);
			SetAdress(64); // goes to line 2 of LCD
			sprintf(str, "%u    %u     ", ch_3_pw, ch_4_pw);
			LCD_WriteString(str);
			_delay_ms(200);
			temp++;
	}
	return 0;
}
