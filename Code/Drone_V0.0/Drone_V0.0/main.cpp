#define F_CPU 1000000
#define min_PWM 0x01
#define max_PWM 0x06

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include "esc.h"
#include "interrupt.h"
#include "lcd.h"
#include "I2C.h"
#include "IMU.h"

int main()
{
	//initialize LCD
	initLCD(); 		/* configure LCD */	
	//char* buffers for printing stuff on the LCD
	char buffer[20];
	//create ESC object
	esc allESC;
	// initialize ESC
	allESC.initialize();
	//create I2C object
	I2C i2c;
	//initialize I2C
	i2c.initialize();
	//create IMU object
	IMU imu;
	//initialize IMU
	imu.initialize(i2c);
	
	//initialize INT
	initializeInterrupt();
	//initialize Radio Controller counter
	initializecounterPWMread();
	while(1)
	{	
		//if (button_falling)
		//{
			//allESC.set(1,1100/2);
			//allESC.set(2,1100/2);
			//allESC.set(3,1100/2);
			//allESC.set(4,1100/2);
			//clearDisplay();
			//LCD_WriteString("Salut Ben");
			//_delay_ms(1000);
			//button_falling = false;
		//}
		//else if (button_rising)
		//{				
			//button_rising = false;
		//}
		//else
		//{
			//allESC.set(1,ch_3_pw);
			//allESC.set(2,ch_3_pw);
			//allESC.set(3,ch_3_pw);
			//allESC.set(4,ch_3_pw);

			//clearDisplay();
			//sprintf(buffer, "%u    %u     ", ch_1_pw, ch_2_pw);
			//LCD_WriteString(buffer);
			//SetAdress(64);
			//sprintf(buffer, "%u    %u     ", ch_3_pw, ch_4_pw);
			//LCD_WriteString(buffer);
			//_delay_ms(100);
			//clearDisplay();
			//LCD_WriteString("   Yo ");
			//SetAdress(64); // goes to line 2 of LCD
			//LCD_WriteString("     Lau!");
			//_delay_ms(1000);			
			//clearDisplay();
			//LCD_WriteString("   Le meilleur");
			//SetAdress(64); // goes to line 2 of LCD
			//LCD_WriteString(" reste a venir!!");					
			//_delay_ms(1000);
			
			imu.IMUTakeMeasures(i2c);
			
			clearDisplay();	
			_delay_ms(20);
			sprintf(buffer, "X:%u  Y:%u", imu.gyrx, imu.gyry);
			LCD_WriteString(buffer);
			SetAdress(LINE2);
			sprintf(buffer, "Z:%u  Pres:%u", imu.gyrz, imu.pres);
			LCD_WriteString(buffer);
			_delay_ms(100);

			//clearDisplay();
			//_delay_ms(80);
			//sprintf(buffer, "X:%u  Y:%u", imu.gyrx, imu.gyry);
			//LCD_WriteString(buffer);
			//SetAdress(LINE2);
			//sprintf(buffer, "Z:%u  Pres:%u", imu.gyrz, imu.pres);
			//LCD_WriteString(buffer);
			//_delay_ms(400);		
			
			
			//clearDisplay();			
			////if(i2c.read(0x00, dataIn,1) == 1)
			//
			//clearDisplay();	
			//sprintf(buffer, "%u    %u     ", dataIn[0], dataIn[1]);
			//LCD_WriteString(buffer);		
			//if(i2c.read(0xAA, dataIn,1) == 0)
			//{
				//clearDisplay();
				//LCD_WriteString(" Success");
				//_delay_ms(100000);
			//}

			
		//}

	}
	return 0;
}
