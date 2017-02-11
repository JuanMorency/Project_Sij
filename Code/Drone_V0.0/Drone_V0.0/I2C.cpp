#include "I2C.h"

/**
	* @brief Initialize the prescaler of the I2C clock
	* @param none
	* @retval none
	*/
void initializeI2C()
{
	TWBR = ((F_CPU / 40000) - 16) / 2; // set frequency of sclk to 400 kHz	
	TWCR = (1<<TWEN)|(0<<TWIE)|(0<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO);
}

	/*
	 * Saved TWI status register, for error messages only.  We need to
	 * save it in a variable, since the datasheet only guarantees the TWSR
	 * register to have valid contents while the TWINT bit in TWCR is set.
	 */
uint8_t twst;

uint8_t writeI2C(uint8_t phys_address, uint8_t address, uint8_t data)
{
	uint8_t data1[1];
	data1[0] = data;
	writeI2C(phys_address,address, data1, 1);
}

/**
	* @brief This methods sends bytes of data to another controller via I2C
	* @param phys_address: This is the actual physical address (often fixed on hardware) of the device to communicate with (SLA_W)
	* @param address: Register address on the slave device to write to
	* @param data: pointer to the data that is to be sent to the slave
	* @param length: number of bytes to write
	* @retval uint8_t:  0	success
	*					1	not in start condition
	*					2	error in sending physical address, not expected status register
	*					3	TW_MT_DATA_NACK when sending address
	*					4	error in sending address, not expected status register
	*					5	TW_MT_DATA_NACK when sending data
	*					6	error in sending data, not expected status register
	*					10	MAX_ITER reached timed out
	* @note Refer to page 244 of the Atmega for information
	*/
uint8_t writeI2C(uint8_t phys_address, uint8_t address, uint8_t *data, uint8_t length)
{
	char buffer[20];
	uint8_t twcr, n = 0;
	
	restart:
	if (n++ >= MAX_ITER)
	{
		clearDisplay();
		LCD_WriteString("Salve not responding");
		_delay_ms(2000);
		return 10;	
	}
	begin:
	I2Cstart();
	WaitForTWINT();
	switch ((twst = TW_STATUS))
	{
		case TW_REP_START:		/* OK, but should not happen */
		case TW_START:			/* This is the expected answer */
			break;
		case TW_MT_ARB_LOST:	/*  A master must not initiate a stop condition in order to not corrupt the ongoing transfer from the active master. 
								This will cause a new start condition to be initiated, which will normally be delayed until the currently active master has 
								released the bus. */
			goto begin;
		default:
			clearDisplay();
			LCD_WriteString("Start I2C fail");
			_delay_ms(2000);		
			return 1;		/* error: not in start condition */
			  /* NB: do /not/ send stop condition */
	}
		
	TWDR = phys_address; //Load SLA_W into TWDR Register.
	I2CstartTransmit();
	WaitForTWINT();
	switch ((twst = TW_STATUS))
	{
		case TW_MT_SLA_ACK:
			break;

		case TW_MT_SLA_NACK:	/* nack during select: device busy writing */
			goto restart;

		case TW_MT_ARB_LOST:	/* re-arbitrate */
			goto begin;

		default:
			clearDisplay();
			LCD_WriteString("Invalid phys add");
			_delay_ms(2000);
			ERROR_I2C();		/* must send stop condition */
			I2Cstop();
			return 2;
	}
	//send the address of the register in the IMU chip we want to write to
	TWDR = address; //put register address on the bus
	I2CstartTransmit();
	WaitForTWINT();	
	switch ((twst = TW_STATUS))
	{
		case TW_MT_DATA_ACK:
			break;

		case TW_MT_DATA_NACK:
			I2Cstop();
			return 3;

		case TW_MT_ARB_LOST:
			goto begin;

		default:
			clearDisplay();
			LCD_WriteString("No Data Ack from Slave");
			_delay_ms(2000);
			ERROR_I2C();		/* must send stop condition */
			I2Cstop();
			return 4;
	}
	
	//send data	
	for (int i = 0; i<length;i++)
	{
		TWDR = data[i]; //Load DATA into TWDR Register.
		I2CstartTransmit();
		WaitForTWINT();	
		switch ((twst = TW_STATUS))
		{
			case TW_MT_DATA_ACK:
				break;

			case TW_MT_DATA_NACK:
				I2Cstop();
				return 5;
			case TW_MT_ARB_LOST:
				goto begin;

			default:
				clearDisplay();
				LCD_WriteString("No Data Ack from Slave");
				_delay_ms(2000);
				ERROR_I2C();		/* must send stop condition */
				I2Cstop();
				return 6;
		}
	}
	return 0;
}

