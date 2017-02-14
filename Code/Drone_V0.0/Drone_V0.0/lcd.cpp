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

char g_line1[20];
char g_line2[20];

bool lcdInitialized = false;


/**
	* @brief initializes the GPIOs for the LCD and clean up the display
	* @param None
	* @retval None
	*/
void initLCD(void){

	TCCR1B |= (1 << CS11); /*prescaler 8*/   //debug
	//set pins as outputs
	DDRA |= (1 << DDA7) | (1 << DDA6) | (1 << DDA5) | (1 << DDA4) | (1 << DDA3) | (1 << DDA2) | (1 << DDA1) | (1 << DDA0);
	DDRG |= (1 << DDG2) | (1 << DDG1) | (1 << DDG0);

	clearDisplay();
	TCNT1L = 0, TCNT1H = 0;
	while ((TCNT1L | TCNT1H << 8) < ENABLE_TIME){/* delay for synchronization */}
	TCNT1L = 0, TCNT1H = 0;
	PORTG &= ~(1 << E);
	TCNT1L = 0, TCNT1H = 0;
	while ((TCNT1L | TCNT1H << 8) < WAIT_TIME_AFTER_RESET){/* delay for synchronization */}
	TCNT1L = 0, TCNT1H = 0;

	turnOn();
	TCNT1L = 0, TCNT1H = 0;
	while ((TCNT1L | TCNT1H << 8) < ENABLE_TIME){/* delay for synchronization */}
	TCNT1L = 0, TCNT1H = 0;
	PORTG &= ~(1 << E);

	functionSet();
	TCNT1L = 0, TCNT1H = 0;
	while ((TCNT1L | TCNT1H << 8) < ENABLE_TIME){/* delay for synchronization */}
	TCNT1L = 0, TCNT1H = 0;
	PORTG &= ~(1 << E);
	
	lcdInitialized = true;
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
			PORTA |= (1 << dataPin[i]);
		} else {
			PORTA &= ~(1 << dataPin[i]);
		}
	}
	PORTG |= (1 << E);
}

/**
	* @brief Clear every character on the display and returns to the first address of display
	* @param None
	* @retval None
	*/
void clearDisplay(void){
	PORTG &= ~(1 << RS);
	PORTG &= ~(1 << RW);
	PORTA &= ~(1 << DB7);
	PORTA &= ~(1 << DB6);
	PORTA &= ~(1 << DB5);
	PORTA &= ~(1 << DB4);
	PORTA &= ~(1 << DB3);
	PORTA &= ~(1 << DB2);
	PORTA &= ~(1 << DB1);
	PORTA |= (1 << DB0);
	PORTG |= (1 << E);
}

/**
	* @brief set address of the LCD display to address
	* @param address: Address of in the memory of the LCD RAM to read or write next character
	*				 beware of heaxedecimal to int conversion
	* @retval None
	* @note When 1-line display mode (N=LOW), DDRAM address is form 0x00 to 0x4F
	* 			In 2-line display mode (N=HIGH), DDRAM address is form 0x00 to 0x27 for first line
	*				and 0x40 to 0x67 for second line
	*/
void SetAdress(int address){
    int i;
	PORTG &= ~(1 << RS);
	PORTG &= ~(1 << RW);
	PORTA |= (1 << DB7);
	/* set DB0 to DB6 to the address of the DDRAM */
	for (i = 0; i < 7; i++) {
		if ((address & (1 << i)) >> i == 1) {
			PORTA |= (1 << dataPin[i]);
		} else {
			PORTA &= ~(1 << dataPin[i]);
		}
	}
	PORTG |= (1 << E);
}

/**
	* @brief change the address of the LCD pointer to the first one
	* @param None
	* @retval None
	*/
void returnHome(void){
	PORTG &= ~(1 << RS);
	PORTG &= ~(1 << RW);
	PORTA &= ~(1 << DB7);
	PORTA &= ~(1 << DB6);
	PORTA &= ~(1 << DB5);
	PORTA &= ~(1 << DB4);
	PORTA &= ~(1 << DB3);
	PORTA &= ~(1 << DB2);
	PORTA |= (1 << DB1);
	PORTA &= ~(1 << DB0);
	PORTG |= (1 << E);
}

/**
	* @brief set up pins to input mode
	* @param None
	* @retval None
	*/
