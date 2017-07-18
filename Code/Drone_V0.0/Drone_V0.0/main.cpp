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
#include "PID.h"

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
	initializeESC();
	//initWS2812();
	initializeI2C();
	imu.initialize();
	initSerial(MYUBRR);
	startInterrupt();
	//After everything is initialized, start interrupts
	
	
	//PID objects
	PID yawPid(YAW,YAW_KP,YAW_KI,YAW_KD);
	PID pitchPid(PITCH,PITCH_KP,PITCH_KI,PITCH_KD);
	PID rollPid(ROLL,ROLL_KP,ROLL_KI,ROLL_KD);
	
	while(1)
	{
		imu.updateImuAndMadgwick();

		if(flagSerial)
		{
			flagSerial = false;

			// print IMU sensor values
			
			//sprintf(buffer, "ACC: x:%i y:%i z:%i\t\tGYR: x:%i y:%i z:%i\t\tMAG: x:%i y:%i z:%i\t\tP:%li A:%li T:%li",
			//imu.mpu9255.getAccelerationX(), imu.mpu9255.getAccelerationY(), imu.mpu9255.getAccelerationZ(), 
			//imu.mpu9255.getRotationX(), imu.mpu9255.getRotationY(), imu.mpu9255.getRotationZ(), 
			//imu.ak8963.getMagneticFieldX(), imu.ak8963.getMagneticFieldY(), imu.ak8963.getMagneticFieldZ(), imu.bmp180.getPressure(), imu.bmp180.getAltitude(), imu.bmp180.getTemperature());
				
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

			////print yaw pitch roll
			//
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
			//
			//
			sprintf(buffer,"yawAdj:%i\tPitchAdj:%i\tRollAdj:%i\t", yawPid.getAdjustment(),
			pitchPid.getAdjustment(), rollPid.getAdjustment());
			FloatToString(floatbuff, yaw);
			strcat (buffer,"\tyaw:");
			strcat (buffer,floatbuff);
			FloatToString(floatbuff, pitch);
			strcat (buffer,"\tpitch:");
			strcat (buffer,floatbuff);
			FloatToString(floatbuff, roll);
			strcat (buffer,"\troll:");
			strcat (buffer,floatbuff);
			
			////Print sensor update rate
			//
			//strcat (buffer,"\t\trates: ");
			////calculate and print the update frequency of AK8963
			//if (sumAk8963 != 0)
			//{
				//FloatToString(floatbuff, (float)sumCountAk8963/sumAk8963);
				//strcat (buffer,"Mag:");
				//strcat (buffer,floatbuff);
			//}
			//else
			//{
				//strcat (buffer,"Mag:Too fast ");
			//}
			////reset the counters for update rate calculations
			//sumCountAk8963 = 0;
			//sumAk8963 = 0;
			//
			//
			////calculate and print the update frequency of MPU9255
			//if (sumMpu9255 != 0)
			//{
				//FloatToString(floatbuff, (float)sumCountMpu9255/sumMpu9255);
				//strcat (buffer,"\t Acc: ");
				//strcat (buffer,floatbuff);
			//}
			//else
			//{
				//strcat (buffer,"\t Acc:Too fast ");
			//}
			////reset the counters for update rate calculations
			//sumCountMpu9255 = 0;
			//sumMpu9255 = 0;
//
			////calculate and print the update frequency of BMP180
			//if (sumBmp180 != 0)
			//{
				//FloatToString(floatbuff, (float)sumCountBmp180/sumBmp180);
				//strcat (buffer,"\t Pre:");
				//strcat (buffer,floatbuff);
			//}
			//else
			//{
				//strcat (buffer,"\t Pre:Too slow ");
			//}
			////reset the counters for update rate calculations
			//sumCountBmp180 = 0;
			//sumBmp180 = 0;

			strcat (buffer,"\n");
			serialTransmit(buffer);

			//sprintf(buffer,"pitch adj: %i, FL: %i, BL: %i, BR: %i, FR: %i \t\t", pitchPid.getAdjustment(), FlSpeed, BlSpeed, BrSpeed, FrSpeed);
			//FloatToString(floatbuff, getDesiredAngleFromRf(PITCH));
			//strcat (buffer,"desired pitch:");
			//strcat (buffer,floatbuff);
			//FloatToString(floatbuff, pitch);
			//strcat (buffer,"\t\t actual pitch:");
			//strcat (buffer,floatbuff);
			//strcat (buffer,"\n");
			//
			//serialTransmit(buffer);
			
			
		}

		//ESC handler
		if(flagEsc)
		{
			flagEsc = 0;
			if(RFInitialized)
			{
				//calculate the PID adjustments
				yawPid.updatePid(yaw,getDesiredAngleFromRf(YAW));
				pitchPid.updatePid(pitch,-getDesiredAngleFromRf(PITCH));
				rollPid.updatePid(roll,getDesiredAngleFromRf(ROLL));
	
				//calculate the PWM to send to the ESCs
				
				//be careful here if we get a negative value before the type casting, we get some weird stuff. 
				//maybe make sure not negative before doing this for safety
				FlSpeed = (uint16_t)(ESC_INIT_PW + ch_3_pw - CHANNEL_3_MIN_PWM + rollPid.getAdjustment() + pitchPid.getAdjustment() - yawPid.getAdjustment());
				BlSpeed = (uint16_t)(ESC_INIT_PW + ch_3_pw - CHANNEL_3_MIN_PWM + rollPid.getAdjustment() - pitchPid.getAdjustment() + yawPid.getAdjustment());
				BrSpeed = (uint16_t)(ESC_INIT_PW + ch_3_pw - CHANNEL_3_MIN_PWM - rollPid.getAdjustment() - pitchPid.getAdjustment() - yawPid.getAdjustment());
				FrSpeed = (uint16_t)(ESC_INIT_PW + ch_3_pw - CHANNEL_3_MIN_PWM - rollPid.getAdjustment() + pitchPid.getAdjustment() + yawPid.getAdjustment());			
				
				//Set FL ESC
				if(FlSpeed < ESC_INIT_PW) escFL.set(ESC_INIT_PW);
				else if (FlSpeed > ESC_MAX_PW) escFL.set(ESC_MAX_PW);
				else escFL.set(FlSpeed);

				//Set BL ESC
				if(BlSpeed < ESC_INIT_PW) escBL.set(ESC_INIT_PW);
				else if (BlSpeed > ESC_MAX_PW) escBL.set(ESC_MAX_PW);
				else escBL.set(BlSpeed);

				//Set BR ESC
				if(BrSpeed < ESC_INIT_PW) escBR.set(ESC_INIT_PW);
				else if (BrSpeed > ESC_MAX_PW) escBR.set(ESC_MAX_PW);
				else escBR.set(BrSpeed);
				
				//Set FR ESC
				if(FrSpeed < ESC_INIT_PW) escFR.set(ESC_INIT_PW);
				else if (FrSpeed > ESC_MAX_PW) escFR.set(ESC_MAX_PW);
				else escFR.set(FrSpeed);				
			}
			else
			{
				escFL.set(ESC_INIT_PW);
				escBL.set(ESC_INIT_PW);
				escBR.set(ESC_INIT_PW);
				escFR.set(ESC_INIT_PW);
			}

			//escFL.set(ch_3_pw);
			//escBL.set(ch_3_pw);
			//escBR.set(ch_3_pw);
			//escFR.set(ch_3_pw);
		}
		
		if(flagWs2812)
		{
			flagWs2812 = 0;
			
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
	}
	return 0;
}