uint8_t readI2C(uint8_t phys_address, uint8_t address)
{
	uint8_t data[1];
	if(readI2C(phys_address,address,data, 1) == 0)
	return data[0];
	else
	return 0xFF;
}


/**
	* @brief This methods receives bytes of data from another controller via I2C
	* @param phys_address: This is the actual physical address (often fixed on hardware) of the device to communicate with (SLA_W)
	* @param address: Register address on the slave device to write to
	* @param data: pointer to the data that is to be sent to the slave
	* @param length: number of bytes to read
	* @retval uint8_t:  0	success
	*					1	not in start condition
	*					2	error in sending physical address SLA+W, not expected status register
	*					3	TW_MT_DATA_NACK when sending address
	*					4	error in sending address, not expected status register
	*					5	while Repeated start condition, not expected status register
	*					6	error in sending physical address SLA+R, not expected status register
	*					7	TW_MT_DATA_NACK when sending address
	*					8	TW_MR_DATA_NACK when reading data
	*					9	error in reading data, not expected status register
	*					10	MAX_ITER reached timed out
	* @note Refer to page 244 of the Atmega for information
	*/
uint8_t readI2C(uint8_t phys_address, uint8_t address, uint8_t *data, uint8_t length)
{
	uint8_t twcr, n = 0;
	char buffer[20];
		
	restart:
	if (n++ >= MAX_ITER)
	{
		clearDisplay();
		LCD_WriteString("Slave not responding");
		_delay_ms(2000);
		return 10;	
	}
	
	begin:
	/*Send start condition*/
	I2Cstart();
	WaitForTWINT();
	switch ((twst = TW_STATUS))
	{
		case TW_REP_START:		/* OK, but should not happen */
		case TW_START:
			break;
		case TW_MT_ARB_LOST:	/*  A master must not initiate a stop condition in order to not corrupt the ongoing transfer from the active master. 
								This will cause a new start condition to be initiated, which will normally be delayed until the currently active master has 
								released the bus. */
			goto begin;
		default:
			clearDisplay();
			LCD_WriteString("Start I2C fail");
			_delay_ms(2000);		
			return 1;		/* error: not in start condition */
			  /* NB: do /not/ send stop condition */
	}
	
	
	 /*Load SLA_W into TWDR Register.*/
	TWDR = phys_address;
	I2CstartTransmit();
	WaitForTWINT();
	switch ((twst = TW_STATUS))
	{
		case TW_MT_SLA_ACK:
			break;

		case TW_MT_SLA_NACK:	/* nack during select: device busy writing */
			goto restart;

		case TW_MT_ARB_LOST:	/* re-arbitrate */
			goto begin;

		default:
			clearDisplay();
			LCD_WriteString("Invalid phys add");
			_delay_ms(2000);
			ERROR_I2C();		/* must send stop condition */
			I2Cstop();
			return 2;
	}

	//send the address of the register in the IMU chip we want to read
	TWDR = address; //put register address on the bus
	I2CstartTransmit();
	WaitForTWINT();	
	switch ((twst = TW_STATUS))
	{
		case TW_MT_DATA_ACK:
			break;

		case TW_MT_DATA_NACK:
			I2Cstop();
			return 3;

		case TW_MT_ARB_LOST:
			goto begin;

		default:
			clearDisplay();
			LCD_WriteString("No Data Ack from Slave");
			_delay_ms(2000);
			ERROR_I2C();		/* must send stop condition */
			I2Cstop();
			return 4;
	}
		
	
	
	/* send repeated start condition */	
	I2Cstart(); 
	WaitForTWINT();
	switch ((twst = TW_STATUS))
	{
		case TW_START:		// OK, but should not happen
		case TW_REP_START:	// expected
			break;

		case TW_MT_ARB_LOST:
			goto begin;

		default:
			clearDisplay();
			LCD_WriteString("Rs error");
			_delay_ms(2000);
			ERROR_I2C();		/* must send stop condition */
			I2Cstop();
			return 5;
	}	


	/* send SLA+R */
	TWDR = phys_address | TW_READ; //Load SLA_R into TWDR Register.	
	I2CstartTransmit();
	WaitForTWINT();
	switch ((twst = TW_STATUS))
	{
		case TW_MR_SLA_ACK:
		break;

		case TW_MR_SLA_NACK:
			I2Cstop();
			return 6;

		case TW_MR_ARB_LOST:
			goto begin;

		default:
			clearDisplay();
			LCD_WriteString("MR_SLA_ACK ");
			_delay_ms(2000);
			ERROR_I2C();		/* must send stop condition */
			I2Cstop();
			return 7;
	}
	

	for (int i = 0; i < length; i++)    /*********************To check***********************/
	{
		if (i == length-1)
		{
			TWCR = (1<<TWINT)|(1<<TWEN);
		}
		else
		{
			TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
		}
		WaitForTWINT();
		switch ((twst = TW_STATUS))
		{
			case TW_MR_DATA_NACK:
				if (i == length-1) //good last bit received,  stop I2C
				{
					data[i] = TWDR;
					I2Cstop();
					return 0;
				}
				else
				{
					clearDisplay();
					LCD_WriteString("TW_MR_DATA_NACK ");
					_delay_ms(2000);
					ERROR_I2C();		/* must send stop condition */
					I2Cstop();
					return 8;					
				}
			case TW_MR_DATA_ACK:
				data[i] = TWDR;
				break;
			default:
				clearDisplay();
				LCD_WriteString("MR_SLA_ACK ");
				_delay_ms(2000);
				ERROR_I2C();		/* must send stop condition */
				I2Cstop();
				return 9;
		}
	}

	
}