void inputMode(void){
	PORTG |= (1 << RS);
	PORTG &= ~(1 << RW);
}
	
/**
	* @brief Turn on the display
	* @param None
	* @retval None
	*/
void turnOn(void){
	PORTG &= ~(1 << RS);
	PORTG &= ~(1 << RW);
	PORTA &= ~(1 << DB7);
	PORTA &= ~(1 << DB6);
	PORTA &= ~(1 << DB5);
	PORTA &= ~(1 << DB4);
	PORTA |= (1 << DB3);
	PORTA |= (1 << DB2);
	PORTA &= ~(1 << DB1);
	PORTA &= ~(1 << DB0);
	PORTG |= (1 << E);
}

/**
	* @brief Set up length of data bus, and size of display
	* @param None
	* @retval None
	*/
void functionSet(void){
	PORTG &= ~(1 << RS);
	PORTG &= ~(1 << RW);
	PORTA &= ~(1 << DB7);
	PORTA &= ~(1 << DB6);
	PORTA |= (1 << DB5);
	PORTA |= (1 << DB4);
	PORTA |= (1 << DB3);
	PORTA &= ~(1 << DB2);
	PORTA &= ~(1 << DB1);
	PORTA &= ~(1 << DB0);
	PORTG |= (1 << E);
}


/**
	* @brief set up the entry mode of the display
	* @param None
	* @retval None
	*/
void entryMode(void){
	PORTG &= ~(1 << RS);
	PORTG &= ~(1 << RW);
	PORTA &= ~(1 << DB7);
	PORTA &= ~(1 << DB6);
	PORTA &= ~(1 << DB5);
	PORTA &= ~(1 << DB4);
	PORTA &= ~(1 << DB3);
	PORTA |= (1 << DB2);
	PORTA |= (1 << DB1);
	PORTA &= ~(1 << DB0);
	PORTG |= (1 << E);
}

// TODO make method easier to use : calculate the length automatically
void changeLCDText(char* line1, char* line2)
{
	int i = 0;
	for(; i < 16; i++)
	{
		g_line1[i] = line1[i];
		g_line2[i] = line2[i];
	}
	i++;
	g_line1[i] = '\0';
	g_line2[i] = '\0';
}

// TODO make method easier to use : calculate the length automatically
void changeLCDText(char* line1)
{
	int i = 0;
	for(; i < 16; i++)
	{
		g_line1[i] = line1[i];
		g_line2[i] = ' ';
	}
	i++;
	g_line1[i] = '\0';
}



//Finite state machine for the interrupt service routine
void handleFSMLCD(void){

	//initialize variables once with static
	static uint8_t LCDState = 0;
	static uint8_t enableOnOff = 1;
	static uint8_t counterShortLCD = 0;
	static uint16_t counterLongLCD = 0;
	static uint8_t charPosition = 0;
	static char currentChar;

	switch(LCDState){
		case CLEAR_STATE:
		clearDisplay();
		//Set enable bit for 1 cycle of 50 us
		if(enableOnOff){
			enableOnOff = 0;
		}
		else
		{
			enableOnOff = 1;
			PORTG &= ~(1 << E);
			counterShortLCD = 0;
			LCDState = WAIT_1_5_MS_STATE;
		}
		break;

		// 30 * 50us = 1.5ms
		case WAIT_1_5_MS_STATE:
		if (counterShortLCD >= 30){
			LCDState = LINE1_STATE;
			charPosition = 0;
			inputMode();
		}
		counterShortLCD++;
		break;
		case LINE1_STATE:
		currentChar = g_line1[charPosition];
		if((charPosition < MAX_NUMBER_CHAR_LCD_DISPLAY) && (currentChar != '\0'))
		{
			if(enableOnOff){
				LCD_WriteChar(currentChar);
				enableOnOff = 0;
			}
			else
			{
				PORTG &= ~(1 << E);
				enableOnOff = 1;
				charPosition++;
			}
		}
		else
		{
			LCDState = CHANGE_ADDRESS_STATE;
		}
		break;

		case CHANGE_ADDRESS_STATE:
		if(enableOnOff){
			SetAdress(LINE2);
			enableOnOff = 0;
		}
		else
		{
			PORTG &= ~(1 << E);
			enableOnOff = 1;
			charPosition = 0;
			LCDState = LINE2_STATE;
			inputMode();
		}
		break;

		case LINE2_STATE:
		currentChar = g_line2[charPosition];
		if((charPosition < MAX_NUMBER_CHAR_LCD_DISPLAY) && (currentChar != '\0'))
		{
			if(enableOnOff){
				LCD_WriteChar(currentChar);
				enableOnOff = 0;
			}
			else
			{
				PORTG &= ~(1 << E);
				enableOnOff = 1;
				charPosition++;
			}
		}
		else
		{
			charPosition = 0;
			counterLongLCD = 0;
			LCDState = WAIT_STATE;
		}
		break;

		// 100 cycles of interrupts are required to fully update the LCD
		// After these 100 cycles, the state machine stays in this state for a certain
		// time depending on the LCD_REFRESH_RATE parameter set in lcd.h
		// 1/LCD_REFRESH_RATE = (100 + number of cycles) * time_between_LCD_interrupts
		//
		case WAIT_STATE:
		if(counterLongLCD >= 2000) // approximately 10 Hz refresh rate
		{
			LCDState = CLEAR_STATE;
		}
		else
		{
			counterLongLCD++;
		}
		break;
		default:
		LCDState = WAIT_STATE;
		counterShortLCD = 0;
		counterLongLCD = 0;
		enableOnOff = 1;
	}
}



