/**
  ******************************************************************************
	* File Name			: lcd.h
	* Description		: Header for lcd.c
	* Author			: Auguste Lalande, Felix Dube, Juan Morency Trudel
	* Version			: 1.0.0
	* Date				: last modified September 4 2016
  ******************************************************************************
  */

#ifndef LCD_H
#define LCD_H

#include <avr/io.h>

#define RS 	PORTL5
#define RW 	PORTL6
#define E 	PORTL7
#define DB0 PORTK0
#define DB1 PORTK1
#define DB2 PORTK2
#define DB3 PORTK3
#define DB4 PORTK4
#define DB5 PORTK5
#define DB6 PORTK6
#define DB7 PORTK7

#define MAX_NUMBER_CHAR_LCD_DISPLAY 16
#define ENABLE_TIME 0x190 						/* 400 us, don't set below ~50 or weird timing issue with display */
#define WAIT_TIME_AFTER_RESET 0x7D0 	/* 2000 us, don't set below ~1000 us*/

void initLCD(void);
void enable(void);
void LCD_WriteChar(char c);
void LCD_WriteString(char * string);
void clearDisplay(void);
void returnHome(void);
void inputMode(void);
void turnOn(void);
void functionSet(void);
void entryMode(void);
void SetAdress(int address);

#endif
