/**
******************************************************************************
* File Name         : I2C.h
* Description       : methods for I2C communication
* Author			: Juan Morency Trudel
* Version           : 1.0.0
* Date				: June 2017
******************************************************************************
*/

#include "I2C.h"
#include "debugLED.h"



//Saved TWI status register, for error messages only.  We need to
//save it in a variable, since the datasheet only guarantees the TWSR
//register to have valid contents while the TWINT bit in TWCR is set.
uint8_t twst;

int j;
uint8_t dataReadBuffer[14];
uint8_t phys_adress;
uint8_t data_adress;
uint8_t data_length;

bool TwiInterruptInitialized  = false;


/**
	* @brief Initialize the prescaler of the I2C clock
	*/
void initializeI2C()
{
	TWBR = (uint8_t)(F_CPU/SCL_FREQUENCY_TIMES_8)-2; // set frequency of sclk to SCL_FREQ
	TWCR = (1<<TWEN)|(0<<TWIE)|(0<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO);
}

/**
	* @brief Enables interrupts for I2C
	*/
void startI2CForFsm()
{
	TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(0<<TWEA)|(1<<TWSTA)|(0<<TWSTO);
	TwiInterruptInitialized  = true;
}

/**
	* @brief Disables interrupts for I2C
	*/
void I2cDisableInterruptForFsm()
{
	TwiInterruptInitialized  = false;
}


/**
	* @brief This methods sends one char via I2C
	* @param phys_address: This is the actual physical address (often fixed on hardware) of the device to communicate with (SLA_W)
	* @param address: Register address on the slave device to write to
	* @param data: char that is to be sent to the slave
	* @retval uint8_t:  0	success
	*					1	not in start condition
	*					2	error in sending physical address, not expected status register
	*					3	TW_MT_DATA_NACK when sending address
	*					4	error in sending address, not expected status register
	*					5	TW_MT_DATA_NACK when sending data
	*					6	error in sending data, not expected status register
	*					10	MAX_ITER reached timed out
	* @note Refer to page 244 of the Atmega for more information
	*/
