/*
 * I2C_test.cpp
 *
 * Created: 12/29/2016 11:36:55
 * Author : juan-
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>


#include "lcd.h"



int main(void)
{
	initLCD();
	clearDisplay();
	DDRD = 0xFF;
	PORTD = 0XFF;
	_delay_ms(3000);
    char buffer[20];
	/* Replace with your application code */
	clearDisplay();
	LCD_WriteString("---1---");
	SetAdress(LINE2);
	sprintf(buffer, "TWCR:%X, TWSR:%X", TWCR, TWSR);
	LCD_WriteString(buffer);
	_delay_ms(3000);
	//I2C init
	TWBR = ((F_CPU / 40000) - 16) / 2; // set frequency of sclk to 400 kHz
	//TWSR |= (1<<TWPS0);
	TWCR = (1<<TWEN)|(0<<TWIE)|(0<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO);


	
	//I2C send start condition
	clearDisplay();
	LCD_WriteString("---2---");
	SetAdress(LINE2);
	sprintf(buffer, "TWCR:%X, TWSR:%X", TWCR, TWSR);
	LCD_WriteString(buffer);
	_delay_ms(3000);
		
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); //Send START condition
	
	while (!(TWCR & (1<<TWINT)))// Wait for TWINT Flag set.
	{
	clearDisplay();
	LCD_WriteString("---3---");
	SetAdress(LINE2);
	sprintf(buffer, "TWCR:%X, TWSR:%X", TWCR, TWSR);
	LCD_WriteString(buffer);
	_delay_ms(3000);		
	}
	
	clearDisplay();
	LCD_WriteString("---4---");
	SetAdress(LINE2);
	sprintf(buffer, "TWCR:%X, TWSR:%X", TWCR, TWSR);
	LCD_WriteString(buffer);
	_delay_ms(3000);
		
	TWDR = 0xD0; //Load SLA_W into TWDR Register.

	
}

