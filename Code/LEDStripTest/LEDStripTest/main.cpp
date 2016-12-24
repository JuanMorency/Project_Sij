/*
 * light_ws2812 example
 *
 * Created: 07.03.2014 12:49:30
 *  Author: Matthias Riegler
 */ 
#define F_CPU 16000000UL
#include "WS2812.h"
#include <util/delay.h>

WS2812 LEDFRT(100, FRT); // 1 LED
WS2812 LEDFRB(10, FRB); // 1 LED	
cRGB valueFRT;
cRGB valueFRB;

int h = 0;   //stores 0 to 614
uint8_t steps = 15; //number of hues we skip in a 360 range per update

uint8_t sat = 255;
uint8_t val = 127;

 void Cycle()
 {
	 valueFRT.SetHSV(h, sat, val);
	 
	 h += steps;
	 if(h > 360)
	 {
		 h %= 360;
	 }
 }

int main()
{
	LEDFRT.reset();
	while(1)
	{	
		//Cycle();
 //
		//for(int i = 0; i < 10; i++)
		//{
			//LEDFRT.set_crgb_at(i, valueFRT);
		//}
		//// Sends the data to the LEDs
		//LEDFRT.sync();
 //
		//_delay_ms(50);	
		for(int j = 0; j<100; j++)
		{
			valueFRT.b = 255; valueFRT.g = 255; valueFRT.r = 255; // RGB Value -> White strong
			LEDFRT.set_crgb_at(j, valueFRT); // Set value at LED found at index j
			
			if(j>0)
			{
				valueFRT.b = 40; valueFRT.g = 40; valueFRT.r = 40; // RGB Value -> White weak
				LEDFRT.set_crgb_at(j-1, valueFRT); // Set value at LED found at index j-1
			}

			if(j>1)
			{
				valueFRT.b = 15; valueFRT.g = 15; valueFRT.r = 15; // RGB Value -> White weak
				LEDFRT.set_crgb_at(j-2, valueFRT); // Set value at LED found at index j-1
			}
			
			if(j>2)
			{
				valueFRT.b = 0; valueFRT.g = 0; valueFRT.r = 0; // RGB Value -> White weak
				LEDFRT.set_crgb_at(j-3, valueFRT); // Set value at LED found at index j-1
			}

			
			LEDFRT.sync(); // Sends the value to the LED
			_delay_ms(20); // Wait 500 ms
		}
		
	}
	

 }




