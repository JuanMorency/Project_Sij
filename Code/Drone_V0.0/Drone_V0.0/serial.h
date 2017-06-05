/**
  ******************************************************************************
	* File Name         : serial.h
	* Description       : methods for full duplex serial communication using USB to UART CH340 chip
	* Author			: Juan Morency Trudel
	* Version           : 1.0.0
	* Date				: May 2017
  ******************************************************************************
  */

#ifndef SERIAL_H
#define SERIAL_H
#include <avr/io.h>
#include <stdio.h>
#include <string.h>

#define F_CPU 16000000
#define BAUD 57600					//default baud rate max 57600, min 9600
#define MYUBRR F_CPU/16/BAUD-1

#define RF_SERIAL_SPEED_DIVIDER 30
#define IMU_SERIAL_SPEED_DIVIDER 10

extern bool serialInitialized;
extern char buffer[200];
extern volatile int RFserialSlowDownCounter;
extern volatile int IMUserialSlowDownCounter;


void initSerial(unsigned int ubrr);
void serialTransmit(char* data);
void serialTransmitINT();
void serialReceive();
char getSerialBuffer();



#endif
