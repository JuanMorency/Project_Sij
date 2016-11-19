/**
  ******************************************************************************
	* File Name         : lcd.cpp
	* Description       : Implement methods for controlling the lcd
	* Author			: Auguste Lalande, Felix Dube, Juan Morency Trudel
	* Version           : 1.0.0
	* Date				: February, 2016
  ******************************************************************************
  */

#include "lcd.h"

/* Array to update the values of the pins easier in loops */
const int dataPin[8] =	{DB0, DB1, DB2, DB3, DB4, DB5, DB6, DB7};


/**
	* @brief initializes the GPIOs for the LCD and clean up the display
	* @param None
	* @retval None
	*/
void initLCD(void){
	//init timer for LCD control. Counts up every 1us
	TCCR1B |= (1 << CS00); /*prescaler 1*/
	//set pins as outputs
	DDRK |= (1 << DDK7) | (1 << DDK6) | (1 << DDK5) | (1 << DDK4) | (1 << DDK3) | (1 << DDK2) | (1 << DDK1) | (1 << DDK0);
	DDRL |= (1 << DDL7) | (1 << DDL6) | (1 << DDL5);
	clearDisplay();
	turnOn();
	functionSet();
	entryMode();
	returnHome();
}

/**
	* @brief Toggle the enable pin to "send" command to display
	* @param None
	* @retval None
	*/
void enable(void){
	PORTL |= (1 << E);
	TCNT1L = 0, TCNT1H = 0;
	while ((TCNT1L | TCNT1H << 8) < ENABLE_TIME){/* delay for synchronization */}	
	TCNT1L = 0, TCNT1H = 0;
	PORTL &= ~(1 << E);
}

/**
	* @brief set the data pin to a particular character
  * @param c: char to be written on the LCD
	* @retval None
	*/
void LCD_WriteChar(char c){
	int i;
	for (i = 0; i < 8; i++) {
		/* extracts a bit of the char and checks if it's 1 to set the line to high */
		if ((c & (1 << i)) >> i == 1) {
			PORTK |= (1 << dataPin[i]);
		} else {
			PORTK &= ~(1 << dataPin[i]);
		}
	}
	enable();
}

/**
	* @brief send one by one all the character of the string to LCD_WriteChar()
  * @param string: String to be printed on the LCD
	* @retval None
	*/
void LCD_WriteString(char * string){
	int i;
	inputMode(); /************ NOT sure we need this */
	/* Extract each char individually and calls LCD_WriteChar for each one */
	/* until it hits the MAX_NUMBER_CHAR_LCD_DISPLAY or the end char \0 */
	for (i = 0; (i < MAX_NUMBER_CHAR_LCD_DISPLAY) && (string[i] != '\0') ; i++){
		LCD_WriteChar(string[i]);
	}
}

/**
	* @brief Clear every character on the display and returns to the first address of display
	* @param None
	* @retval None
	*/
void clearDisplay(void){
	PORTL &= ~(1 << RS);
	PORTL &= ~(1 << RW);
	PORTK &= ~(1 << DB7);
	PORTK &= ~(1 << DB6);
	PORTK &= ~(1 << DB5);
	PORTK &= ~(1 << DB4);
	PORTK &= ~(1 << DB3);
	PORTK &= ~(1 << DB2);
	PORTK &= ~(1 << DB1);
	PORTK |= (1 << DB0);
	enable();
	/* extra wait time from the datasheet 1.53ms */
	TCNT1L = 0, TCNT1H = 0;
	while ((TCNT1L | TCNT1H << 8) < WAIT_TIME_AFTER_RESET){/* delay for synchronization */}	
	TCNT1L = 0, TCNT1H = 0;
}

/**
	* @brief set address of the LCD display to address
	* @param address: Address of in the memory of the LCD RAM to read or write next character
	*				 beware of heaxedecimal to int conversion
	* @retval None
	*	@note When 1-line display mode (N=LOW), DDRAM address is form 0x00 to 0x4F
	* 			In 2-line display mode (N=HIGH), DDRAM address is form 0x00 to 0x27 for first line
	*				and 0x40 to 0x67 for second line
	*/
void SetAdress(int address){
  int i;
	PORTL &= ~(1 << RS);
	PORTL &= ~(1 << RW);
	PORTK |= (1 << DB7);
	/* set DB0 to DB6 to the address of the DDRAM */
	for (i = 0; i < 7; i++) {
		if ((address & (1 << i)) >> i == 1) {
			PORTK |= (1 << dataPin[i]);
		} else {
			PORTK &= ~(1 << dataPin[i]);
		}
	}
	enable();
}

/**
	* @brief change the address of the LCD pointer to the first one
	* @param None
	* @retval None
	*/
void returnHome(void){
	PORTL &= ~(1 << RS);
	PORTL &= ~(1 << RW);
	PORTK &= ~(1 << DB7);
	PORTK &= ~(1 << DB6);
	PORTK &= ~(1 << DB5);
	PORTK &= ~(1 << DB4);
	PORTK &= ~(1 << DB3);
	PORTK &= ~(1 << DB2);
	PORTK |= (1 << DB1);
	PORTK &= ~(1 << DB0);
	enable();
	/* extra wait time from the datasheet 1.53ms */
	TCNT1L = 0, TCNT1H = 0;
	while ((TCNT1L | TCNT1H << 8) < WAIT_TIME_AFTER_RESET){/* delay for synchronization */}
	TCNT1L = 0, TCNT1H = 0;
}

/**
	* @brief set up pins to input mode
	* @param None
	* @retval None
	*/
void inputMode(void){
	PORTL |= (1 << RS);
	PORTL &= ~(1 << RW);
}
	
/**
	* @brief Turn on the display
	* @param None
	* @retval None
	*/
void turnOn(void){
	PORTL &= ~(1 << RS);
	PORTL &= ~(1 << RW);
	PORTK &= ~(1 << DB7);
	PORTK &= ~(1 << DB6);
	PORTK &= ~(1 << DB5);
	PORTK &= ~(1 << DB4);
	PORTK |= (1 << DB3);
	PORTK |= (1 << DB2);
	PORTK &= ~(1 << DB1);
	PORTK &= ~(1 << DB0);
	enable();
}

/**
	* @brief Set up length of data bus, and size of display
	* @param None
	* @retval None
	*/
void functionSet(void){
	PORTL &= ~(1 << RS);
	PORTL &= ~(1 << RW);
	PORTK &= ~(1 << DB7);
	PORTK &= ~(1 << DB6);
	PORTK |= (1 << DB5);
	PORTK |= (1 << DB4);
	PORTK |= (1 << DB3);
	PORTK &= ~(1 << DB2);
	PORTK &= ~(1 << DB1);
	PORTK &= ~(1 << DB0);
	enable();
}


/**
	* @brief set up the entry mode of the display
	* @param None
	* @retval None
	*/
void entryMode(void){
	PORTL &= ~(1 << RS);
	PORTL &= ~(1 << RW);
	PORTK &= ~(1 << DB7);
	PORTK &= ~(1 << DB6);
	PORTK &= ~(1 << DB5);
	PORTK &= ~(1 << DB4);
	PORTK &= ~(1 << DB3);
	PORTK |= (1 << DB2);
	PORTK |= (1 << DB1);
	PORTK &= ~(1 << DB0);
	enable();
}