uint8_t writeI2C(uint8_t phys_address, uint8_t address, uint8_t data)
{
	uint8_t data1[1];
	data1[0] = data;
	return writeI2C(phys_address,address, data1, 1);
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
	uint8_t n = 0;
	
	restart:
	if (n++ >= MAX_ITER)
	{
		serialTransmit("Slave not responding");
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
			serialTransmit("Start I2C fail \n");
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
			serialTransmit("Invalid phys add");
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
			serialTransmit("No Data Ack from Slave");
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
				serialTransmit("No Data Ack from Slave");
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
	uint8_t n = 0;
		
	restart:
	if (n++ >= MAX_ITER)
	{
		serialTransmit("Slave not responding");
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
			serialTransmit("Start I2C fail \n");	
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
			serialTransmit("Invalid phys add");
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
			serialTransmit("No Data Ack from Slave");
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
			serialTransmit("Rs error");
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
			serialTransmit("MR_SLA_ACK ");
			I2Cstop();
			return 7;
	}
	
	
	for (int i = 0; i < length; i++)
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
				serialTransmit("MR_SLA_ACK ");
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
	TWCR = 0;
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


/**
	* @brief Finite State Machine implementation of the I2C read. The physical address, data address and data length have to 
	* be stored in their global variables, and the output data is stored in dataReadBuffer. This method is to be called in the
	* TWI_vect ISR. It should be called after a start condition has been 
	* @param phys_address: This is the actual physical address (often fixed on hardware) of the device to communicate with (SLA_W)
	* @param address: Register address on the slave device to write to
	* @param data: pointer to the data that is to be sent to the slave
	* @param length: number of bytes to read
	* @retval uint8_t:  0	finished reading bytes
	*					1	not in start condition
	*					2	error in sending physical address SLA+W, not expected status register
	*					3	TW_MT_DATA_NACK when sending address
	*					4	error in sending address, not expected status register
	*					5	while Repeated start condition, not expected status register
	*					6	error in sending physical address SLA+R, not expected status register
	*					7	TW_MT_DATA_NACK when sending address
	*					8	error in reading data, not expected status register
	*					9	finished loop without error
	*					10	MAX_ITER reached timed out
	* @note Refer to page 244 of the Atmega for information
	*/
uint8_t handleFsmI2c()
{
	
	static uint8_t State = CHECK_START_AND_LOAD_ADDRESS;
	static uint8_t counter = 0;
	static uint8_t n = 0;

	switch(State)
	{
		case CHECK_TIMEOUT_AND_START:		
			if (n++ >= MAX_ITER)
			{
				serialTransmit("Slave not responding");
				return 10;	
			}	
			/*Send start condition*/
			TWCR = 0;
			TWCR = (1<<TWINT)|(1<<TWIE)|(1<<TWSTA)|(1<<TWEN);
			State = CHECK_START_AND_LOAD_ADDRESS;
			break;
			
		case CHECK_START_AND_LOAD_ADDRESS: 
			switch ((twst = TW_STATUS))
			{
				case TW_REP_START:		/* OK, but should not happen */
				case TW_START:
					break;
				case TW_MT_ARB_LOST:	/*  A master must not initiate a stop condition in order to not corrupt the ongoing transfer from the active master. 
										This will cause a new start condition to be initiated, which will normally be delayed until the currently active master has 
										released the bus. */
					State = CHECK_TIMEOUT_AND_START;
					serialTransmit("TW_MR_ARB_LOST");
					return 1;
				default:
						sprintf(buffer, "Start I2C Fail, TW_STATUS:%x \n",twst);
						serialTransmit(buffer);
						return 1;/* error: not in start condition */
					  /* NB: do /not/ send stop condition */
			}
			
			 /*Load SLA_W into TWDR Register.*/
			TWDR = phys_adress;
			
			// start transmit
			TWCR = (1<<TWINT)|(1<<TWIE)|(1<<TWEN); //Clear TWINT bit in TWCR to start transmission of data/address
			State = CHECK_ACK_AND_LOAD_DATA_ADDRESS;
			break;
			
		case CHECK_ACK_AND_LOAD_DATA_ADDRESS: 
			switch ((twst = TW_STATUS))
			{
				case TW_MT_SLA_ACK:
					break;

				case TW_MT_SLA_NACK:	/* nack during select: device busy writing */
					State = CHECK_TIMEOUT_AND_START;
					serialTransmit("TW_MT_SLA_NACK");
					return 2;
				case TW_MT_ARB_LOST:	/* re-arbitrate */
					State = CHECK_TIMEOUT_AND_START;
					serialTransmit("TW_MR_ARB_LOST");
					return 2;
				default:
					sprintf(buffer, "Invalid phys add, TW_STATUS:%x \n",twst);
					serialTransmit(buffer);
					return 2;
			}

			//send the address of the register in the IMU chip we want to read
			TWDR = data_adress; //put register address on the bus
			// start transmit
			TWCR = (1<<TWINT)|(1<<TWIE)|(1<<TWEN); //Clear TWINT bit in TWCR to start transmission of data/address
			State = CHECK_DATA_ACK_AND_SEND_REPEATED_START;
			break;
				
		case CHECK_DATA_ACK_AND_SEND_REPEATED_START: 
			switch ((twst = TW_STATUS))
			{
				case TW_MT_DATA_ACK:
					break;
				case TW_MT_DATA_NACK:
					serialTransmit("TW_MT_DATA_NACK");
					return 3;
				case TW_MT_ARB_LOST:
					State = CHECK_TIMEOUT_AND_START;
					serialTransmit("TW_MR_ARB_LOST");
					return 4;
				default:
					serialTransmit("No Data Ack from Slave");
					return 4;
			}
		
			/*Send start condition*/
			TWCR = 0;
			TWCR = (1<<TWINT)|(1<<TWIE)|(1<<TWSTA)|(1<<TWEN);
			State = CHECK_REPEATED_START_AND_START_READ;
			break;
				
		case CHECK_REPEATED_START_AND_START_READ: 				 
			switch ((twst = TW_STATUS))
			{
				case TW_START:		// OK, but should not happen
				case TW_REP_START:	// expected
					break;
				case TW_MT_ARB_LOST:
					State = CHECK_TIMEOUT_AND_START;
					serialTransmit("TW_MR_ARB_LOST");
					return 5;
				default:
					serialTransmit("Rs error");
					return 5;
			}	

			/* send SLA+R */
			TWDR = phys_adress | TW_READ; //Load SLA_R into TWDR Register.	
			// start transmit
			TWCR = (1<<TWINT)|(1<<TWIE)|(1<<TWEN); //Clear TWINT bit in TWCR to start transmission of data/address
			State = CHECK_ACK_AND_SEND_READ_BYTE_REQUEST;
			break;

		case CHECK_ACK_AND_SEND_READ_BYTE_REQUEST:
			n = 0;
			switch ((twst = TW_STATUS))
			{
				case TW_MR_SLA_ACK:
				break;
				case TW_MR_SLA_NACK:
					serialTransmit("TW_MR_SLA_NACK");
					return 6;
				case TW_MR_ARB_LOST:
					State = CHECK_TIMEOUT_AND_START;
					serialTransmit("TW_MR_ARB_LOST");
					return 7;
				default:
					turnDebugLedOn(6);
					serialTransmit("MR_SLA_ACK ");
					return 7;
			}
			//enable the TWI ask for the byte to read
			TWCR = (1<<TWINT)|(1<<TWIE)|(1<<TWEN)|(1<<TWEA);
			State = READ_BYTE_AND_SEND_MORE_READ_BYTE_REQUEST;
			break;
	
		case READ_BYTE_AND_SEND_MORE_READ_BYTE_REQUEST:	
			switch ((twst = TW_STATUS))
			{
				case TW_MR_DATA_ACK:
					dataReadBuffer[counter] = TWDR;
					break;
				case TW_MR_DATA_NACK:
					dataReadBuffer[counter] = TWDR;
					//don't stop the I2C and repeat since the same sensor is read all the time
					//I2Cstop();
					break;
				default:
					turnDebugLedOn(7);
					serialTransmit("MR_SLA_ACK");
					return 8;
			}
			
				
			if(counter  < data_length-2)
			{
				TWCR = (1<<TWINT)|(1<<TWIE)|(1<<TWEN)|(1<<TWEA);
			}				
			else if(counter  == data_length-2)
			{
				TWCR = (1<<TWINT)|(1<<TWIE)|(1<<TWEN);
			}
			else
			{
				counter = 0;
				State = CHECK_START_AND_LOAD_ADDRESS;
				return 0; //finished reading bytes
			}
			counter++;
			break;
			
		default:
			return 9;
			break;	
	}
	return 9;
}
