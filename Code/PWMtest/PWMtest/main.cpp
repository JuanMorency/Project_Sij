#define F_CPU 1000000
#define min_PWM 0x01
#define max_PWM 0x06

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <stdio.h>
#include "esc.h"
#include "interrupt.h"
#include "lcd.h"


int main()
{
	//char* buffers for the use of itoa
	char str[6];
	ch_1_pw = 22;
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
	LCD_WriteString("  Simon va se"); 	/* The 2 initial space are for centering */
	SetAdress(64); // goes to line 2 of LCD
	LCD_WriteString("  chier dessus"); 
	// run forever
	
	while(1)
	{
		if (button_falling)
		{
			clearDisplay();
			returnHome();
			LCD_WriteString(" don't push the"); 	/* The 2 initial space are for centering */
			SetAdress(64); // goes to line 2 of LCD
			LCD_WriteString("   damn button");
			OCR4AH = 0x03;
			OCR4AL = 0x00;
			//_delay_ms(1000);
			button_falling = false;
		}
		else if (button_rising)
		{
			clearDisplay();
			sprintf(str, "%hd", 1209);
			LCD_WriteString(str);
			//_delay_ms(1000);
			button_rising = false;
		}
		else
		{
			OCR4AH = 0x01;
			OCR4AL = 0x00;	
		}


	}
	return 0;
}