// OLD methods






///**
	//* @brief initializes the GPIOs for the LCD and clean up the display
	//* @param None
	//* @retval None
	//*/
//void initLCD(void){
	////init timer for LCD control. Counts up every 0.5 us (1/ (16M/8))
	//TCCR1B |= (1 << CS11); /*prescaler 8*/
	////set pins as outputs
	//DDRA |= (1 << DDA7) | (1 << DDA6) | (1 << DDA5) | (1 << DDA4) | (1 << DDA3) | (1 << DDA2) | (1 << DDA1) | (1 << DDA0);
	//DDRG |= (1 << DDG2) | (1 << DDG1) | (1 << DDG0);
	//clearDisplay();
	//turnOn();
	//functionSet();
	////entryMode();
	////returnHome();
//}
//
///**
	//* @brief Toggle the enable pin to "send" command to display
	//* @param None
	//* @retval None
	//*/
//void enable(void){
	//PORTG |= (1 << E);
	//TCNT1L = 0, TCNT1H = 0;
	//while ((TCNT1L | TCNT1H << 8) < ENABLE_TIME){/* delay for synchronization */}	
	//TCNT1L = 0, TCNT1H = 0;
	//PORTG &= ~(1 << E);
//}
//
///**
	//* @brief set the data pin to a particular character
	//* @param c: char to be written on the LCD
	//* @retval None
	//*/
//void LCD_WriteChar(char c){
	//int i;
	//for (i = 0; i < 8; i++) {
		///* extracts a bit of the char and checks if it's 1 to set the line to high */
		//if ((c & (1 << i)) >> i == 1) {
			//PORTA |= (1 << dataPin[i]);
		//} else {
			//PORTA &= ~(1 << dataPin[i]);
		//}
	//}
	//enable();
//}
//
///**
	//* @brief send one by one all the character of the string to LCD_WriteChar()
    //* @param string: String to be printed on the LCD
	//* @retval None
	//*/
//void LCD_WriteString(char * string){
	//int i;
	//inputMode();
	///* Extract each char individually and calls LCD_WriteChar for each one */
	///* until it hits the MAX_NUMBER_CHAR_LCD_DISPLAY or the end char \0 */
	//for (i = 0; (i < MAX_NUMBER_CHAR_LCD_DISPLAY) && (string[i] != '\0') ; i++){
		//LCD_WriteChar(string[i]);
	//}
//}
//
///**
	//* @brief Clear every character on the display and returns to the first address of display
	//* @param None
	//* @retval None
	//*/
//void clearDisplay(void){
	//PORTG &= ~(1 << RS);
	//PORTG &= ~(1 << RW);
	//PORTA &= ~(1 << DB7);
	//PORTA &= ~(1 << DB6);
	//PORTA &= ~(1 << DB5);
	//PORTA &= ~(1 << DB4);
	//PORTA &= ~(1 << DB3);
	//PORTA &= ~(1 << DB2);
	//PORTA &= ~(1 << DB1);
	//PORTA |= (1 << DB0);
	//enable();
	///* extra wait time from the datasheet 1.53ms */
	//TCNT1L = 0, TCNT1H = 0;
	//while ((TCNT1L | TCNT1H << 8) < WAIT_TIME_AFTER_RESET){/* delay for synchronization */}	
	//TCNT1L = 0, TCNT1H = 0;
