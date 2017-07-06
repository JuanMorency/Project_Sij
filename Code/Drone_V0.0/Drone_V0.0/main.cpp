/**
******************************************************************************
* File Name         : main.cpp
* Description       : Full sensor acquisition, filtering fusion and PID controller for a 600 mm drone. This software also controls outputs to WS2812B 
*						LED strips, the motor controllers and serial communication for debugging. This code is meant to work with an atmega2560 and an MPU9255 AHRS.  
* Author			: Juan Morency Trudel, Simon Poirier.
* Version           : 1.0.0
* Date				: June 2017
******************************************************************************
*/

#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "esc.h"
#include "interrupt.h"
#include "lcd.h"
#include "I2C.h"
#include "IMU.h"
#include "WS2812.h"
#include "RF.h"
#include "debugLED.h"
#include "serial.h"
#include "MadgwickAHRS.h"

int main()
{	
	//create ESC objects
	Esc escFL(FR), escBL(BR),escBR(BL), escFR(FL);
	
	//create objects for led strips
	WS2812 LEDFRT(1, FRT); // 100 LED
	cRGB valueFRT;	
	
	//create IMU object
	IMU imu;
		
	//Initialize modules; comment out to deactivate feature
	initRF();
	//initializeESC();
	//initWS2812();
	initializeI2C();
	imu.initialize();
	initSerial(MYUBRR);
	startInterrupt(imu);
	//After everything is initialized, start interrupts
	while(1)
	{		
		//ESC handler
		if(flagESC)
		{
			flagESC = 0;
			escFL.set(2000 - roll*50 + pitch*50);
			escBL.set(2000 - roll*50 - pitch*50);
			escBR.set(2000 + roll*50 - pitch*50);
			escFR.set(2000 + roll*50 + pitch*50);
		}
		
		if(flagWS2812)
		{
			flagWS2812 = 0;
			
			//example of LED gradually
			static int i = 0;
			static bool directionUp = true;
			//Led strips shit
			for(int j = 0; j<1; j++)
			{
				valueFRT.b = i; valueFRT.g = i; valueFRT.r = i; // RGB Value -> red
				LEDFRT.set_crgb_at(j, valueFRT); // Set valueB at LED found at index j
				valueFRT.b = 255; valueFRT.g = 255; valueFRT.r = 255; // RGB Value -> Blue
			}
			LEDFRT.sync(); // Sends the value to the LED
			if(i>=255) directionUp = false;
			if(i<=0) directionUp = true;
			if (directionUp) i++;
			else i--;
		}
		
		//if(flagBMP180 && !I2CInterruptBusy)
		//{
			//flagBMP180 = false;
			//
			//imu.bmp180.CalculateTemperaturePressureAndAltitude();
			//
			//// Send start condition to reactivate I2C interrupts
			////TWCR = 0;
			////TWCR = (1<<TWINT)|(1<<TWIE)|(1<<TWSTA)|(1<<TWEN);
		//}
		
		if(flagIMU)
		{
			flagIMU = 0;
			imu.updateMadgwick();
			if (IMUserialSlowDownCounter >= IMU_SERIAL_SPEED_DIVIDER)
			{
				IMUserialSlowDownCounter = 0;
				//sprintf(buffer, "ACC: x:%i y:%i z:%i\t\tGYR: x:%i y:%i z:%i\t\tMAG: x:%i y:%i z:%i\t\tP:%li A:%li T:%li",
				//imu.acc.X, imu.acc.Y, imu.acc.Z, imu.rot.X, imu.rot.Y, imu.rot.Z, imu.mag.X, imu.mag.Y, imu.mag.Z,imu.pres,
				//imu.alt, imu.temp);

				// print sensor values
				
				//sprintf(buffer, "ACC: x:%i y:%i z:%i\t\tGYR: x:%i y:%i z:%i\t\tMAG: x:%i y:%i z:%i",
				//imu.acc.X, imu.acc.Y, imu.acc.Z, imu.rot.X, imu.rot.Y, imu.rot.Z, imu.mag.X, imu.mag.Y, imu.mag.Z);

				sprintf(buffer, "ACC: x:%i y:%i z:%i\t\tGYR: x:%i y:%i z:%i\t\tMAG: x:%i y:%i z:%i\t\tP:%li A:%li T:%li",
				currentRawAcc.X, currentRawAcc.Y, currentRawAcc.Z, currentRawGyr.X, currentRawGyr.Y, currentRawGyr.Z, currentRawMag.X, currentRawMag.Y, currentRawMag.Z, imu.bmp180.getPressure(), imu.bmp180.getAltitude(), imu.bmp180.getTemperature() );
				
				//// print quaternions
//
				//FloatToString(floatbuff, q0);
				//strcat (buffer,"\t\t q0:");
				//strcat (buffer,floatbuff);
				//FloatToString(floatbuff, q1);
				//strcat (buffer,"\t q1:");
				//strcat (buffer,floatbuff);
				//FloatToString(floatbuff, q2);
				//strcat (buffer,"\t q2:");
				//strcat (buffer,floatbuff);
				//FloatToString(floatbuff, q3);
				//strcat (buffer,"\t q3:");
				//strcat (buffer,floatbuff);

				//print yaw pitch roll
				
				//sprintf(buffer, " ");				
				//FloatToString(floatbuff, roll);
				//strcat (buffer,"roll:");
				//strcat (buffer,floatbuff);
				//FloatToString(floatbuff, pitch);
				//strcat (buffer,"\t pitch:");
				//strcat (buffer,floatbuff);
				//FloatToString(floatbuff, yaw);
				//strcat (buffer,"\t yaw:");
				//strcat (buffer,floatbuff);
				
				//calculate and print the update frequency
				
				FloatToString(floatbuff, (float)sumCount/sum);
				//reset the counters for update rate calculations for IMU
				sumCount = 0;
				sum = 0;
				strcat (buffer,"\t rate: ");
				strcat (buffer,floatbuff);			
				strcat (buffer,"\n");
				serialTransmit(buffer);
				//sprintf(buffer, "TWINT time: %d", timeWaitForTWINT);
				//serialTransmit(buffer);
			}
			else
			{
				IMUserialSlowDownCounter++;
			}
		}
	}
	return 0;
}