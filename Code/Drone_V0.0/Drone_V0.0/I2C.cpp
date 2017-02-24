#include "I2C.h"

int j;

/**
	* @brief Initialize the prescaler of the I2C clock
	* @param none
	* @retval none
	*/
void initializeI2C()
{
	TWBR = ((F_CPU / 40000) - 16) / 2; // set frequency of sclk to 400 kHz	
	TWCR = (1<<TWEN)|(0<<TWIE)|(0<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO);
	DDRC = 0xFF;
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
		changeLCDText("Slave not responding");
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
			changeLCDText("Start I2C fail");
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
			changeLCDText("Invalid phys add");
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
			changeLCDText("No Data Ack from Slave");
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
				changeLCDText("No Data Ack from Slave");
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
		changeLCDText("Slave not responding");
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
			changeLCDText("Start I2C fail");	
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
			changeLCDText("Invalid phys add");
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
			changeLCDText("No Data Ack from Slave");
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
			changeLCDText("Rs error");
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
			changeLCDText("MR_SLA_ACK ");
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
			case TW_MR_DATA_ACK:
				data[i] = TWDR;
				break;
			case TW_MR_DATA_NACK:
				data[i] = TWDR;
				I2Cstop();
				break;
			default:
				changeLCDText("MR_SLA_ACK ");
				I2Cstop();
				return 9;
		}
	}
	return 0;

}

/**
	* @brief Sets the register to send the start condition
	* @param none
	* @retval none
	*/
void I2Cstart()
{
	TWCR = 0;
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
}

/**
	* @brief wait until TWINT becomes 1 again
	* @param none
	* @retval none
	*/
//might be dangerous, we could get stuck here...
// just implement a timeout
void WaitForTWINT()
{
	while (!(TWCR & (1<<TWINT)))// Wait for TWINT Flag set.
	{
	}

}
/**
	* @brief sets TWCR register to start transmission over I2C
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

