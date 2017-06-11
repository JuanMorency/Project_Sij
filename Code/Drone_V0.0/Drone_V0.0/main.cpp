#define F_CPU 16000000

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
#include "WS2812.h"
#include "RF.h"
#include "debugLED.h"
#include "serial.h"
#include "MadgwickAHRS.h"


int main()
{	
	//create ESC objec
	Esc escFL(FL), escBL(BL),escBR(BR), escFR(FR);
	
	//create objects for led strips
	WS2812 LEDFRT(1, FRT); // 100 LED
	cRGB valueFRT;	
	
	//create IMU object
	IMU imu;	
	//Initialize modules; comment out to deactivate feature
	//initLCD();
	initRF();
	initializeESC();
	//initWS2812();
	initializeI2C();
	imu.initialize();
	initSerial(MYUBRR);
	startInterrupt();
	//After everything is initialized, start interrupts



	
	while(1)
	{
		//LCD handler
		if(flagLCD)
		{
			flagLCD = 0;
			handleFSMLCD();
		}
		
		//ESC handler
		if(flagESC)
		{
			flagESC = 0;
			escFL.set(2000);
			escBL.set(2000);
			escBR.set(2000);
			escFR.set(2000);
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

		if(flagIMU)
		{
			flagIMU = 0;
			imu.takeMeasures();
			if (IMUserialSlowDownCounter >= IMU_SERIAL_SPEED_DIVIDER)
			{
				IMUserialSlowDownCounter = 0;
				sprintf(buffer, "ACC: x:%i y:%i z:%i\t\tGYR: x:%i y:%i z:%i\t\tMAG: x:%i y:%i z:%i\t\tP:%li A:%li T:%li \t\t q0:%f q1:%f q2:%f q3:%f\n", 
				imu.acc.X, imu.acc.Y, imu.acc.Z, imu.rot.X, imu.rot.Y, imu.rot.Z, imu.mag.X, imu.mag.Y, imu.mag.Z,imu.pres, 
				imu.alt, imu.temp, q0, q1, q2, q3);
				serialTransmit(buffer);
			}
			else
			{
				IMUserialSlowDownCounter++;
			}
		}
	}
	return 0;
}