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

int main()
{
	
	
	
	//initialize LCD
	initLCD(); 		// configure LCD
	//char* buffers for printing stuff on the LCD
	char buffer[20];
	//create ESC object
	Esc escFL(FL), escBL(BL),escBR(BR), escFR(FR);
	// initialize ESC
	initializeESC();
	//AllESC.set(1,1100/2);
	//AllESC.set(2,1100/2);
	//AllESC.set(3,1100/2);
	//AllESC.set(4,1100/2);
	//_delay_ms(5000);
	//
	////Initialize I2C object
	//initializeI2C();
	//clearDisplay();
	//_delay_ms(1000);
	//LCD_WriteString("I2C");
//
	//////create IMU object
	////IMU Imu;
	//////initialize IMU
	////Imu.initialize();
	////clearDisplay();
	////LCD_WriteString("IMU");
	////_delay_ms(1000);
	
	//LED_Strip
	WS2812 LEDFRT(1, FRT); // 1 LED
	WS2812 LEDFRB(1, FRB); // 1 LED
	cRGB valueFRT;
	cRGB valueFRB;
	//initialize INT
	initializeInterrupt();
	//initialize Radio Controller counter
	initializecounterPWMread();
	
	int i = 0;
	while(1)
	{	
		//if (button_falling)
		//{
			//AllESC.set(1,1100/2);
			//AllESC.set(2,1100/2);
			//AllESC.set(3,1100/2);
			//AllESC.set(4,1100/2);
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
			escFL.set(ch_3_pw);
			escBL.set(ch_3_pw);
			escBR.set(ch_3_pw);
			escFR.set(ch_3_pw);

			clearDisplay();
			sprintf(buffer, "%u    %u     ", ch_1_pw, ch_2_pw);
			LCD_WriteString(buffer);
			SetAdress(64);
			sprintf(buffer, "%u    %u     ", ch_3_pw, ch_4_pw);
			LCD_WriteString(buffer);
			_delay_ms(100);
			
			i++;
			if(i == 5)
			{
				valueFRT.b = 255; valueFRT.g = 0; valueFRT.r = 0; // RGB Value -> Blue
				valueFRB.b = 0; valueFRB.g = 255; valueFRB.r = 0; // RGB Value -> Blue
				LEDFRT.set_crgb_at(0, valueFRT); // Set value at LED found at index 0
				LEDFRB.set_crgb_at(0, valueFRB); // Set value at LED found at index 0
				LEDFRT.sync(); // Sends the value to the LED
				LEDFRB.sync(); // Sends the value to the LED				
			}

			else if(i == 10)
			{
				valueFRT.b = 0; valueFRT.g = 0; valueFRT.r = 0; // RGB Value -> Blue
				valueFRB.b = 0; valueFRB.g = 0; valueFRB.r = 0; // RGB Value -> Blue
				LEDFRT.set_crgb_at(0, valueFRT); // Set value at LED found at index 0
				LEDFRB.set_crgb_at(0, valueFRB); // Set value at LED found at index 0
				LEDFRT.sync(); // Sends the value to the LED
				LEDFRB.sync(); // Sends the value to the LED
				i = 0;
			}
			
			//Imu.IMUTakeMeasures();
			
			//clearDisplay();	
			//_delay_ms(20);
			//sprintf(buffer, "X:%u  Y:%u", imu.gyrx, imu.gyry);
			//LCD_WriteString(buffer);
			//SetAdress(LINE2);
			//sprintf(buffer, "Z:%u  Pres:%u", imu.gyrz, imu.pres);
			//LCD_WriteString(buffer);
			//_delay_ms(10);
			
			
			//clearDisplay();
			//_delay_ms(20);
			//sprintf(buffer, "P:%i  T:%i", imu.pres, imu.temp);
			//LCD_WriteString(buffer);
			//SetAdress(LINE2);
			//sprintf(buffer, "A:%i P0:%i", imu.alt, getPressure0());
			//LCD_WriteString(buffer);
			//_delay_ms(10);

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
