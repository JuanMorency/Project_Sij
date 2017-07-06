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
char floatbuff[10];
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



/**
 * @brief Transmits the char array
 * @param pointer to the beginning of the data to be transmitted,
 * the function will stop transmitting when it hits a /0 characteristic
 * of an end of string
 */
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
	}
	else
	{
		//indicating serial is being sent too fast and some messages might not be sent
		//not a critical error
		turnDebugLedOn(4);
	}
}

/**
 * @brief This method is called when the interrupt indicating the previous serial data 
 * has been transmitted (USART0_TX_vect) is set and thus the register is free to be loaded 
 * with new data. This method simply loads UDR0 until there is no more data to transmit 
 * 
 */
void serialTransmitINT ()
{
	if (charLeftToTransmit > 0)
	{
		charLeftToTransmit--;
		UDR0 = dataSerial[datalength - charLeftToTransmit]; // load data in the register
	}
}




//TODO work on these receiving functions if needed as it can only be used to receive one char
//right now reads data on receive but does nothing with it

/**
 * @brief This method reads the serial data received in UDR0 register
 * 
 */
void serialReceive ()
{
	//while(!(UCSR0A) & (1<<RXC0));                   // wait while data is being received
	serialBuffer =  UDR0;                                   // return 8-bit data
}

/**
 * @brief This method returns the serial data stored in serialBuffer
 * @retval char with the serial data
 */
char getSerialBuffer()
{
	return serialBuffer;
}


/**
 * @brief method that simply converts a floating point number to a char array
 * since this can't be done with the atmega2560 using sprintf or similar functions
 * @param char* with the text countaining the float converted to string
 * @param float number to be converted to string
 */
void FloatToString(char* text, float number)
{

	const char *tmpSign = (number < 0) ? "-" : "";
	float tmpVal = (number < 0) ? -number : number;

	int tmpInt1 = tmpVal;                  // Get the integer
	float tmpFrac = tmpVal - tmpInt1;      // Get fraction
	int tmpInt2 = trunc(tmpFrac * 10000);  // Turn into integer

	// Print as parts, note that you need 0-padding for fractional bit.

	sprintf (text, "%s%d.%04d", tmpSign, tmpInt1, tmpInt2);	
	
}