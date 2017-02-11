/*
 * test_uC_1.cpp
 *
 * Created: 8/25/2016 1:06:20 PM
 * Author : juan-
 */ 
#ifndef F_CPU
#define F_CPU 16000000UL // 1 MHz clock speed
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "lcd.h"
#include "TWI_Master.h"

int main(void)
{
	////initialize LCD
	initLCD(); 		// configure LCD
	////char* buffers for printing stuff on the LCD
	char buffer[20];
	
	clearDisplay();
	sprintf(buffer, "TWCR:%X, TWSR:%X", TWCR, TWSR);
	LCD_WriteString(buffer);
	_delay_ms(1000);
  TWBR = TWI_TWBR;                                  // Set bit rate register (Baudrate). Defined in header file.
  // TWSR = TWI_TWPS;                                  // Not used. Driver presumes prescaler to be 00.
  TWDR = 0xFF;                                      // Default content = SDA released.
  TWCR = (1<<TWEN)|                                 // Enable TWI-interface and release TWI pins.
  (0<<TWIE)|(0<<TWINT)|                      // Disable Interupt.
  (0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|           // No Signal requests.
  (0<<TWWC);                             //
  
  
  
  	clearDisplay();
  	sprintf(buffer, "TWCR:%X, TWSR:%X", TWCR, TWSR);
  	LCD_WriteString(buffer);
  	_delay_ms(1000);
	  
	  
  TWCR = (1<<TWEN)|                             // TWI Interface enabled.
  (1<<TWIE)|(1<<TWINT)|                  // Enable TWI Interupt and clear the flag.
  (0<<TWEA)|(1<<TWSTA)|(0<<TWSTO)|       // Initiate a START condition.
  (0<<TWWC);                             //
  
   	clearDisplay();
   	sprintf(buffer, "TWCR:%X, TWSR:%X", TWCR, TWSR);
   	LCD_WriteString(buffer);
   	_delay_ms(1000);
//
	//while(1) //infinite loop
	//{
		//PORTD = 0xFF; //Turns ON All LEDs
		//_delay_ms(1000); //1 second delay
		//PORTD= 0x00; //Turns OFF All LEDs
		//_delay_ms(1000); //1 second delay
	//}
}
