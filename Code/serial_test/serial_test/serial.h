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
#define BAUD 9600					//default baud rate
#define MYUBRR F_CPU/16/BAUD-1

extern bool serialInitialized; //cannot name it IMUInitialized, seems to have a conflict with the class...

void initSerial(unsigned int ubrr);
void serialTransmit(char* data);
void serialTransmitINT();
void serialReceive();
char getSerialBuffer();



#endif
