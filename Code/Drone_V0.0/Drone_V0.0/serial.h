/**
  ******************************************************************************
	* File Name         : serial.h
	* Description       : methods for full duplex serial communication using USB to UART CH340 chip
	* Author			: Juan Morency Trudel
	* Version           : 1.0.0
	* Date				: May 2017
  ******************************************************************************
  */


// to use Serial, plug the CH340 board by USB on your computer and simply plug:
// Tx of the Serial board in E0
// Rx of Serial board in E1


#ifndef SERIAL_H
#define SERIAL_H

#ifndef F_CPU
#define F_CPU 16000000
#endif

#include <avr/io.h>
#include <stdio.h>
#include <string.h>

#define BAUD 57600					//default baud rate max 57600, min 9600
#define MYUBRR F_CPU/16/BAUD-1

#define RF_SERIAL_SPEED_DIVIDER 50
#define IMU_SERIAL_SPEED_DIVIDER 500
#define ESC_SERIAL_SPEED_DIVIDER 20

extern bool serialInitialized;
extern char buffer[200];
extern char floatbuff[10];
extern volatile int RFserialSlowDownCounter;
extern volatile int IMUserialSlowDownCounter;
extern volatile int EscSerialSlowDownCounter;


void initSerial(unsigned int ubrr);
void serialTransmit(char* data);
void serialTransmitINT();
void serialReceive();
char getSerialBuffer();
void FloatToString(char* text, float number);


#endif