//}
//
///**
	//* @brief set address of the LCD display to address
	//* @param address: Address of in the memory of the LCD RAM to read or write next character
	//*				 beware of heaxedecimal to int conversion
	//* @retval None
	//* @note When 1-line display mode (N=LOW), DDRAM address is form 0x00 to 0x4F
	//* 			In 2-line display mode (N=HIGH), DDRAM address is form 0x00 to 0x27 for first line
	//*				and 0x40 to 0x67 for second line
	//*/
//void SetAdress(int address){
  //int i;
	//PORTG &= ~(1 << RS);
	//PORTG &= ~(1 << RW);
	//PORTA |= (1 << DB7);
	///* set DB0 to DB6 to the address of the DDRAM */
	//for (i = 0; i < 7; i++) {
		//if ((address & (1 << i)) >> i == 1) {
			//PORTA |= (1 << dataPin[i]);
		//} else {
			//PORTA &= ~(1 << dataPin[i]);
		//}
	//}
	//enable();
//}
//
///**
	//* @brief change the address of the LCD pointer to the first one
	//* @param None
	//* @retval None
	//*/
//void returnHome(void){
	//PORTG &= ~(1 << RS);
	//PORTG &= ~(1 << RW);
	//PORTA &= ~(1 << DB7);
	//PORTA &= ~(1 << DB6);
	//PORTA &= ~(1 << DB5);
	//PORTA &= ~(1 << DB4);
	//PORTA &= ~(1 << DB3);
	//PORTA &= ~(1 << DB2);
	//PORTA |= (1 << DB1);
	//PORTA &= ~(1 << DB0);
	//enable();
	///* extra wait time from the datasheet 1.53ms */
	//TCNT1L = 0, TCNT1H = 0;
	//while ((TCNT1L | TCNT1H << 8) < WAIT_TIME_AFTER_RESET){/* delay for synchronization */}
	//TCNT1L = 0, TCNT1H = 0;
//}
//
///**
	//* @brief set up pins to input mode
	//* @param None
	//* @retval None
	//*/
//void inputMode(void){
	//PORTG |= (1 << RS);
	//PORTG &= ~(1 << RW);
//}
	//
///**
	//* @brief Turn on the display
	//* @param None
	//* @retval None
	//*/
//void turnOn(void){
	//PORTG &= ~(1 << RS);
	//PORTG &= ~(1 << RW);
	//PORTA &= ~(1 << DB7);
	//PORTA &= ~(1 << DB6);
	//PORTA &= ~(1 << DB5);
	//PORTA &= ~(1 << DB4);
	//PORTA |= (1 << DB3);
	//PORTA |= (1 << DB2);
	//PORTA &= ~(1 << DB1);
	//PORTA &= ~(1 << DB0);
	//enable();
//}
//
///**
	//* @brief Set up length of data bus, and size of display
	//* @param None
	//* @retval None
	//*/
//void functionSet(void){
	//PORTG &= ~(1 << RS);
	//PORTG &= ~(1 << RW);
	//PORTA &= ~(1 << DB7);
	//PORTA &= ~(1 << DB6);
	//PORTA |= (1 << DB5);
	//PORTA |= (1 << DB4);
	//PORTA |= (1 << DB3);
	//PORTA &= ~(1 << DB2);
	//PORTA &= ~(1 << DB1);
	//PORTA &= ~(1 << DB0);
	//enable();
//}
//
//
///**
	//* @brief set up the entry mode of the display
	//* @param None
	//* @retval None
	//*/
//void entryMode(void){
	//PORTG &= ~(1 << RS);
	//PORTG &= ~(1 << RW);
	//PORTA &= ~(1 << DB7);
	//PORTA &= ~(1 << DB6);
	//PORTA &= ~(1 << DB5);
	//PORTA &= ~(1 << DB4);
	//PORTA &= ~(1 << DB3);
	//PORTA |= (1 << DB2);
	//PORTA |= (1 << DB1);
	//PORTA &= ~(1 << DB0);
	//enable();
//}



