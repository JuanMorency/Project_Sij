#define F_CPU 16000000
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
#include "WS2812.h"
#include "RF.h"
#include "debugLED.h"

int main()
{
	//initialize LCD
	initLCD(); 		// configure LCD
	//char* buffers for printing stuff on the LCD
	char buffer[20];
	char buffer2[20];
	
	//create ESC object
	Esc escFL(FL), escBL(BL),escBR(BR), escFR(FR);
	
	//create objects for led strips
	WS2812 LEDFRT(1, FRT); // 100 LED
	cRGB valueFRT;	
	
	//create IMU object
	IMU imu;	
	
	//Initialize modules; comment out to deactivate feature
	initLCD();
	//initRF();
	//initializeESC();
	//initWS2812();
	initializeI2C();
	imu.initialize();
	
	//After everything is initialized, start interrupts
	startInterrupt();
	while(1)
	{
		//LCD handler
		if(flagLCD){
			flagLCD = 0;
			handleFSMLCD();
		}

		//RF receiver handler
		if(flagRF)
		{
			flagRF = 0;
			handleFSMRF();
			sprintf(buffer, "1:%u 2:%u", ch_1_pw, ch_2_pw);
			sprintf(buffer2, "3:%u 4:%u", ch_3_pw, ch_4_pw);
			changeLCDText(buffer, buffer2);
		}
		
		//ESC handler
		if(flagESC)
		{
			flagESC = 0;
			escFL.set(ch_3_pw);
			escBL.set(ch_3_pw);
			escBR.set(ch_3_pw);
			escFR.set(ch_3_pw);
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
				LEDFRT.set_crgb_at(j, valueFRT); // Set value at LED found at index j
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
			sprintf(buffer, "x:%i y:%i", imu.rot.X, imu.rot.Y);
			sprintf(buffer2, "z:%i :%i", imu.rot.Z, imu.rot.Y);
			changeLCDText(buffer, buffer2);
		}
	}
	return 0;
}