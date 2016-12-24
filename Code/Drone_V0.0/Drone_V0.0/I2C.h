#ifndef I2C_H
#define I2C_H

#define F_CPU 16000000
#define IMU_ADDRESS 0xD0 //Address of IMU + 0 for write SLA_W
#define BMP180_ADDRESS 0xEE
#define MAX_ITER 200

#include "lcd.h"
#include <stdio.h>
#include <util/delay.h>
#include <util/twi.h>

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