/**
	* @brief Sets the register to send the start condition
	* @param none
	* @retval none
	*/
void I2Cstart()
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); //Send START condition
}

/**
	* @brief Sets the register to send the stop condition
	* @param none
	* @retval none
	*/
void I2Cstop()
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO); //Transmit STOP condition
	TWCR = 0;
}

//
///**
	//* @brief waits for a status flag returns 0 if it's the right one, 1 if not 
	//* @param status: Flag to expect
	//* @retval 0 good flag, 1 wrong flag, -1 not in start condition
	//*/
////TODO simplify this
//uint8_t WaitAndCheckFor(uint8_t status)
//{
	////char buffer[20];
	////clearDisplay();
	////sprintf(buffer, "%X", (TWSR & 0xF8));
	////LCD_WriteString(buffer);
	////_delay_ms(1000);
	//
	//while (!(TWCR & (1<<TWINT))); // Wait for TWINT Flag set. This indicates that the START condition has been transmitted
	//twst = TW_STATUS;
	//
	////weird should not be here To check
	//if (status == TW_START) //Check value of TWI Status Register. Mask prescaler bits. If status different from START go to ERROR
	//{
		//switch ((twst = TW_STATUS))
		//{
			  //case TW_REP_START:		/* OK, but should not happen */
			  //case TW_START:
				//break;
//
			  //case TW_MT_ARB_LOST:	/* Note [9] */
			  //return 1;
//
			  //default:
			  //return -1;		/* error: not in start condition */
			  ///* NB: do /not/ send stop condition */
		//}
	//}
	//if ((TWSR & 0xF8) != status) //Check value of TWI Status Register. Mask prescaler bits. If TWSR different from status go to ERROR
	//{
		//ERROR_I2C();
		//return 1;	
	//}
	//else
	//{
		//return 0;
	//}
//}



/**
	* @brief wait until TWINT becomes 1 again
	* @param none
	* @retval none
	*/
//might be dangerous, we could get stuck here...
void WaitForTWINT()
{
	while (!(TWCR & (1<<TWINT)))// Wait for TWINT Flag set.
	{
	}

}
/**
	* @brief sets TWCR register to start trnamission over I2C
	* @param none
	* @retval none
	*/
void I2CstartTransmit()
{
	TWCR = (1<<TWINT) | (1<<TWEN); //Clear TWINT bit in TWCR to start transmission of data/address
}
/**
	* @brief sets the TWINT bit to 0
	* @param none
	* @retval none
	*/
void resetTWINT()
{
	TWCR &= ~(1<< TWINT);
}
/**
	* @brief handles error. Just writes a message to the LCD for now
	* @param none
	* @retval none
	*/
void ERROR_I2C()
{
	//char buffer[20];
	//clearDisplay();
	//sprintf(buffer, "%X", TWSR);
	//LCD_WriteString(buffer);
	//_delay_ms(1000);
	//SetAdress(64); // goes to line 2 of LCD
	//sprintf(buffer, "%X", TWDR);
	//LCD_WriteString(buffer);
	//_delay_ms(5000);
	
	clearDisplay();
	LCD_WriteString("I2C Error");
	_delay_ms(2000);
	I2Cstop();
}

