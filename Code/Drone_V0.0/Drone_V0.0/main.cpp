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




#define LED0B 0x01
#define LED0R 0x02
#define LED0J 0x04
#define LED1B 0x08
#define LED1R 0x10
#define LED1J 0X20
#define LED2B 0x40
#define LED2R 0x80

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
		////minimum useless operation in the while loop, otherwise bugs
		DDRC = 0xFF;

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
			sprintf(buffer, "x:%i y:%i", imu.acc.X, imu.acc.Y);
			sprintf(buffer2, "z:%i m:%i", imu.acc.Z, imu.mag.X);
			changeLCDText(buffer, buffer2);
		}
		
			//static uint8_t data[20];
			//data[0] = readI2C(MP9255_ADDRESS,ACCEL_XOUT_H);
			//data[1] = readI2C(MP9255_ADDRESS,ACCEL_XOUT_L);
			//data[2] = readI2C(MP9255_ADDRESS,ACCEL_YOUT_H);
			//data[3] = readI2C(MP9255_ADDRESS,ACCEL_YOUT_L);
			//data[4] = readI2C(MP9255_ADDRESS,ACCEL_ZOUT_H);
			//data[5] = readI2C(MP9255_ADDRESS,ACCEL_ZOUT_L);
			//while(readI2C(MP9255_ADDRESS,ACCEL_XOUT_H, data,14)) //problem with read I2C for more than 1 data.
			//{
				//readI2C(MP9255_ADDRESS,ACCEL_XOUT_H, data,14);
				//_delay_ms(2);
				//imu.accRaw.X = (data[0] << 8) | data[1];
				//imu.accRaw.Y = (data[2] << 8) | data[3];
				//imu.accRaw.Z = (data[4] << 8) | data[5];
				////tempRaw =	(data[6] << 8) | data[7];
				////gyroRaw.X = (data[8] << 8) | data[9];
				////gyroRaw.Y = (data[10] << 8) | data[11];
				////gyroRaw.Z = (data[12] << 8) | data[13];
				//static uint8_t i = 0;
				////writeI2C(MP9255_ADDRESS,0x31, i);
				//sprintf(buffer, "I2C:%i, test %i, %i",  writeI2C(MP9255_ADDRESS,0x31, i), readI2C(MP9255_ADDRESS,0x31), i);
				////sprintf(buffer2, "z:%i M:%i  %u", imu.accRaw.Z, imu.gyroRaw.X, i);
				//changeLCDText(buffer, buffer2);
				//i=1-i;
			//}
			//_delay_ms(50);
	}
	return 0;
}