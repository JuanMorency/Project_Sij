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
#include "debugLED.h"

bool serialInitialized  = false;
char serialBuffer;
int datalength; 
volatile int charLeftToTransmit = 0;
char* dataSerial;
char buffer[200];
volatile int RFserialSlowDownCounter = 0;
volatile int IMUserialSlowDownCounter = 0;

/**
* @brief initializes the GPIOs for the LCD and clean up the display
* @param baud rate register ubrr
*/
void initSerial(unsigned int ubrr)
{
	/* Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/* Enable receiver and transmitter with interrupts*/
	UCSR0B = (1<<RXCIE0)|(1<<TXCIE0)|(1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
	
	serialInitialized = true;
}



// function to send data
void serialTransmit (char* data)
{
	if (charLeftToTransmit == 0) // to make sure previous data is finished sending
	{
		//count the number of characters in the data to send
		datalength = strlen(data)-1;
		charLeftToTransmit = datalength;
		dataSerial = data;
		///* Wait for empty transmit buffer */
		//while ( !( UCSR0A & (1<<UDRE0)) )
		UDR0 = data[0];                                   // load data in the register
		turnDebugLedOff(4);
	}
	else
	{
		//indicating serial is being sent too fast and some messages might not be sent
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



//work on these receiving functions if needed as it can only be used to receive one char
//right now reads data on receive but does nothing with it

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
