/*
* light weight WS2812 lib V2.1 - Arduino support
*
* Controls WS2811/WS2812/WS2812B RGB-LEDs
* Author: Matthias Riegler
*
* Mar 07 2014: Added Arduino and C++ Library
*
* September 6, 2014:	Added option to switch between most popular color orders
*						(RGB, GRB, and BRG) --  Windell H. Oskay
* 
* License: GNU GPL v2 (see License.txt)
*/

#include "WS2812.h"
#include <stdlib.h>
#include <avr/io.h>
#include <avr/portpins.h>

WS2812::WS2812(uint16_t num_leds, uint8_t stripPos) {
	count_led = num_leds;
	stripPosition = stripPos;
	pixels = (uint8_t*)malloc(count_led*3);
	#ifdef RGB_ORDER_ON_RUNTIME	
		offsetGreen = 0;
		offsetRed = 1;
		offsetBlue = 2;
	#endif
}

cRGB WS2812::get_crgb_at(uint16_t index) {
	
	cRGB px_value;
	
	if(index < count_led) {
		
		uint16_t tmp;
		tmp = index * 3;

		px_value.r = pixels[OFFSET_R(tmp)];
		px_value.g = pixels[OFFSET_G(tmp)];
		px_value.b = pixels[OFFSET_B(tmp)];
	}
	
	return px_value;
}

uint8_t WS2812::set_crgb_at(uint16_t index, cRGB px_value) {
	
	if(index < count_led) {
		
		uint16_t tmp;
		tmp = index * 3;
		
		pixels[OFFSET_R(tmp)] = px_value.r;
		pixels[OFFSET_G(tmp)] = px_value.g;
		pixels[OFFSET_B(tmp)] = px_value.b;		
		return 0;
	} 
	return 1;
}

uint8_t WS2812::set_subpixel_at(uint16_t index, uint8_t offset, uint8_t px_value) {
	if (index < count_led) {
		uint16_t tmp;
		tmp = index * 3;

		pixels[tmp + offset] = px_value;
		return 0;
	}
	return 1;
}

void WS2812::sync() {
	setAsOutput();
	*ws2812_port_reg |= pinMask; // Enable DDR 
	ws2812_sendarray_mask(pixels,3*count_led,pinMask,(uint8_t*) ws2812_port,(uint8_t*) ws2812_port_reg );	
}

#ifdef RGB_ORDER_ON_RUNTIME	
void WS2812::setColorOrderGRB() { // Default color order
	offsetGreen = 0;
	offsetRed = 1;
	offsetBlue = 2;
}

void WS2812::setColorOrderRGB() {
	offsetRed = 0;
	offsetGreen = 1;
	offsetBlue = 2;
}

void WS2812::setColorOrderBRG() {
	offsetBlue = 0;
	offsetRed = 1;
	offsetGreen = 2;
}
#endif

WS2812::~WS2812() {
	free(pixels);
	
}


void WS2812::setAsOutput()
{
	//seem to have problems with port F, except for F3..., can't do shit
	switch (stripPosition)
	{
	case FRT:
		setOutput(&PORTB,&DDRB, 6);
		break;
	case FRB:
		setOutput(&PORTB,&DDRB, 7);
		break;
	case FLT:
		setOutput(&PORTJ,&DDRJ, 7);
		break;
	case FLB:
		setOutput(&PORTJ,&DDRJ, 6);
		break;
	case BRT:
		setOutput(&PORTF,&DDRF, 7);
		break;
	case BRB:
		setOutput(&PORTF,&DDRF, 6);
		break;
	case BLT:
		setOutput(&PORTK,&DDRK, 0);
		break;
	case BLB:
		setOutput(&PORTK,&DDRK, 1);
		break;
	}

}

void WS2812::setOutput(volatile uint8_t *port, volatile uint8_t *ddr, uint8_t pin) 
{
	//seem to have problems with port F, except for F3..., can`t do shit
		pinMask = _BV(pin);
		ws2812_port = port;
		ws2812_port_reg = ddr;

}

void WS2812::reset()
{
	int ledNumber3x = count_led*3;
	for(int i = 0; i < ledNumber3x; i++)
	{
		pixels[i] = 0;
	}
}