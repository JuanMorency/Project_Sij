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

#include <stdio.h>
#include <util/delay.h>
#include <util/twi.h>
#include "lcd.h"
#include "serial.h"

void initializeI2C();
uint8_t writeI2C(uint8_t phys_address, uint8_t address, uint8_t *data, uint8_t length);
uint8_t writeI2C(uint8_t phys_address, uint8_t address, uint8_t data);
uint8_t readI2C(uint8_t phys_address, uint8_t address, uint8_t *data, uint8_t length);
uint8_t readI2C(uint8_t phys_address, uint8_t address);

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