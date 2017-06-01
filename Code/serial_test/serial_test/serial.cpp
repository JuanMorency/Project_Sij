/**
******************************************************************************
* File Name         : serial.cpp
* Description       : methods for full duplex serial communication using USB to UART CH340 chip
* Author			: Juan Morency Trudel
* Version           : 1.0.0
* Date				: May 2017
******************************************************************************
*/

#include "serial.h"
#include "lcd.h"
#include "debugLED.h"

bool serialInitialized  = false;
char serialBuffer;
int datalength; 
int charLeftToTransmit;
char* dataSerial;

/**
* @brief initializes the GPIOs for the LCD and clean up the display
* @param baud rate register ubrr
*/
void initSerial(unsigned int ubrr)
{
	/* Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXCIE0)|(1<<TXCIE0)|(1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
	
	serialInitialized = true;
} // USART_Init


// function to send data
void serialTransmit (char* data)
{
	if (charLeftToTransmit == 0)
	{
		//count the number of characters in the data to send
		datalength = strlen(data);
		charLeftToTransmit = datalength;
		dataSerial = data;
		///* Wait for empty transmit buffer */
		//while ( !( UCSRnA & (1<<UDREn)) )
		UDR0 = data[0];                                   // load data in the register
	}
	else
	{
		turnDebugLedOn(4);
	}
}

// function to send data
void serialTransmitINT ()
{
	if (charLeftToTransmit > 0)
	{
		charLeftToTransmit--;
		UDR0 = dataSerial[datalength - charLeftToTransmit]; // load data in the register
	}
}

// function to receive data
void serialReceive ()
{
	//while(!(UCSR0A) & (1<<RXC0));                   // wait while data is being received
	serialBuffer =  UDR0;                                   // return 8-bit data
}

char getSerialBuffer()
{
	return serialBuffer;
}
