/**
******************************************************************************
* File Name         : I2C.h
* Description       : header file for I2C.cpp
* Author			: Juan Morency Trudel
* Version           : 1.0.0
* Date				: June 2017
******************************************************************************
*/

#ifndef I2C_H
#define I2C_H

#ifndef F_CPU
#define F_CPU 16000000
#endif

#define MAX_ITER 200
#define SCL_FREQUENCY 400000
#define SCL_FREQUENCY_TIMES_8 SCL_FREQUENCY*8

#include <stdio.h>
#include <util/delay.h>
#include <util/twi.h>
#include "lcd.h"
#include "serial.h"

void initializeI2C();
void startI2CForFsm();
uint8_t writeI2C(uint8_t phys_address, uint8_t address, uint8_t *data, uint8_t length);
uint8_t writeI2C(uint8_t phys_address, uint8_t address, uint8_t data);
uint8_t readI2C(uint8_t phys_address, uint8_t address, uint8_t *data, uint8_t length);
uint8_t readI2C(uint8_t phys_address, uint8_t address);
uint8_t handleFsmI2c();

enum
{
	CHECK_TIMEOUT_AND_START = 0,
	CHECK_START_AND_LOAD_ADDRESS,
	CHECK_ACK_AND_LOAD_DATA_ADDRESS,
	CHECK_DATA_ACK_AND_SEND_REPEATED_START,
	CHECK_REPEATED_START_AND_START_READ,
	CHECK_ACK_AND_SEND_READ_BYTE_REQUEST,
	READ_BYTE_AND_SEND_MORE_READ_BYTE_REQUEST,
};

extern uint8_t dataReadBuffer[14];
extern uint8_t phys_adress;
extern uint8_t data_adress;
extern uint8_t data_length;

extern bool TwiInterruptInitialized;

void I2Cstart();
void I2Cstop();
uint8_t WaitAndCheckFor(uint8_t status);
void WaitForTWINT();
void I2CstartTransmit();
void AcknowledgeReceive();
void NotAcknowledgeReceive();
void resetTWINT();
void ERROR_I2C();
#endif